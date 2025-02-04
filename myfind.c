// myfind.c
#include "myfind.h"
#include "queue.h"
#include "search.h"

void print_usage(void) {
    fprintf(stderr, "Usage: myfind [options] [path...] [expression]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -name PATTERN    search for files matching PATTERN\n");
    fprintf(stderr, "  -R, -r          search recursively\n");
    fprintf(stderr, "  -a              show hidden files\n");
    fprintf(stderr, "  -t              use multiple threads\n");
    fprintf(stderr, "  -h              display this help message\n");
}

void parse_arguments(int argc, char* argv[], SearchOptions* options) {
    options->name_pattern = NULL;
    options->recursive = 0;
    options->show_hidden = 0;
    options->use_threads = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            options->name_pattern = argv[++i];
        } else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "-r") == 0) {
            options->recursive = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            options->show_hidden = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            options->use_threads = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage();
            exit(0);
        }
    }
}

int main(int argc, char* argv[]) {
    SearchOptions options;
    parse_arguments(argc, argv, &options);

    ThreadSafeQueue queue;
    init_queue(&queue);

    ThreadContext context = {
        .queue = &queue,
        .options = &options
    };

    const char* start_path = ".";
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