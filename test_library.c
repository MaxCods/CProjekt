#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include "library.h"

void test_set_permissions_and_check_permissions() {
    mode_t perm_mask = 0;
    // Test mit vollständigen Berechtigungen "rwxrwxrwx"
    const char* perms = "rwxrwxrwx";
    set_permissions(perms, &perm_mask);
    
    // Erstelle ein st_mode, das alle in perm_mask geforderten Bits besitzt
    mode_t full_mode = S_IRUSR | S_IWUSR | S_IXUSR |
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

void test_hello_function() {
    // Einfache Ausgabe, die manuell verifiziert werden kann
    printf("Test hello function: ");
    hello();
    printf("\n");
}

int main() {
    printf("Tests starten...\n");
    test_set_permissions_and_check_permissions();
    test_hello_function();
    printf("Alle Tests bestanden.\n");
    return 0;
} 