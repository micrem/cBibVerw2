/**
 * LinkedList
 * @file LinkedList.c
 * Implementiert eine LinkedList mit void-Pointern zu Daten
 */


#include "LinkedList.h"


//interne Hilfsfunktionen

/** interne Hilfsfunktion
 * gibt Pointer zur letzten Node der Liste 'list' aus oder NULL
 * @param list Pointer zur Liste in der gesucht wird
 * @return Pointer zur letzten Node oder NULL
 */
LLNode *getListLastNode(LinkedList *list) {
    if (list == NULL) {
        if (LLIST_DEBUG_MODE) printf("getListLastNode() hat leere Liste erhalten!\n");
        return NULL;
    }
    LLNode *runner = list->first;
    while (runner->next != NULL) {
        runner = runner->next;
    }
    return runner;
}
/** interne Hilfsfunktion
 * gibt Pointer zur Node mit Index 'index' aus Liste 'list' zurueck oder NULL
 * @param list LinkedList in der gesucht wird
 * @param index Index der Node in der Liste (beginnend mit 0)
 * @return LLNode Pointer zur indizierten Node oder NULL
 */
LLNode *getListNode(LinkedList *list, int index) {
    LLNode *runner;
    int i = 0;
    if (list == NULL || index + 1 > list->length || index < 0) {
        if (LLIST_DEBUG_MODE) printf("getListNode hat leere Liste oder ungueltigen Index erhalten!\n");
        return NULL;
    }
    runner = list->first;
    for (i = 0; i < index; i++) {
        runner = runner->next;
    }
    if (LLIST_DEBUG_MODE > 1) printf("getListNode() fand Index %d\n", i);
    return runner;
}

LLNode *newEmptyNode() {
    LLNode *new_node;
    new_node = malloc(sizeof(LLNode));
    if (new_node == NULL) {
        if (LLIST_DEBUG_MODE) printf("newEmptyNode() konnte keinen Speicher anlegen!\n");
        return NULL;
    }
    new_node->data = NULL;
    new_node->next = NULL;
    return new_node;
}


int addListItem(LinkedList *list, void *data) {
    if (LLIST_DEBUG_MODE > 1) printf("addListItem() Beginn: length: %d\n", list->length);
    LLNode *new_node;
    LLNode *old_node;

    if (data == NULL || list == NULL) {
        if (LLIST_DEBUG_MODE) printf("addListItem:(data == NULL || list == NULL) \n");
        return -1;
    }
    new_node = newEmptyNode();
    if (new_node == NULL) {
        if (LLIST_DEBUG_MODE) printf("addListItem: Fehler in newEmptyNode() \n");
        return -1;
    }
    new_node->data = data;

    if (list->first == NULL) {
        list->first = new_node;
    } else {
        old_node = getListLastNode(list);
        if (old_node) {
            old_node->next = new_node;
        } else {
            if (LLIST_DEBUG_MODE) printf("addListItem: schwerer Fehler, Liste inkonsistent\n");
            return -2;
        }
    }
    list->length += 1;
    if (LLIST_DEBUG_MODE > 1) printf("addListItem neues Element, neues length: %d\n", list->length);
    return 0;

}

int removeListItem(LinkedList *list, int index) {
    LLNode *node_to_delete, *previous, *following;
    node_to_delete = previous = following = NULL;

    if (list == NULL || index + 1 > list->length || index < 0 || list->length == 0) {
        if (LLIST_DEBUG_MODE)
            printf("removeListItem() Fehler (index + 1 > list->length || index < 0 || list == NULL || list->length == 0)\n");
        return -1;
    }
    node_to_delete = getListNode(list, index);
    previous = getListNode(list, index - 1); //liefert NULL falls index==0 (hiesse header wird geloescht)
    following = node_to_delete->next;

    //free(node_to_delete->data); //nicht verwendet, Liste legt Daten nicht an, uebernimmt nicht Verantwortung fuer Daten loeschen
    free(node_to_delete);
    if (previous) previous->next = following; //falls previous existiert (header zB hat keinen); klappt auch fuer following == NULL (beim letzten)
    if (index == 0) list->first = following; //Listen-Eintrag fuer erstes Element korrigieren falls geloescht
    list->length -= 1;
    return 0;
}

/**
 *
 * @param list
 * @param index
 * @return
 */
void *getListData(LinkedList *list, int index) {
    LLNode *node = getListNode(list, index);
    if (node == NULL) {
        if (LLIST_DEBUG_MODE) printf("getListData Fehler: leerer Parameter\n");
        return NULL;
    }
    return node->data;
}

