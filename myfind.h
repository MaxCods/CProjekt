// myfind.h
#ifndef MYFIND_H
#define MYFIND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_PATH_LENGTH 4096
#define MAX_THREADS 8

typedef struct SearchOptions {
    char* name_pattern;
    int recursive;
    int show_hidden;
    int use_threads;
    char type_filter;
    int size_operator;   // 0 = exact, 1 = greater, 2 = smaller
    off_t size_value; //size of bytes
} SearchOptions;

void print_usage(void);
void parse_arguments(int argc, char* argv[], SearchOptions* options, const char** start_path);

#endif