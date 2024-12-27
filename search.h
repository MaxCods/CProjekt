// search.h
#ifndef SEARCH_H
#define SEARCH_H

#include "myfind.h"
#include "queue.h"

typedef struct ThreadContext {
    ThreadSafeQueue* queue;
    SearchOptions* options;
} ThreadContext;

void* search_worker(void* arg);
void process_directory(const char* dir_path, SearchOptions* options, ThreadSafeQueue* queue);

#endif