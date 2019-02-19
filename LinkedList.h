/**
 * @file LinkedList.h Header fuer LinkedList Implementierung
 * definiert benoetigte Structs und Art des Debug-logging (0-2) speziell fuer LinkedList Fehlermeldungen
 * definiert Zugriff-funktionen sowie Funktionen zum hinzufuegen und loeschen uber Index
 * fuer Pseudo-Kapselung
 *
 */

#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>


// Debugmodus
// 0 = keine Debug-Nachrichten
// 1 = normal
// 2 = verbos
#define LLIST_DEBUG_MODE 0

typedef struct LLNodeS LLNode;
typedef struct LinkedListS LinkedList;

/**
 * @brief Node-Struct
 * verlinkt Daten-Payload mit void-Pointer data
 * und naechste Node mit LLNode-Pointer next
 */
struct LLNodeS {
    void* data ;
    LLNode* next ;
} ;

/**
 * @brief Struct welches die Liste selbst repraesentiert
 * verlinkt mit first auf erste Node (NULL bei leerer Liste)
 * Anzahl der Elemente length wird von Zugriffsfunktionen verwaltet
 */
struct LinkedListS {
    LLNode* first ;
    int length ;
} ;

int addListItem(LinkedList* list, void* data);
int removeListNode(LinkedList *list, int index);
void* getListData(LinkedList* list, int index);
LLNode *getListNode(LinkedList *list, int index);

#endif // LINKEDLIST_H_INCLUDED
