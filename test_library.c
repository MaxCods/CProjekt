#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include "library.h"
#include "myfind.h"
#include "queue.h"
#include "search.h"

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

void test_queue_functions() {
    // Erstelle Queue und teste deren Funktionalität
    Queue* q = createQueue();
    assert(isQueueEmpty(q) && "Queue sollte initial leer sein");
    enqueue(q, 42);
    enqueue(q, 84);
    int val = dequeue(q);
    assert(val == 42 && "Erstes Element sollte 42 sein");
    val = dequeue(q);
    assert(val == 84 && "Zweites Element sollte 84 sein");
    assert(isQueueEmpty(q) && "Queue sollte nach dem Entfernen aller Elemente leer sein");
    destroyQueue(q);
}

void test_myfind_and_search_functions() {
    // Teste myfind-Funktion: Erwartet wird eine nicht-negative Rückgabe
    int count = myfind(".", "test");
    assert(count >= 0 && "myfind sollte eine nicht-negative Anzahl zurückgeben");

    // Teste search_directory-Funktion: Erwartet wird eine nicht-negative Rückgabe
    int search_count = search_directory(".", "test");
    assert(search_count >= 0 && "search_directory sollte eine nicht-negative Anzahl zurückgeben");
}

int main() {
    printf("Tests starten...\n");
    test_set_permissions_and_check_permissions();
    test_hello_function();
    test_queue_functions();
    test_myfind_and_search_functions();
    printf("Alle Tests bestanden.\n");
    return 0;
} 