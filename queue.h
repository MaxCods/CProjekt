// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "myfind.h"

typedef struct QueueNode {
    char* path;
    struct QueueNode* next;
} QueueNode;

typedef struct ThreadSafeQueue {
    QueueNode* front;
    QueueNode* rear;
    pthread_mutex_t mutex;
    sem_t items;
    int count;
} ThreadSafeQueue;

void init_queue(ThreadSafeQueue* queue);
void enqueue(ThreadSafeQueue* queue, const char* path);
char* dequeue(ThreadSafeQueue* queue);
void cleanup_queue(ThreadSafeQueue* queue);

#endif