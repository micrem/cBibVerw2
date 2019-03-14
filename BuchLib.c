#include "BuchLib.h"

/**
 * legt neues leeres Buch mit malloc() an und initialisiert alle Variablen auf 0 oder entsprechende Werte.
 * @return Pointer auf neues, initialisiertes Buch oder NULL bei Fehler
 */
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

/**
 * legt neue leere Bibliothek mit malloc() an und initialisiert alle Variablen auf 0 oder ensprechende Werte.
 * @return Pointer auf neue Bibliothek oder NULL bei Fehler
 */
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

/**
 * legt neuen Ausleiher mit malloc() an und initialisiert alle Variablen auf 0 oder ensprechende Werte.
 * @return Pointer auf neuen Ausleiher oder NULL bei Fehler
 */
Ausleiher *newEmptyAusleiher() {
    Ausleiher *newAusleiher = malloc(sizeof(Ausleiher));
    if (newAusleiher == NULL) {
        if (DEBUG_MODE)printf("newEmptyBibliothek() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    strcpy(newAusleiher->name, "");
    return newAusleiher;
}

/**
 * fuegt zur Bibliothek bib das Buch buch hinzu, mithilfe von LinkedList Zugriffsfunktion addListItem
 * @param bib Pointer auf Bibliothek in die das buch hinzugefuegt werden soll
 * @param buch Pointer auf das Buch das hinzugefuegt wird
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int addBuch(Bibliothek *bib, Buch *buch) {
    //Fehlerchecks: leere Pointer
    if (!buch || !bib) {
        if (DEBUG_MODE)printf("addBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    return addListItem(&(bib->BuecherListe), buch); //reicht Fehler aus addListItem() weiter
}

/**
 * entfernt das Buch an Stelle Index aus der Bibliothek bib
 * gibt Speicher von Buch frei
 * @param bib Bibliothek aus der das Buch entfernt wird
 * @param buchIndex index des Buches das entfernt wird
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int removeBuch(Bibliothek* bib, int buchIndex){
    //Fehlerchecks: leere Pointer oder negativer Index
    if (!bib || buchIndex<0 ) {
        if (DEBUG_MODE)printf("addBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    return freeBuch(bib,buchIndex); //reicht Fehler aus Freigabefunktion weiter
}

/**
 * gibt Buch an Bibliothek bib zurueck, vom Ausleiher mit Index auslIndex
 * @param buch Pointer auf das Buch von dem ein Exemplar zurueckgegeben wird
 * @param auslIndex Index des Ausleihers
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int checkInBuchByIndex(Buch *buch, int auslIndex) {
    //Fehlerchecks: NULL-parameter, ob Buch Exemplare hat und ob Buch Ausleiher hat, negativer Index parameter
    if (buch == NULL) {
        if (DEBUG_MODE)printf("checkInBuchByIndex() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    if (buch->AnzahlExemplare == 0 || buch->ListeAusleiher.length == 0) {
        if (DEBUG_MODE)printf("checkInBuchByIndex() Fehler: Buch hat keine Exemplare/Ausleiher!\n");
        return BIBL_ERROR; //Buch hat keine Exemplare/Ausleiher, man kann nichts zurueckgeben
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

/**
 * Buch buch ausleihen, an Ausleiher mit Namen ausleiherName
 * @param buch Buch das ausgeliehen wird
 * @param ausleiherName Name des Ausleihers
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int checkOutBuch(Buch *buch, const char *ausleiherName) {
    //Fehlerchecks: NULL Buch-Parameter, alle Exemplare verliehen
    if (buch == NULL) return BIBL_ERROR;
    if (buch->ListeAusleiher.length == buch->AnzahlExemplare) return BIBL_ERROR;
    Ausleiher *new_ausleiher = newEmptyAusleiher(); //neuen Ausleiher initialisieren
    if(new_ausleiher==NULL) return BIBL_ERROR; //entspricht malloc()-Fehler
    strncpy(new_ausleiher->name, ausleiherName, MAXBUFFERSIZE);
    new_ausleiher->name[MAXBUFFERSIZE - 1] = '\0'; //zur Sicherheit Namenstring abschliessen
    addListItem(&(buch->ListeAusleiher), new_ausleiher); //zur Ausleiher LinkedList hinzufuegen
    return BIBL_SUCCESS;
}

/**
 * Buch buch lesbar in Konsole ausgeben
 * @param buch Pointer auf das Buch das ausgegeben werden soll
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int printBuch(Buch *buch) {
    if (!buch) return BIBL_ERROR;
    printf("Titel: %s", buch->Buchtitel);
    printf("Autor: %s", buch->Buchautor);
    printf("ISBN: %lld\n", buch->ISBN);
    printf("Exemplare: %d\n", buch->AnzahlExemplare);
    //falls Ausleiher vorhanden, 'header' ausgeben
    if (buch->ListeAusleiher.length > 0) {
        printf("Ausleiher:\n");
    } else {
        printf("(keine Ausleiher)\n");
    }
    //Schleife um alle Ausleiher auszugeben, mit index
    for (int i = 0; i < buch->ListeAusleiher.length; i++) {
        printf("\t%d)%s", i, (char *) getListData(&(buch->ListeAusleiher), i));
    }
    printf("\n");
    return BIBL_SUCCESS;
}

/**
 * Bibliothek lesbar formatiert in Konsole ausgeben
 * @param bibliothek Pointer auf Bibliothek die ausgegeben werden soll
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int printBibliothek(Bibliothek *bibliothek) {
    if (bibliothek==NULL) return BIBL_ERROR;
    printf("Bibliothek:\n");
    int anzBuecher = bibliothek->BuecherListe.length;
    //falls keine Buecher vorhanden
    if (anzBuecher == 0) {
        printf("keine Buecher registriert\n");
        return BIBL_SUCCESS;
    }
    printf("%d Buecher\n", anzBuecher);
    //loop ueber Buecher der Bibliothek
    for (int i = 0; i < anzBuecher; i++) {
        printf("\nBuch %d:\n", i + 1);
        printBuch(getListData(&(bibliothek->BuecherListe), i));
    }
    return BIBL_SUCCESS;
}

/**
 * Gibt Anzahl der Ausleiher des Buches an Stelle "buchIndex" in Bibliothek bib aus
 * @param bib Bibliothek in der das Buch eingetragen ist
 * @param buchIndex Index des Buches
 * @return gibt Anzahl der Ausleiher aus oder BIBL_ERROR
 */
int getAusleiherCount(Bibliothek *bib, int buchIndex) {
    //Fehlerchecks: buchIndex out-of-bounds, NULL-Pointer
    Buch* tempBuch;
    if (bib == NULL || buchIndex > bib->BuecherListe.length || buchIndex < 0) return BIBL_ERROR;
    tempBuch = getBuchByIndex(bib, buchIndex); //Zeiger auf entspr. Buch holen
    if (tempBuch == NULL){
        if (DEBUG_MODE) printf("getBuchByIndex: Fehler, konnte Buch %d nicht finden!\n", buchIndex);
        return BIBL_ERROR;
    }
    return tempBuch->ListeAusleiher.length;
}

/**
 * Gibt Pointer auf Buch aus Bibliothek aus, nach Index
 * @param bib
 * @param buchIndex
 * @return
 */
Buch *getBuchByIndex(Bibliothek *bib, int buchIndex) {
    Buch *tempBuch;
    tempBuch = (Buch *) getListData(&(bib->BuecherListe), buchIndex);
    if (tempBuch == NULL && DEBUG_MODE) printf("getBuchByIndex: Fehler, konnte Buch %d nicht finden!\n", buchIndex);
    return tempBuch;
}

/**
 *
 * @param bib
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
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

/**
 *
 * @param bib
 * @param index
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
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
 * Durchsucht Bibliothek nach Buechern bei denen \p searchStr in Titel, Author, ISBN oder einem der Ausleiher auftritt,
 * konvertiert Suche vorher zu lowercase
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
    int found=0; //found-flag, wird auf 1 gesetzt falls searchStr irgendwo im Buch auftaucht

    if (tempNode==NULL) return NULL;
    //loop buch
    if(searchStr[strnlen(searchStr,MAXBUFFERSIZE)-1]=='\n') //'Enter' im String mit Terminierung ersetzen
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


/**
 * Hilfsfuntion, schreibt zu lowercase konvertierten strIn nach strOut
 * @param strIn Pointer zum String der Konvertiert wird
 * @param strOut Pointer zum String in den der neue String geschrieben wird
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int strToLower(const char *strIn, char *strOut) {
    if (strIn==NULL ||strOut==NULL) return BIBL_ERROR;
    for(int i=0; i<MAXBUFFERSIZE;i++){
        strOut[i]=(char)tolower(strIn[i]);
    }
    return BIBL_SUCCESS;
}

