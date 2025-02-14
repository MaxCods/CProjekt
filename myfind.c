// myfind.c
#include "myfind.h"
#include "queue.h"
#include "search.h"
#include "library.h"

void print_usage(void) {
    fprintf(stderr, "Usage: myfind [options] [path...] [expression]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -name PATTERN    search for files matching PATTERN\n");
    fprintf(stderr, "  -R, -r          search recursively\n");
    fprintf(stderr, "  -a              show hidden files\n");
    fprintf(stderr, "  -t              use multiple threads\n");
    fprintf(stderr, "  -type           filter by file type (f for file, d for dir and l for symbolic links)\n");
    fprintf(stderr, "  -size SIZE      filter by file size (N for exact size, +N for greater than, -N for less than)\n");
    fprintf(stderr, "  -mtime DAYS     filter by last modification time (N for exact, +N for older, -N for newer)\n");
    fprintf(stderr, "  -perm MODE      filter by file permissions (e.g., rwxrwxrwx)\n");
    fprintf(stderr, "  -l              show detailed information about the matching files (permissions, owner, size, etc.)\n");
    fprintf(stderr, "  -h              display this help message\n");
}



void parse_arguments(int argc, char* argv[], SearchOptions* options, const char** start_path) {
    options->name_pattern = NULL;
    options->recursive = 0;
    options->show_hidden = 0;
    options->use_threads = 0;
    options->type_filter = '\0';
    options->size_operator = -1; // -1 = don't used as default value
    options->size_value = -1;
    options->mtime_operator = -1;
    options->mtime_value = -1;
    options->perm_mask = 0;  // Initialize permission mask to 0
    options->show_details = 0;

    *start_path = "."; // current path as default

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            options->name_pattern = argv[++i];
        } else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "-r") == 0) {
            options->recursive = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            options->show_hidden = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            options->use_threads = 1;
        } else if (strcmp(argv[i], "-type") == 0 && i + 1 < argc) {
            options->type_filter = argv[++i][0];
        } else if (strcmp(argv[i], "-size") == 0 && i + 1 < argc) {
            char* size_str = argv[++i];
            if (size_str[0] == '+') {
                options->size_operator = 1;
                options->size_value = atoi(size_str + 1);
            } else if (size_str[0] == '-') {
                options->size_operator = 2;
                options->size_value = atoi(size_str + 1);
            } else {
                options->size_operator = 0;
                options->size_value = atoi(size_str);
            }
        } else if (strcmp(argv[i], "-mtime") == 0 && i + 1 < argc) {
            char* mtime_str = argv[++i];
            if (mtime_str[0] == '+') {
                options->mtime_operator = 1;
                options->mtime_value = atoi(mtime_str + 1);
            } else if (mtime_str[0] == '-') {
                options->mtime_operator = 2;
                options->mtime_value = atoi(mtime_str + 1);
            } else {
                options->mtime_operator = 0;
                options->mtime_value = atoi(mtime_str);
            }
        } else if (strcmp(argv[i], "-perm") == 0 && i + 1 < argc) {
            // Parse the permission filter
            char* perm_str = argv[++i];
            options->perm_mask = 0;

            // Check for the '/' operator (at least these permissions)
            if (perm_str[0] == '/') {
                // Skip the '/' and treat the rest as permissions to match
                perm_str++;
                set_permissions(perm_str, &options->perm_mask);
            } else {
                // If there's no '/', treat it as an exact permission match
                set_permissions(perm_str, &options->perm_mask);
            }
        } else if (strcmp(argv[i], "-l") == 0) {
            options->show_details = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage();
            exit(0);
        } else if (argv[i][0] != '-')
        {
            //if not an option, pass a path to a directory
            *start_path = argv[i];
        }
    }
}

int main(int argc, char* argv[]) {
    SearchOptions options;
    const char* start_path = ".";
    parse_arguments(argc, argv, &options, &start_path);

    ThreadSafeQueue queue;
    init_queue(&queue);

    ThreadContext context = {
        .queue = &queue,
        .options = &options
    };

    if (options.use_threads) {
        pthread_t threads[MAX_THREADS];
        enqueue(&queue, start_path);

        for (int i = 0; i < MAX_THREADS; i++) {
            pthread_create(&threads[i], NULL, search_worker, &context);
        }

        for (int i = 0; i < MAX_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }
    } else {
        process_directory(start_path, &options, &queue);
    }

    cleanup_queue(&queue);
    return 0;
}