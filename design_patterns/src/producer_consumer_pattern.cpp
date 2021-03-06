#include "producer_consumer_pattern.h"

#define CONSUMERS_CNT 2
#define PRODUCERS_CNT 1

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

pthread_t g_thread[CONSUMERS_CNT + PRODUCERS_CNT];

int shared_var = 0;

namespace pc {

void* consumer(void* arg) {
    int* num = (int*)arg;
    while (1) {
        // lock
        pthread_mutex_lock(&g_mutex);

        while (shared_var == 0) {
            std::cout << "consumer " << *num << " begin wait a condition...\n";
            pthread_cond_wait(&g_cond, &g_mutex); // block here util producer notify
        }

        std::cout << "consumer " << *num << " end wait a condition...\n";
        std::cout << "consumer " << *num << " begin consume product...\n";


        shared_var--;

        // unlock
        pthread_mutex_unlock(&g_mutex);
        sleep(1);    
    }

    return nullptr;
}

void* producer(void* arg) {
    int* num = (int*)arg;

    while (1) {

        pthread_mutex_lock(&g_mutex);

        std::cout << "producer " << *num << " begin product...\n";
        shared_var++;
        std::cout << "producer " << *num << " end product...\n";

        pthread_cond_signal(&g_cond);
        std::cout << "producer " << *num << " notify consumer by cond var...\n";

        pthread_mutex_unlock(&g_mutex);
        sleep(2);

    }

    return nullptr;
}

void test() {

    pthread_mutex_init(&g_mutex, nullptr);
    pthread_cond_init(&g_cond, nullptr);

    int pthread_index[] = {1,2,3};

    for (int i = 0; i < CONSUMERS_CNT; i++) {
        pthread_create(&g_thread[i], nullptr, consumer, (void*)&pthread_index[i]);
        sleep(1);
    }

    sleep(2);

    for (int i = CONSUMERS_CNT; i < (PRODUCERS_CNT + CONSUMERS_CNT); i++) {
        pthread_create(&g_thread[i], nullptr, producer, (void*)&pthread_index[i]);
        sleep(1);
    }

    for (int i = 0; i < (CONSUMERS_CNT + PRODUCERS_CNT); i++) {
        pthread_join(g_thread[i], nullptr);
    }

    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_cond);


}
}