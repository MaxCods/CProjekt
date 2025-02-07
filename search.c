// search.c
#include "search.h"

void* search_worker(void* arg) {
    ThreadContext* context = (ThreadContext*)arg;
    ThreadSafeQueue* queue = context->queue;
    SearchOptions* options = context->options;

    while (1) {
        char* path = dequeue(queue);
        if (path == NULL) break;

        process_directory(path, options, queue);
        free(path);
    }

    return NULL;
}

void process_directory(const char* dir_path, SearchOptions* options, ThreadSafeQueue* queue) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (!options->show_hidden && entry->d_name[0] == '.') continue;

        char full_path[MAX_PATH_LENGTH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (lstat(full_path, &statbuf) == -1) continue;

        //filter for name
        if (options->name_pattern == NULL || strstr(entry->d_name, options->name_pattern)) {
            //filter for type if specified
            if (options->type_filter == 'f' && !S_ISREG(statbuf.st_mode)) continue;
            if (options->type_filter == 'd' && !S_ISDIR(statbuf.st_mode)) continue;
            if (options->type_filter == 'l' && !S_ISLNK(statbuf.st_mode)) continue;

            //filter for size
            if (options->size_operator != -1) {
                int match_size = 0;
                if (options->size_operator == 0) {
                    if (statbuf.st_size == options->size_value) match_size = 1;
                } else if (options->size_operator == 1) {
                    if (statbuf.st_size > options->size_value) match_size = 1;
                } else if (options->size_operator == 2) {
                    if (statbuf.st_size < options->size_value) match_size = 1;
                }

                if (!match_size) continue;
            }
          printf("%s\n", full_path);
        }

        //search recursively
        if (S_ISDIR(statbuf.st_mode) && options->recursive) {
            if (options->use_threads) {
                enqueue(queue, full_path);
            } else {
                process_directory(full_path, options, queue);
            }
        }
    }

    closedir(dir);
}