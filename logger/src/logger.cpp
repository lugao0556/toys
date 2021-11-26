#include "logger.h"

#include <iostream>
#include <errno.h>
#include <unistd.h>//access, getpid
#include <assert.h>//assert
#include <stdarg.h>//va_list
#include <sys/stat.h>//mkdir
#include <sys/syscall.h>//system call

logger* logger::_ins = nullptr;

pthread_once_t logger::_once = PTHREAD_ONCE_INIT;
pthread_cond_t logger::_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t logger::_mutex = PTHREAD_MUTEX_INITIALIZER;

uint32_t logger::_buff_size = 30 * 1024 * 1024; // 30MB

#define BUF_NUM 3
#define LOG_USE_LIMIT (1u * 1024 * 1024 * 1024) // 1GB
#define BUFF_WAIT_TIME 1
#define RELOG_THRESOLD 5
#define LOG_LEN_LIMIT (4 * 1024) // 4K
#define MEM_USE_LIMIT (3u * 1024 * 1024 * 1024) // 3GB

pid_t gettid() {
    return syscall(__NR_gettid);
}

logger::logger() :
    _buff_num(BUF_NUM),
    _curr_buf(nullptr),
    _persist_buf(nullptr),
    _fp(nullptr),
    _log_cnt(0),
    _env_ok(false),
    _level(INFO),
    _timer(nullptr) {

    create_double_linked_ring();
    _pid = getpid();
    utc_timer t;
    _timer = &t;
}

void logger::init_app(const char* log_dir, const char* app_name, int level) {

    pthread_mutex_lock(&_mutex);

    strncpy(_log_dir, log_dir, MAX_LOG_DIR_LEN);
    strncpy(_app_name, app_name, MAX_APP_NAME_LEN);

    pthread_mutex_unlock(&_mutex);

    mkdir(_log_dir, 0777);
    if (access(_log_dir, F_OK | W_OK) == -1) {
        fprintf(stderr, "logdir: %s error: %s\n", _log_dir, strerror(errno));
    } else {
        _env_ok = true;
    }

    if (level > TRACE) {
        level = TRACE;
    }
    if (level < FATAL) {
        level = FATAL;
    }
    _level = level;  
}

void logger::persist() {
    // forever loop
    while (true) {
        pthread_mutex_lock(&_mutex);

        if (_persist_buf->status == buffer_node::FREE) {
            struct timespec sp_timer;
            struct timeval now;
            gettimeofday(&now, nullptr);
            sp_timer.tv_sec = now.tv_sec;
            sp_timer.tv_nsec = now.tv_usec * 1000; // nanoseconds
            sp_timer.tv_sec += BUFF_WAIT_TIME; // wait for 1 seconds
            pthread_cond_timedwait(&_cond, &_mutex, &sp_timer);
        }

        if (_persist_buf->empty()) {
            // give up, go to next run
            pthread_mutex_unlock(&_mutex);
            continue;

        }

        if (_persist_buf->status == buffer_node::FREE) {
            assert(_curr_buf == _persist_buf);
            _curr_buf->status = buffer_node::FULL;
            _curr_buf = _curr_buf->next;
        }

        int year = _timer->year, mon = _timer->mon, day = _timer->day;

        pthread_mutex_unlock(&_mutex);

        if (!decis_file(year, mon, day)) {
            continue;
        }

        _persist_buf->persist(_fp);
        fflush(_fp);

        pthread_mutex_lock(&_mutex);
        _persist_buf->clear();
        _persist_buf = _persist_buf->next;
        pthread_mutex_unlock(&_mutex);
    }
}

void logger::try_append(const char* lvl, const char* format, ...) {
    int ms;
    uint64_t curr_sec = _timer->get_curr_time(&ms);

    if (_lst_lts && curr_sec - _lst_lts < RELOG_THRESOLD) {
        return;
    }
    
    char log_line[LOG_LEN_LIMIT];
    int prev_len = snprintf(log_line, LOG_LEN_LIMIT, "%s[%s.%03d]", lvl, _timer->utc_fmt, ms);

    va_list arg_ptr;
    va_start(arg_ptr, format);
    int main_len = vsnprintf(log_line + prev_len, LOG_LEN_LIMIT - prev_len, format, arg_ptr);
    va_end(arg_ptr);

    uint32_t len = prev_len + main_len;

    _lst_lts = 0;
    bool notify_flag = false;

    pthread_mutex_lock(&_mutex);
    if (_curr_buf ->status == buffer_node::FREE && _curr_buf->avail_len() >= len) {
        _curr_buf->append(log_line, len);
    } else {
        //1. _curr_buf->status = cell_buffer::FREE but _curr_buf->avail_len() < len
        //2. _curr_buf->status = cell_buffer::FULL
        if (_curr_buf -> status == buffer_node::FREE) {
            _curr_buf->status = buffer_node::FULL;
            buffer_node* next_buf = _curr_buf->next;
            notify_flag = true;

            if (next_buf->status == buffer_node::FULL) {
                // it means that next_buf is under the persist job
                if (_buff_size * (_buff_num + 1) > MEM_USE_LIMIT) {
                    fprintf(stderr, "no more log space can use\n");
                    _curr_buf = next_buf;
                    _lst_lts = curr_sec;
                } else {
                    // insert a new buffer
                    buffer_node* new_buffer = new buffer_node(_buff_size);
                    _buff_num += 1;
                    new_buffer->prev = _curr_buf;
                    _curr_buf->next = new_buffer;
                    new_buffer->next = next_buf;
                    next_buf->prev = new_buffer;
                    _curr_buf = new_buffer;
                }
            } else {
                _curr_buf = next_buf;
            }

            if (!_lst_lts) {
                _curr_buf->append(log_line, len);
            }
        } else {
            //_curr_buf->status == cell_buffer::FULL, assert persist is on here too!
            _lst_lts = curr_sec;
        }
    }
    pthread_mutex_unlock(&_mutex);
    
    if (notify_flag) {
        pthread_cond_signal(&_cond);
    }
}

// this api is used as args for function pthread
void* do_thread_routine(void* args) {
    logger::ins()->persist();
    return nullptr;
}


/*------------------------------------------------
* Internal Function Defination Part
*-------------------------------------------------*/
void logger::create_double_linked_ring() {
    buffer_node* head = new buffer_node(_buff_size);
    if (!head) {
        std::cout << "no memory existed to allocte buffer node\n";
        exit(1);
    }

    buffer_node* pre_node = head;
    buffer_node* curr_node = nullptr;
    for (uint32_t i = 1; i < _buff_num; i++) {
        curr_node = new buffer_node(_buff_size);
        pre_node->next = curr_node;
        curr_node->prev = pre_node;

        pre_node = curr_node;
    }

    curr_node->next = head;
    head->prev = curr_node;

    _curr_buf = head;
    _persist_buf = head;
}


bool logger::decis_file(int year, int mon, int day) {

    if (_env_ok) {
        if (_fp) {
            fclose(_fp);
        }

        _fp = fopen("/dev/null", "w");
        return _fp != nullptr;
    }

    if (!_fp) {
        _year = year, _mon = mon, _day = day;
        char log_path[1024] = {};
        sprintf(log_path, "%s/%s.%d%02d%02d.%u.log", _log_dir, _app_name, _year, _mon, _day, _pid);
        _fp = fopen(log_path, "w");
        if (_fp) {
            _log_cnt += 1;
        }
    } else if (_day != day) {
        fclose(_fp);
         _year = year, _mon = mon, _day = day;
        char log_path[1024] = {};
        sprintf(log_path, "%s/%s.%d%02d%02d.%u.log", _log_dir, _app_name, _year, _mon, _day, _pid);
        _fp = fopen(log_path, "w");
        if (_fp) {
            _log_cnt += 1;
        }
    } else if (ftell(_fp) >= LOG_USE_LIMIT) {
        fclose(_fp);
        char old_path[1024] = {};
        char new_path[1024] = {};
        //mv xxx.log.[i] xxx.log.[i + 1]
        for (int i = _log_cnt - 1; i > 0; --i)
        {
            sprintf(old_path, "%s/%s.%d%02d%02d.%u.log.%d", _log_dir, _app_name, _year, _mon, _day, _pid, i);
            sprintf(new_path, "%s/%s.%d%02d%02d.%u.log.%d", _log_dir, _app_name, _year, _mon, _day, _pid, i + 1);
            rename(old_path, new_path);
        }
        //mv xxx.log xxx.log.1
        sprintf(old_path, "%s/%s.%d%02d%02d.%u.log", _log_dir, _app_name, _year, _mon, _day, _pid);
        sprintf(new_path, "%s/%s.%d%02d%02d.%u.log.1", _log_dir, _app_name, _year, _mon, _day, _pid);
        rename(old_path, new_path);
        _fp = fopen(old_path, "w");
        if (_fp) {
            _log_cnt += 1;
        }
    }

    return _fp != nullptr;
}