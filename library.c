#include <sys/stat.h>

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