#ifndef PROJEKT_LIBRARY_H
#define PROJEKT_LIBRARY_H
#include <sys/stat.h>

int check_permissions(mode_t st_mode, int perm_mask);
void print_file_details(const char* path, struct stat* statbuf);
void hello(void);


#endif //PROJEKT_LIBRARY_H