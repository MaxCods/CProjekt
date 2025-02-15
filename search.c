// search.c
#include "search.h"
#include "library.h"
#include <fnmatch.h>

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

void print_results(const char* result, const SearchOptions* options) {
    printf("%s\n", result);

    if (options->output_file) {
        FILE* file = fopen(options->output_file, "a");
        if (file) {
            fprintf(file, "%s\n", result);
            fclose(file);
        } else {
            perror("error while opening the file");
        }
    } else{
        printf("%s\n", result);
    }
}
void process_directory(const char* dir_path, SearchOptions* options, ThreadSafeQueue* queue) {
    DIR* dir = opendir(dir_path);
    if (!dir) return;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip the current and parent directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        // Skip hidden files unless the option to show hidden files is enabled
        if (!options->show_hidden && entry->d_name[0] == '.') continue;

        char full_path[MAX_PATH_LENGTH];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat statbuf;
        if (lstat(full_path, &statbuf) == -1) continue;

        // filter for name
        if (options->name_pattern == NULL || fnmatch(options->name_pattern, entry->d_name, 0) == 0) {
            // filter for type if specified
            if (options->type_filter == 'f' && !S_ISREG(statbuf.st_mode)) continue;
            if (options->type_filter == 'd' && !S_ISDIR(statbuf.st_mode)) continue;
            if (options->type_filter == 'l' && !S_ISLNK(statbuf.st_mode)) continue;

            // filter for size
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

            // filter for time
            if (options->mtime_value != -1) {
                time_t now = time(NULL);
                int days_old = (now - statbuf.st_mtime) / (60 * 60 * 24); // convert seconds to days

                int match_mtime = 0;
                if (options->mtime_operator == 0) {
                    if (days_old == options->mtime_value) match_mtime = 1; // exact n days
                } else if (options->mtime_operator == 1) {
                    if (days_old > options->mtime_value) match_mtime = 1; // more than n days
                } else if (options->mtime_operator == 2) {
                    if (days_old < options->mtime_value) match_mtime = 1; // less than n days
                }

                if (!match_mtime) continue;
            }

            // Filter by permissions if specified
            if (options->perm_mask != 0 && !check_permissions(statbuf.st_mode, options->perm_mask)) continue;

            // If -l option is enabled, show detailed information
            if (options->show_details) {
                print_file_details(full_path, &statbuf);
            }

            // If all filters passed, print the file path
            print_results(full_path, options);
        }

        // Search recursively if it's a directory
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