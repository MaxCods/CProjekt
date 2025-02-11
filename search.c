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

// Function to check if the permissions match the given mask
int check_permissions(mode_t st_mode, int perm_mask) {
    // Check for user permissions (S_IRUSR, S_IWUSR, S_IXUSR)
    if ((perm_mask & S_IRUSR) && !(st_mode & S_IRUSR)) return 0;
    if ((perm_mask & S_IWUSR) && !(st_mode & S_IWUSR)) return 0;
    if ((perm_mask & S_IXUSR) && !(st_mode & S_IXUSR)) return 0;

    // Check for group permissions (S_IRGRP, S_IWGRP, S_IXGRP)
    if ((perm_mask & S_IRGRP) && !(st_mode & S_IRGRP)) return 0;
    if ((perm_mask & S_IWGRP) && !(st_mode & S_IWGRP)) return 0;
    if ((perm_mask & S_IXGRP) && !(st_mode & S_IXGRP)) return 0;

    // Check for other permissions (S_IROTH, S_IWOTH, S_IXOTH)
    if ((perm_mask & S_IROTH) && !(st_mode & S_IROTH)) return 0;
    if ((perm_mask & S_IWOTH) && !(st_mode & S_IWOTH)) return 0;
    if ((perm_mask & S_IXOTH) && !(st_mode & S_IXOTH)) return 0;

    return 1; // All checks passed, return 1 (match)
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
            //filter for time
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

          // if none match
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