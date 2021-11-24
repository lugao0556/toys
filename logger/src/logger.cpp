#include "logger.h"

#include <errno.h>
#include <unistd.h>//access, getpid
#include <assert.h>//assert
#include <stdarg.h>//va_list
#include <sys/stat.h>//mkdir
#include <sys/syscall.h>//system call

logger* logger::_ins = nullptr;
pthread_once_t logger::_once = PTHREAD_ONCE_INIT;


logger::logger() {}