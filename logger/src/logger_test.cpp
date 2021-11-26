#include "logger.h"
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>

namespace logger_test{

int64_t get_current_millis(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void test()
{
    LOG_INIT("log", "myname", 3);
    uint64_t start_ts = get_current_millis();
    for (int i = 0;i < 6; ++i)
    {
        LOG_ERROR("my number is number my number is my number is my number is my number is my number is my number is %d", i);
    }
    uint64_t end_ts = get_current_millis();
    printf("time use %lums\n", end_ts - start_ts);

}
}