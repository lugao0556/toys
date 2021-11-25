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

pid_t gettid() {
    return syscall(__NR_gettid);
}

logger::logger() :
    _buff_num(BUF_NUM),
    _curr_buf(nullptr),
    _persist_buf(nullptr),
    _fp(nullptr),
    _log_size(0),
    _env_ok(false),
    _level(INFO),
    _timer() {

    create_double_linked_ring();
    _pid = getpid();
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

        }



    }

}

// this api is used as args for function pthread
#define BUFF_WAIT_TIME 1
void* do_by_thread(void* args) {
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
