// queue.c
#include "queue.h"

void init_queue(ThreadSafeQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
    queue->count = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    sem_init(&queue->items, 0, 0);
}

void enqueue(ThreadSafeQueue* queue, const char* path) {
    QueueNode* new_node = malloc(sizeof(QueueNode));
    new_node->path = strdup(path);
    new_node->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->rear == NULL) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->count++;

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->items);
}

char* dequeue(ThreadSafeQueue* queue) {
    sem_wait(&queue->items);
    pthread_mutex_lock(&queue->mutex);

    if (queue->front == NULL) {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }

    QueueNode* temp = queue->front;
    char* path = temp->path;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    queue->count--;

    free(temp);
    pthread_mutex_unlock(&queue->mutex);
    return path;
}

void cleanup_queue(ThreadSafeQueue* queue) {
    pthread_mutex_destroy(&queue->mutex);
    sem_destroy(&queue->items);

    while (queue->front != NULL) {
        QueueNode* temp = queue->front;
        queue->front = queue->front->next;
        free(temp->path);
        free(temp);
    }
}