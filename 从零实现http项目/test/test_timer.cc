#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <atomic>

// 每个线程的数据结构
typedef struct ThreadData {
    timer_t timer_id;
    std::atomic<bool> should_exit;
} ThreadData;

void *thread_func(void *arg) {
    ThreadData *data = (ThreadData *)arg;

    struct sigevent sev;
    struct itimerspec its;
    struct sigaction sa;

    // 初始化信号事件
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify = SIGEV_THREAD_ID;
    sev._sigev_un._tid = pthread_self();
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &data->timer_id;

    // 创建定时器
    if (timer_create(CLOCK_REALTIME, &sev, &data->timer_id) == -1) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }

    // 设置定时器
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = 60; // 定时器过期时间为60秒
    its.it_interval.tv_sec = 0; // 不重复触发

    if (timer_settime(data->timer_id, 0, &its, NULL) == -1) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }

    // 注册信号处理函数
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = [](int signum, siginfo_t *info, void *context) {
        printf("Timer expired in thread %lu\n", (unsigned long)pthread_self());
        data->should_exit.store(true, std::memory_order_release);
    };
    sa.sa_flags = SA_SIGINFO;
    if (sigaction(SIGRTMIN, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // 主循环
    while (!data->should_exit.load(std::memory_order_acquire)) {
        // 线程的工作
        usleep(100000); // 休眠100ms
    }

    // 清理定时器
    if (timer_delete(data->timer_id) == -1) {
        perror("timer_delete");
        exit(EXIT_FAILURE);
    }

    free(data); // 释放线程数据结构
    return NULL;
}

int main() {
    int num_threads = 5; // 假设有5个线程
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        ThreadData *data = new ThreadData;
        data->should_exit = false;

        if (pthread_create(&threads[i], NULL, thread_func, (void *)data) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // 等待所有线程结束
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}