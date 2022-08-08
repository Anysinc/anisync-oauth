#include "timer.h"
#include "time.h"

void* timeit(void* data) {
    struct timer* timer = (struct timer*) data;
    while (timer->init) {
        while (!timer->running && timer->init);
        if (!timer->init) break;
        time_sleep(timer->ms);
        timer->running = false;
        timer->callback(timer->data);
    }
}

void timer_init(struct timer* timer) {
    timer->running = false;
    timer->init = true;
    thread_init(&timer->th);
    thread_start(&timer->th, timeit, timer);
}

void timer_start(struct timer* timer, int ms, void* (*callback) (void*), void* data) {
    timer->ms = ms;
    timer->callback = callback;
    timer->data = data;
    timer->running = true;
}

void timer_term(struct timer* timer) {
    timer->init = false;
    thread_term(&timer->th);
}