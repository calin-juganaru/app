#include <iostream>

#include <pthread.h>
#include <errno.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "queue.h"

using namespace std;

extern queue_struct thr_queue;
int active_workers;
pthread_mutex_t mutex_active_workers;

int need_increase, need_decrease;
int init_num_workers = 4;
int total_workers = 4;

constexpr auto MAX_THREADS = 16;
pthread_t threads[MAX_THREADS];
pthread_attr_t attr;

// ============================================================================

void check_error(int code, string generator) {}

int get_work(q_work_struct* w) { return rand() + 1; }

// ============================================================================

void* worker(void* args)
{
    while (true)
    {
        auto ptr = dequeue(&thr_queue);

        cout << "New task for worker " << gettid() << endl;

        pthread_mutex_lock(&mutex_active_workers);
        ++active_workers;
        pthread_mutex_unlock(&mutex_active_workers);

        if (!ptr)
        {
            cout << "dequeue() error\n";
            exit(-1);
        }

        sleep(5);
        pthread_mutex_lock(&mutex_active_workers);

        int work_load = active_workers * 100 / (total_workers - 1);
        cout << "Work load = " << work_load << "%\n";

        if (work_load > 80)
        {
            need_decrease = 1;
            need_increase = 0;
        }
        else if (work_load < 20)
        {
            need_decrease = 0;
            need_increase = 1;
        }
        else
        {
            need_decrease = 0;
            need_increase = 0;
        }

        --active_workers;
        pthread_mutex_unlock(&mutex_active_workers);

        cout << "Task finished by worker " << gettid() << endl;

        free(ptr);
    }
}

// ============================================================================

void* boss(void* args)
{
    for (auto i = 0; i < init_num_workers * 10; ++i)
    {
        auto ptr = (q_work_struct*)malloc(sizeof(q_work_struct));
        if (!ptr) break;

        auto ret = get_work(ptr);
        if (!ret) { free(ptr); break; }

        auto work_load = active_workers * 100 / (total_workers - 1);
        cout << "Work load = " << work_load << "%\n";

        if (work_load > 80)
        {
            need_decrease = 0;
            need_increase = 1;
        }
        else if (work_load < 20)
        {
            need_decrease = 1;
            need_increase = 0;
        }
        else
        {
            need_decrease = 0;
            need_increase = 0;
        }

        if (enqueue(&thr_queue, ptr))
        {
            cout << "enqueue() error\n";
            exit(-1);
        }

        int old_total = total_workers;
        if (need_increase)
        {
            if (total_workers + (init_num_workers / 2) < MAX_THREADS)
                total_workers += init_num_workers / 2;

            cout << "Total workers: " << total_workers << endl;

            for (auto i = old_total; (i < total_workers) && (i < MAX_THREADS); ++i)
            {
                auto ret = pthread_create(&threads[i], &attr, worker, nullptr);
                check_error(ret, "pthread_create()");
            }
        }
    }

    return nullptr;
}

// ============================================================================

int queue_terminate_workers(queue_struct* queue)
{
    if (!queue || (queue->valid != QUEUE_VALID)) return EINVAL;

    pthread_mutex_lock(&queue->qlock);
    queue->qexit = 1;

    pthread_cond_broadcast(&queue->worker_cv);
    pthread_mutex_unlock(&queue->qlock);

    return 0;
}

void queue_wait_workers(queue_struct* queue, int nthreads)
{
    if (!queue || (queue->valid != QUEUE_VALID)) return;

    pthread_mutex_lock(&queue->qlock);
    while (queue->worker_waiting != nthreads)
    {
        ++queue->boss_waiting;
        pthread_cond_wait(&queue->boss_cv, &queue->qlock);
        --queue->boss_waiting;
    }
    pthread_mutex_unlock(&queue->qlock);
}

void process_tasks()
{
    auto nthreads = total_workers;
    auto max_queue_len = init_num_workers * 10;

    if (queue_init(&thr_queue, max_queue_len))
    {
        cout << "queue_init() error\n";
        exit(-1);
    }

    pthread_attr_init(&attr);

    for (auto i = 1; (i < nthreads) && (i < MAX_THREADS); ++i)
    {
        auto ret = pthread_create(&threads[i], &attr, worker, nullptr);
        check_error(ret, "pthread_create()");
    }
    pthread_attr_destroy(&attr);

    boss(nullptr);

    if (queue_terminate_workers(&thr_queue))
    {
        cout << "dequeue() error\n";
        exit(-1);
    }

    for (auto i = 1; (i < total_workers) && (i < MAX_THREADS); ++i)
    {
        auto ret = pthread_join(threads[i], nullptr);
        check_error(ret, "pthread_join()");
    }
}

// ============================================================================

int main()
{
    pthread_mutex_init(&mutex_active_workers, nullptr);
    need_increase = 0;
    need_decrease = 0;
    process_tasks();
}
