#ifndef PROJEKT_LIBRARY_H
#define PROJEKT_LIBRARY_H
#include <sys/stat.h>

int check_permissions(mode_t st_mode, int perm_mask);
void print_file_details(const char* path, struct stat* statbuf);
void set_permissions(const char* perm_str, mode_t* perm_mask);
void hello(void);


#endif //PROJEKT_LIBRARY_H