#ifndef  LOGGER_H_
#define LOGGER_H_

// refer to https://github.com/LeechanX/Ring-Log

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>//getpid, gettid

enum LOG_LEVEL {
    FATAL = 1,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
};

#define MAX_LOG_DIR_LEN 512
#define MAX_APP_NAME_LEN 128

extern pid_t gettid();

struct utc_timer
{  
    // public data exported to user
    int year, mon, day, hour, min, sec;
    char utc_fmt[20];

    utc_timer() {
        struct timeval tv;
        gettimeofday(&tv, nullptr);

        // set _sys_acc_sec, _sys_acc_min
        _sys_acc_sec = tv.tv_sec;
        _sys_acc_min = tv.tv_sec / 60;

        // use _sys_acc_sec calc year, mon, day, hour, min, sec
        struct tm cur_tm;
        localtime_r((time_t*)&_sys_acc_sec, &cur_tm);
        year = cur_tm.tm_year + 1900;
        mon = cur_tm.tm_mon + 1;
        day = cur_tm.tm_mday;
        hour  = cur_tm.tm_hour;
        min  = cur_tm.tm_min;
        sec  = cur_tm.tm_sec;

        set_utc_fmt();
    }

    uint64_t get_curr_time(int* p_msec = nullptr) {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        if (p_msec) {
            *p_msec = tv.tv_usec / 1000;
        }

        if ((uint32_t)tv.tv_sec != _sys_acc_sec) {
            // not in same second
            sec = tv.tv_sec % 60;
            _sys_acc_sec = tv.tv_sec;
            if (_sys_acc_sec / 60  != _sys_acc_min) {
                // not in same minute
                //use _sys_acc_sec update year, mon, day, hour, min, sec
                _sys_acc_min = _sys_acc_sec / 60;
                struct tm cur_tm;
                localtime_r((time_t*)&_sys_acc_sec, &cur_tm);
                year = cur_tm.tm_year + 1900;
                mon  = cur_tm.tm_mon + 1;
                day  = cur_tm.tm_mday;
                hour = cur_tm.tm_hour;
                min  = cur_tm.tm_min;
                //reformat utc format
                set_utc_fmt();

            } else {
                // only update the second
                set_utc_sec_fmt();
            }

        } else {
            // in same second
            return tv.tv_sec;
        }

    }

    // internal data/api only used for class function
private:
    uint64_t _sys_acc_min;
    uint64_t _sys_acc_sec;

    void set_utc_fmt() {
        snprintf(utc_fmt, 20, "%d-%02d-%02d %02d:%02d:%02d", year, mon, day, hour, min, sec);
    }

    void set_utc_sec_fmt() {
        snprintf(utc_fmt + 17, 3, "%02d", sec);
    }

};

// Essentially, buffer node is the node of Circular doubly linked list 
class buffer_node {
public:
    enum buffer_status {FREE, FULL};
    buffer_status status;
    buffer_node* prev;
    buffer_node* next;

    buffer_node(uint32_t len) : 
        status(FREE), 
        prev(nullptr), 
        next(nullptr), 
        _total_len(len), 
        _used_len(0) {
        
        _data = new char[len];
        if (!_data) {
            fprintf(stderr, "no space to allocate _data\n");
            exit(1);
        }
    }

    bool empty() const {return _used_len == 0;}

    void clear() {_used_len = 0; status = FREE;}

    uint32_t avail_len() const {return _total_len - _used_len;}

    void append(const char* log_str, uint32_t len) {
        if (avail_len() < len) {
            return;
        }
        memcpy(_data + _used_len, log_str, len);
        _used_len += len;
    }

    void persist(FILE* fp) {
        uint32_t wt_len = fwrite(_data, 1, _used_len, fp);
        if (wt_len != _used_len) {
             fprintf(stderr, "write log to disk error, wt_len %u\n", wt_len);
        }
    }

private:
    buffer_node(const buffer_node&);
    buffer_node& operator=(const buffer_node&);

    uint32_t _total_len;
    uint32_t _used_len;
    char* _data;

};

class logger {

public:
    // for thread-safe singleton
    static logger* ins() {
        pthread_once(&_once, logger::init);
        return _ins;
    }

    static void init() {
        while (!_ins) _ins = new logger();
    }

     int get_level() const { return _level;}

    void init_app(const char* log_dir, const char* app_name, int level);

    void persist();

    void try_append(const char* lvl, const char* format, ...);

private:
    logger();
    logger(const logger&);
    logger& operator=(const logger&);

    static logger* _ins;
    static pthread_once_t _once;

    static pthread_cond_t _cond;
    static pthread_mutex_t _mutex;
    static uint32_t _buff_size;

    char _app_name[MAX_APP_NAME_LEN];
    char _log_dir[MAX_LOG_DIR_LEN];

    bool _env_ok;
    uint8_t _level;
    uint32_t _buff_num;
    int _year, _mon, _day, _log_cnt;

    buffer_node* _curr_buf; // producer throw log into this buffer
    buffer_node* _persist_buf; // consumer retrieve log from this buffer to persist

    FILE* _fp;
    pid_t _pid;

     uint64_t _lst_lts;//last can't log error time(s) if value != 0, log error happened last time

    utc_timer* _timer;

    // internal function
    void create_double_linked_ring();
    bool decis_file(int year, int mon, int day);
};

void* do_thread_routine(void* args);


#define LOG_MEM_SET(mem_lmt) \
    do \
    { \
        if (mem_lmt < 90 * 1024 * 1024) \
        { \
            mem_lmt = 90 * 1024 * 1024; \
        } \
        else if (mem_lmt > 1024 * 1024 * 1024) \
        { \
            mem_lmt = 1024 * 1024 * 1024; \
        } \
        logger::_buff_size = mem_lmt; \
    } while (0)

#define LOG_INIT(log_dir, app_name, level) \
    do \
    { \
        logger::ins()->init_app(log_dir, app_name, level); \
        pthread_t tid; \
        pthread_create(&tid, NULL, do_thread_routine, NULL); \
        pthread_detach(tid); \
    } while (0)

//format: [LEVEL][yy-mm-dd h:m:s.ms][tid]file_name:line_no(func_name):content
#define LOG_TRACE(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= TRACE) \
        { \
            logger::ins()->try_append("[TRACE]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_DEBUG(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= DEBUG) \
        { \
            logger::ins()->try_append("[DEBUG]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_INFO(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= INFO) \
        { \
            logger::ins()->try_append("[INFO]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_NORMAL(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= INFO) \
        { \
            logger::ins()->try_append("[INFO]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_WARN(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= WARN) \
        { \
            logger::ins()->try_append("[WARN]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_ERROR(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= ERROR) \
        { \
            logger::ins()->try_append("[ERROR]", "[%u]%s:%d(%s): " fmt "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define LOG_FATAL(fmt, args...) \
    do \
    { \
        logger::ins()->try_append("[FATAL]", "[%u]%s:%d(%s): " fmt "\n", \
            gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    } while (0)

#define TRACE(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= TRACE) \
        { \
            logger::ins()->try_append("[TRACE]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define DEBUG(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= DEBUG) \
        { \
            logger::ins()->try_append("[DEBUG]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define INFO(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= INFO) \
        { \
            logger::ins()->try_append("[INFO]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define NORMAL(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= INFO) \
        { \
            logger::ins()->try_append("[INFO]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define WARN(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= WARN) \
        { \
            logger::ins()->try_append("[WARN]", "[%u]%s:%d(%s): " fmt "\n", \
                    gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define ERROR(fmt, args...) \
    do \
    { \
        if (logger::ins()->get_level() >= ERROR) \
        { \
            logger::ins()->try_append("[ERROR]", "[%u]%s:%d(%s): " fmt "\n", \
                gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
        } \
    } while (0)

#define FATAL(fmt, args...) \
    do \
    { \
        logger::ins()->try_append("[FATAL]", "[%u]%s:%d(%s): " fmt "\n", \
            gettid(), __FILE__, __LINE__, __FUNCTION__, ##args); \
    } while (0)

#endif