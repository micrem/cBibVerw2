#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>


// Debugmodus
// 0 = keine Debug-Nachrichten
// 1 = normal
// 2 = verbos
#define LLIST_DEBUG_MODE 1

typedef struct LLNodeS LLNode;
typedef struct LinkedListS LinkedList;

struct LLNodeS {
    void* data ;
    LLNode* next ;
} ;

struct LinkedListS {
    LLNode* first ;
    int length ;
} ;

//const struct LinkedList InitLinkedList = { NULL, 0};

int addListItem(LinkedList* list, void* data);
int removeListItem(LinkedList* list, int index);
void* getListData(LinkedList* list, int index);


#endif // LINKEDLIST_H_INCLUDED
