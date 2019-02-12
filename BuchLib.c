#include "BuchLib.h"


Buch *newEmptyBuch() {
    Buch *newBuch = malloc(sizeof(Buch));
    if (newBuch == NULL) {
        if (DEBUG_MODE)printf("newEmptyBuch() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    strcpy(newBuch->Buchtitel, "");
    strcpy(newBuch->Buchautor, "");
    newBuch->ISBN = 0;
    newBuch->AnzahlExemplare = 0;
    newBuch->ListeAusleiher.length = 0;
    newBuch->ListeAusleiher.first = NULL;
    return newBuch;
}

Bibliothek *newEmptyBibliothek() {
    Bibliothek *newBibliothek = malloc(sizeof(Bibliothek));
    if (newBibliothek == NULL) {
        if (DEBUG_MODE)printf("newEmptyBibliothek() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    newBibliothek->BuecherListe.first = NULL;
    newBibliothek->BuecherListe.length = 0;
    return newBibliothek;
}

Ausleiher *newEmptyAusleiher() {
    Ausleiher *newAusleiher = malloc(sizeof(Ausleiher));
    if (newAusleiher == NULL) {
        if (DEBUG_MODE)printf("newEmptyBibliothek() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    strcpy(newAusleiher->name, "");
    return newAusleiher;
}


int addBuch(Bibliothek *bibliothek, Buch *buch) {
    if (!buch || !bibliothek) {
        if (DEBUG_MODE)printf("addBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    return addListItem(&(bibliothek->BuecherListe), buch);
}

int removeBuch(Bibliothek* bib, int buchIndex){
    //fehlerchecks
    if (!bib || buchIndex<0 ) {
        if (DEBUG_MODE)printf("addBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    return removeListNode(&(bib->BuecherListe), buchIndex);
}

int checkInBuchByName(Buch *buch, const char *ausleiherName) {
    if (buch == NULL) {
        if (DEBUG_MODE)printf("checkInBuchByName() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    if (buch->AnzahlExemplare == 0 || buch->ListeAusleiher.length == 0) {
        if (DEBUG_MODE)printf("checkInBuchByName() Fehler: Buch hat keine Exemplare/Ausleiher!\n");
        return BIBL_ERROR; //Buch hat keine Exemplare, man kann nichts zurueckgeben
    }
    int ausleiherIndex = getAusleiherIndexByName(&(buch->ListeAusleiher), ausleiherName);
    if (ausleiherIndex < 0) {
        if (DEBUG_MODE) printf("checkInBuchByName: Fehler, konnte Ausleiher nicht finden!\n");
        return BIBL_ERROR;
    }
    Ausleiher *ptrAusleiherToRemove = getListData(&(buch->ListeAusleiher), ausleiherIndex);
    free(ptrAusleiherToRemove);
    removeListNode(&(buch->ListeAusleiher), ausleiherIndex);
    return BIBL_SUCCESS;
}

int checkInBuchByIndex(Buch *buch, int auslIndex) {
    if (buch == NULL) {
        if (DEBUG_MODE)printf("checkInBuchByIndex() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    if (buch->AnzahlExemplare == 0 || buch->ListeAusleiher.length == 0) {
        if (DEBUG_MODE)printf("checkInBuchByIndex() Fehler: Buch hat keine Exemplare/Ausleiher!\n");
        return BIBL_ERROR; //Buch hat keine Exemplare, man kann nichts zurueckgeben
    }
    if (auslIndex < 0) {
        if (DEBUG_MODE) printf("checkInBuchByIndex: Fehler, konnte Ausleiher nicht finden!\n");
        return BIBL_ERROR;
    }
    Ausleiher *ptrAusleiherToRemove = getListData(&(buch->ListeAusleiher), auslIndex);
    free(ptrAusleiherToRemove);
    removeListNode(&(buch->ListeAusleiher), auslIndex);
    return BIBL_SUCCESS;
}

int checkOutBuch(Buch *buch, const char *ausleiherName) {
    if (buch == NULL) return BIBL_ERROR;
    if (buch->ListeAusleiher.length == buch->AnzahlExemplare) return BIBL_ERROR;
    Ausleiher *new_ausleiher = newEmptyAusleiher();
    strncpy(new_ausleiher->name, ausleiherName, MAXBUFFERSIZE);
    new_ausleiher->name[MAXBUFFERSIZE - 1] = '\0';
    addListItem(&(buch->ListeAusleiher), new_ausleiher);
    return BIBL_SUCCESS;
}

int printBuch(Buch *buch) {
    if (!buch) return BIBL_ERROR;
    printf("Titel: %s", buch->Buchtitel);
    printf("Autor: %s", buch->Buchautor);
    printf("ISBN: %lld\n", buch->ISBN);
    printf("Exemplare: %d\n", buch->AnzahlExemplare);
    if (buch->ListeAusleiher.length > 0) {
        printf("Ausleiher:\n");
    } else {
        printf("(keine Ausleiher)\n");
    }
    for (int i = 0; i < buch->ListeAusleiher.length; i++) {
        printf("\t%d)%s", i, (char *) getListData(&(buch->ListeAusleiher), i));
    }
    printf("\n");
    return BIBL_SUCCESS;
}

int printBibliothek(Bibliothek *bibliothek) {
    if (bibliothek==NULL) return BIBL_ERROR;
    printf("Bibliothek:\n");
    int anzBuecher = bibliothek->BuecherListe.length;
    if (anzBuecher == 0) {
        printf("keine Buecher registriert\n");
        return BIBL_SUCCESS;
    }
    printf("%d Buecher\n", anzBuecher);
    for (int i = 0; i < anzBuecher; i++) {
        printf("\nBuch %d:\n", i + 1);
        printBuch(getListData(&(bibliothek->BuecherListe), i));
    }
    return BIBL_SUCCESS;
}

int getAusleiherIndexByName(LinkedList *ListeAusleiher, const char *ausleiherName) {
    for (int i = 0; i < ListeAusleiher->length; i++) {
        if (!strcmp(getListData(ListeAusleiher, i), ausleiherName)) return i;
    }
    return BIBL_ERROR;
}

int getAusleiherCount(Bibliothek *bib, int buchIndex) {
    if (bib == NULL || buchIndex > bib->BuecherListe.length || buchIndex < 0) return BIBL_ERROR;
    return ((Buch *) getListData(&(bib->BuecherListe), buchIndex))->ListeAusleiher.length;
}

Buch *getBuchByIndex(Bibliothek *bib, int index) {
    Buch *tempBuch;
    tempBuch = (Buch *) getListData(&(bib->BuecherListe), index);
    if (tempBuch == NULL && DEBUG_MODE) printf("getBuchByIndex: Fehler, konnte Buch %d nicht finden!\n", index);
    return tempBuch;
}

int freeBib(Bibliothek *bib) {
    if (bib == NULL) return BIBL_SUCCESS;
    int countBuecher = bib->BuecherListe.length;
    int indexBuecher = 0;
    for (indexBuecher = 0; indexBuecher < countBuecher; indexBuecher++) {
        if (freeBuch(bib, 0)) {
            if (DEBUG_MODE) printf("freeBib: Fehler bei Buchspeicherfreigabe!\n");
            return BIBL_SEVERE;
        }
    }
    free(bib);
    if (DEBUG_MODE) printf("(freeBib: Bibliothek geloescht!)\n");
    return BIBL_SUCCESS;
}

int freeBuch(Bibliothek *bib, int index) {
    if (bib == NULL) return BIBL_SUCCESS;
    Buch *tempBuch;
    Ausleiher *tempAusl;
    int countAusl = 0;
    int indexAusl = 0;
    tempBuch = getListData(&(bib->BuecherListe), index);
    countAusl = tempBuch->ListeAusleiher.length;
    for (indexAusl = 0; indexAusl < countAusl; indexAusl++) {
        tempAusl = getListData(&(tempBuch->ListeAusleiher), 0);
        free(tempAusl);
        if (removeListNode(&(tempBuch->ListeAusleiher), 0)) {
            if (DEBUG_MODE)
                printf("freeBuch: Fehler bei Ausleiherspeicherfreigabe!\n");
            return BIBL_SEVERE;
        }
    }
    free(tempBuch);
    if (removeListNode(&(bib->BuecherListe), 0)) {
        if (DEBUG_MODE)
            printf("freeBuch: Fehler bei Buchspeicherfreigabe!\n");
        return BIBL_SEVERE;
    }
    return BIBL_SUCCESS;
}

/**
 * Durchsucht Bibliothek nach Buechern bei denen \p searchStr in Titel, Author, ISBN oder einem der Ausleiher auftritt
 * @param searchStr String nach dem gesucht wird
 * @param bib Bibliothek in der gesucht wird
 * @param searchIndex Pointer zum Buch-Index ab dem gesucht wird, schreibt neuen Index
 * @return Buch-Pointer zum naechsten gefundenen Buch oder NULL
 */
Buch * getNextBuchByString(char *searchStr, Bibliothek *bib, int *searchIndex){
    LLNode* tempNode = getListNode(&(bib->BuecherListe),*searchIndex);
    Buch* tempBuch;
    char tempStrNeedle[MAXBUFFERSIZE];
    char tempStrHaystack[MAXBUFFERSIZE];
    int newIndex=*searchIndex;
    int found=0;

    if (tempNode==NULL) return NULL;
    //loop buch
    if(searchStr[strnlen(searchStr,MAXBUFFERSIZE)-1]=='\n')
        searchStr[strnlen(searchStr,MAXBUFFERSIZE)-1]='\0';
    strToLower(searchStr, tempStrNeedle);
    do{
        tempBuch=tempNode->data;
        strToLower(tempBuch->Buchtitel, tempStrHaystack);
        if (strstr(tempStrHaystack, tempStrNeedle)!=NULL) found=1;
        strToLower(tempBuch->Buchautor, tempStrHaystack);
        if (strstr(tempStrHaystack, tempStrNeedle)!=NULL) found=1;
        sprintf(tempStrHaystack,"%lld",tempBuch->ISBN);
        if (strstr(tempStrHaystack, tempStrNeedle)!=NULL) found=1;
        //loop Ausleiher
        for (int indexAusl=0;indexAusl<tempBuch->ListeAusleiher.length;indexAusl++){
            strToLower( ((Ausleiher*)getListData(&(tempBuch->ListeAusleiher) ,indexAusl))->name, tempStrHaystack);
            if (strstr(tempStrHaystack, tempStrNeedle)!=NULL) found=1;
        }
        if (found){
            *searchIndex=newIndex;
            return tempBuch;
        }
        tempNode=tempNode->next;
        newIndex++;
    } while (tempNode!=NULL);
    return NULL;
}



int strToLower(const char *strIn, char *strOut) {
    if (strIn==NULL ||strOut==NULL) return BIBL_ERROR;
    for(int i=0; i<MAXBUFFERSIZE;i++){
        strOut[i]=(char)tolower(strIn[i]);
    }
    return BIBL_SUCCESS;
}

