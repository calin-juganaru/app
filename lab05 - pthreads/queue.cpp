#include <iostream>

#include <pthread.h>
#include <errno.h>
#include <malloc.h>

#include "queue.h"

using namespace std;

queue_struct thr_queue;
pthread_mutex_t queue_init_mtx = PTHREAD_MUTEX_INITIALIZER;

int queue_init(queue_struct* queue, int max_len)
{
    if (!queue) return EINVAL;
    pthread_mutex_lock(&queue_init_mtx);

    if (queue->valid == QUEUE_VALID)
    {
        pthread_mutex_unlock(&queue_init_mtx);
        return EBUSY;
    }

    pthread_mutex_init(&queue->qlock, nullptr);
    pthread_cond_init(&queue->boss_cv, nullptr);
    pthread_cond_init(&queue->worker_cv, nullptr);
    queue->qexit = 0;
    queue->boss_waiting = 0;
    queue->worker_waiting = 0;
    queue->queue_len = 0;
    queue->max_queue_len = max_len;
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->valid = QUEUE_VALID;
    pthread_mutex_unlock(&queue_init_mtx);

    return 0;
}

int enqueue(queue_struct* queue, q_work_struct* elem)
{
    if (!queue || (queue->valid != QUEUE_VALID)) return EINVAL;

    pthread_mutex_lock(&queue->qlock);

    while (queue->queue_len == queue->max_queue_len)
    {
        queue->boss_waiting++;
        cout << "Total bosses waiting: " << queue->boss_waiting << endl;
        pthread_cond_wait(&queue->boss_cv, &queue->qlock);
        queue->boss_waiting--;
    }

    elem->next = nullptr;
    if (!queue->queue_len)
    {
        queue->head = queue->tail = elem;
    }
    else
    {
        queue->tail->next = elem;
        queue->tail = elem;
    }
    ++queue->queue_len;

    if (queue->worker_waiting > 0)
        pthread_cond_signal(&queue->worker_cv);

    cout << "New task in queue ---> " << queue->queue_len << endl;

    pthread_mutex_unlock(&queue->qlock);
    return 0;
}

q_work_struct* dequeue(queue_struct* queue)
{
    q_work_struct* ptr;
    if (!queue || (queue->valid != QUEUE_VALID)) return nullptr;

    pthread_mutex_lock(&queue->qlock);

    while (queue->queue_len == 0)
    {
        if (queue->qexit)
        {
            pthread_mutex_unlock(&queue->qlock);
            pthread_exit(nullptr);
        }

        ++queue->worker_waiting;
        cout << "Total workers waiting " << queue->worker_waiting << endl;
        pthread_cond_wait(&queue->worker_cv, &queue->qlock);
        --queue->worker_waiting;
    }

    ptr = queue->head;
    queue->head = ptr->next;
    queue->queue_len--;
    if (!queue->queue_len)
        queue->tail = nullptr;

    if (queue->boss_waiting > 0)
        pthread_cond_signal(&queue->boss_cv);

    cout << "Removed task from queue ---> " << queue->queue_len << endl;
    pthread_mutex_unlock(&queue->qlock);
    return ptr;
}
