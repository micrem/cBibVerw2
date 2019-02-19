/**
 * @file LinkedList.c
 * Implementiert eine LinkedList mit Zugriffsfunktionen
 */


#include "LinkedList.h"

//### interne Hilfsfunktionen:

/**
 * legt neue leere, initialisierte LLNode mit malloc() an und gibt einen Pointer darauf zurueck
 * @return Pointer auf neue LLNode, oder NULL bei Fehler
 */
LLNode *newEmptyNode() {
    LLNode *new_node=NULL;
    new_node = malloc(sizeof(LLNode));
    if (new_node == NULL) {
        if (LLIST_DEBUG_MODE) printf("newEmptyNode() konnte keinen Speicher anlegen!\n");
        return NULL;
    }
    new_node->data = NULL;
    new_node->next = NULL;
    return new_node;
}

/**
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
    while (runner->next != NULL) { //geht iterativ durch alle nodes bis zur Letzten
        runner = runner->next;
    }
    return runner;
}
//### Ende interne Hilfsfunktionen


//### im LinkedList-Header definierte, oeffentlich verfuegbare Funktionen

/**
 * gibt Pointer zur Node mit Index 'index' aus Liste 'list' zurueck oder NULL
 * @param list LinkedList in der gesucht wird
 * @param index Index der Node in der Liste (beginnend mit 0)
 * @return Pointer zur indizierten Node oder NULL
 */
LLNode *getListNode(LinkedList *list, int index) {
    LLNode *runner=NULL;
    int i = 0;
    //Fehlerchecks: leerer list-pointer, zu grosser/kleiner Index
    if (list == NULL || index + 1 > list->length || index < 0) {
        if (LLIST_DEBUG_MODE>1) printf("getListNode hat leere Liste oder ungueltigen Index erhalten!\n");
        return NULL;
    }
    runner = list->first;
    for (i = 0; i < index; i++) {
        runner = runner->next;
    }
    if (LLIST_DEBUG_MODE > 1) printf("getListNode() fand Index %d\n", i);
    return runner;
}


/**
 * fuegt neue Node an Liste 'list' an, mit Pointer auf data vom typ 'void'
 * @param list liste an die die Daten angefuegt werden
 * @param data void-Pointer auf Datensatz der in die LinkedList list aufgenommen werden soll
 * Bei festgestellten schweren Problemen in der Listenstruktur wird Programm beendet!
 * @return gibt 0 bei Erfolg oder Fehlerwert(!=0) wieder
 */
int addListItem(LinkedList *list, void *data) {
    if (LLIST_DEBUG_MODE > 1) printf("addListItem() Beginn: length: %d\n", list->length);
    LLNode *new_node;
    LLNode *old_node;
    //Fehlerchecks: leere Pointer fuer Liste oder Daten
    if (data == NULL || list == NULL) {
        if (LLIST_DEBUG_MODE) printf("addListItem:(data == NULL || list == NULL) \n");
        return -1;
    }
    //neue Node anlegen
    new_node = newEmptyNode();
    if (new_node == NULL) { //entspricht malloc-Fehler
        if (LLIST_DEBUG_MODE) printf("addListItem: Fehler in newEmptyNode() \n");
        return -1;
    }
    //Daten zuweisen
    new_node->data = data;

    if (list->first == NULL) {
        list->first = new_node;//falls Liste vorher keine Elemente hatte
    } else {
        old_node = getListLastNode(list);
        if (old_node) {
            old_node->next = new_node;
        } else {
            //sollte nicht vorkommen, da bereits auf Existenz eines Elements geprueft wurde
            //deutet auf fehlerhaft verlinkte Listenelemente hin
            if (LLIST_DEBUG_MODE) printf("addListItem: schwerer Fehler, Liste inkonsistent\n");
            printf("Schwerwiegender Fehler, interne Speicherstruktur inkonsistent, Programm wird beendet!\n(weiter mit Enter)\n");
            getc(stdin);
            exit(-1);
        }
    }
    list->length += 1;
    if (LLIST_DEBUG_MODE > 1) printf("addListItem neues Element, neues length: %d\n", list->length);
    return 0;

}

/**
 * entfernt Element vom Index index aus LinkedList list
 * @param list Pointer auf LinkedList aus der ein Element entfernt wird
 * @param index Index des zu loeschenden Elements
 * @return gibt 0 bei Erfolg oder Fehlerwert(!=0) wieder
 */
int removeListNode(LinkedList *list, int index) {
    LLNode *node_to_delete, *previous, *following;
    node_to_delete = previous = following = NULL;
    //Fehlerchecks: leerer list-Pointer, index 'out-of-bounds', Liste ohne Elemente
    if (list == NULL || index + 1 > list->length || index < 0 || list->length == 0) {
        if (LLIST_DEBUG_MODE)
            printf("removeListNode() Fehler (index + 1 > list->length || index < 0 || list == NULL || list->length == 0)\n");
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
 * @param list
 * gibt void-Pointer auf Daten des Listen-Elements mit Index index aus LinkedList list zurueck
 * @param index
 * @return void-Pointer auf Daten des gefundenen Elements oder NULL-Pointer
 */
void *getListData(LinkedList *list, int index) {
    LLNode *node = getListNode(list, index); //Uebergabefehler werden in getListNode abgefangen
    if (node == NULL) {
        if (LLIST_DEBUG_MODE) printf("getListData Fehler: falsche Zugriffsparameter\n");
        return NULL;
    }
    return node->data;
}

//### Ende oeffentlich verfuegbare Funktionen
