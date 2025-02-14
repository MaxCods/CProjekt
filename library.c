#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

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

void set_permissions(const char* perm_str, mode_t* perm_mask) {
    if (perm_str[0] == 'r') *perm_mask |= S_IRUSR;
    if (perm_str[1] == 'w') *perm_mask |= S_IWUSR;
    if (perm_str[2] == 'x') *perm_mask |= S_IXUSR;
    if (perm_str[3] == 'r') *perm_mask |= S_IRGRP;
    if (perm_str[4] == 'w') *perm_mask |= S_IWGRP;
    if (perm_str[5] == 'x') *perm_mask |= S_IXGRP;
    if (perm_str[6] == 'r') *perm_mask |= S_IROTH;
    if (perm_str[7] == 'w') *perm_mask |= S_IWOTH;
    if (perm_str[8] == 'x') *perm_mask |= S_IXOTH;
}

void print_file_details(const char* path, struct stat* statbuf) {
    // Mostrar permisos
    char perms[11];
    snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c%c",
             (S_ISDIR(statbuf->st_mode)) ? 'd' : '-',
             (statbuf->st_mode & S_IRUSR) ? 'r' : '-',
             (statbuf->st_mode & S_IWUSR) ? 'w' : '-',
             (statbuf->st_mode & S_IXUSR) ? 'x' : '-',
             (statbuf->st_mode & S_IRGRP) ? 'r' : '-',
             (statbuf->st_mode & S_IWGRP) ? 'w' : '-',
             (statbuf->st_mode & S_IXGRP) ? 'x' : '-',
             (statbuf->st_mode & S_IROTH) ? 'r' : '-',
             (statbuf->st_mode & S_IWOTH) ? 'w' : '-',
             (statbuf->st_mode & S_IXOTH) ? 'x' : '-');

    printf("%s ", perms);

    // Mostrar número de enlaces
    printf("%lu ", statbuf->st_nlink);

    // Mostrar propietario
    struct passwd *pw = getpwuid(statbuf->st_uid);
    printf("%s ", pw ? pw->pw_name : "unknown");

    // Mostrar grupo
    struct group *gr = getgrgid(statbuf->st_gid);
    printf("%s ", gr ? gr->gr_name : "unknown");

    // Mostrar tamaño
    printf("%ld ", statbuf->st_size);

    // Mostrar fecha de última modificación
    char timebuf[80];
    struct tm *tm_info = localtime(&statbuf->st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
    printf("%s ", timebuf);

    // Mostrar el nombre del archivo
    printf("%s\n", path);
}