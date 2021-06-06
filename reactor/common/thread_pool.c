/*************************************************************************
	> File Name: 3.thread_pool.c
	> Author: wushanning 
	> Mail: 954726270@qq.com 
	> Created Time: Wed Apr 21 10:55:44 2021
 ************************************************************************/

#include "head.h"

void task_queue_init(struct task_queue *taskQueue, int size) {
    taskQueue->size = size;
    taskQueue->total = taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
    taskQueue->data = calloc(size + 5, sizeof(void *));
    return ;
}

void task_queue_push(struct task_queue *taskQueue, char *str) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) {
        printf("taskQueue is full!\n");
        pthread_mutex_unlock(&taskQueue->mutex);
        return ;
    }
    printf("<Push> : %s\n", str);
    taskQueue->data[taskQueue->tail] = str;
    ++taskQueue->total;
    if (++taskQueue->tail == taskQueue->size) {
        printf("taskQueue reached end!\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
    return ;
}

char *task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == 0) {
        printf("taskQueue is empty!\n");
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    char *str = taskQueue->data[taskQueue->head];
    printf("<Pop> : %s\n", str);
    --taskQueue->total;
    if (++taskQueue->head == taskQueue->size) {
        printf("taskQueue head reached end!\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return str;   
}
