#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "library.h"

void test_set_permissions_and_check_permissions() {
    mode_t perm_mask = 0;
    // Test mit vollständigen Berechtigungen "rwxrwxrwx"
    const char* perms = "rwxrwxrwx";
    set_permissions(perms, &perm_mask);
    
    // Erstelle ein st_mode, das alle in perm_mask geforderten Bits besitzt
    mode_t full_mode = S_IRUSR | S_IWUSR | S_IXUSR |   //rwx jeweils 
                       S_IRGRP | S_IWGRP | S_IXGRP |
                       S_IROTH | S_IWOTH | S_IXOTH;
    
    int result = check_permissions(full_mode, perm_mask);
    assert(result == 1 && "Test: Volle Berechtigungen sollten übereinstimmen");

    // Test: Fehlende Lese-Berechtigung für den User
    full_mode = S_IWUSR | S_IXUSR |
               S_IRGRP | S_IWGRP | S_IXGRP |
               S_IROTH | S_IWOTH | S_IXOTH;
    result = check_permissions(full_mode, perm_mask);
    assert(result == 0 && "Test: Fehlende Benutzerleseberechtigung muss zum Fehlschlag führen");
}


void test_myfind_application() {
    printf("Test myfind application: ");
    int ret = system("./myfind . test > /dev/null 2>&1");
    if(ret == -1) {
        fprintf(stderr, "Fehler: system() konnte den Befehl nicht ausführen.\n");
        exit(EXIT_FAILURE);
    }
    int exit_status = WEXITSTATUS(ret);
    assert(exit_status == 0 && "myfind sollte mit exit code 0 enden");
    printf("OK\n");
}

int main() {
    printf("Tests starten...\n");
    test_set_permissions_and_check_permissions();
    test_myfind_application();
    printf("Alle Tests bestanden.\n");
    return 0;
} 