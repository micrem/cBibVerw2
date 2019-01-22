#include "BuchLib.h"

Buch* newEmptyBuch(){
    Buch * newBuch = malloc(sizeof(Buch));
    if (newBuch==NULL){
        if(DEBUG_MODE)printf("newEmptyBuch() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    strcpy(newBuch->Buchtitel, "");
    strcpy(newBuch->Buchautor, "");
    newBuch->ISBN=0;
    newBuch->AnzahlExemplare=0;
    newBuch->ListeAusleiher.length=0;
    newBuch->ListeAusleiher.first=NULL;
    return newBuch;
}

Bibliothek* newEmptyBibliothek(){
    Bibliothek * newBibliothek = malloc(sizeof(Bibliothek));
    if (newBibliothek==NULL){
        if(DEBUG_MODE)printf("newEmptyBibliothek() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    newBibliothek->BuecherListe.first=NULL;
    newBibliothek->BuecherListe.length=0;
    return newBibliothek;
}

Ausleiher* newEmptyAusleiher(){
    Ausleiher * newAusleiher = malloc(sizeof(Ausleiher));
    if (newAusleiher==NULL){
        if(DEBUG_MODE)printf("newEmptyBibliothek() Fehler: konnte neuen Speicher nicht anlegen\n");
        return NULL;
    }
    strcpy(newAusleiher->name, "");
    return newAusleiher;
}


int addBuch(Bibliothek* bibliothek, Buch* buch) {
    if (!buch || !bibliothek){
        if(DEBUG_MODE)printf("addBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    return addListItem(&(bibliothek->BuecherListe), buch);
}

int checkInBuch(Buch *buch, const char* ausleiherName) {
    if (buch == NULL){
        if(DEBUG_MODE)printf("checkInBuch() Fehler: NULL-Parameter\n");
        return BIBL_ERROR;
    }
    if (buch->AnzahlExemplare == 0 || buch->ListeAusleiher.length == 0){
        if(DEBUG_MODE)printf("checkInBuch() Fehler: Buch hat keine Exemplare/Ausleiher!\n");
        return BIBL_ERROR; //Buch hat keine Exemplare, man kann nichts zurueckgeben
    }
    int ausleiherIndex = getAusleiherIndexByName(&(buch->ListeAusleiher),ausleiherName);
    if (ausleiherIndex<0) {
        if(DEBUG_MODE) printf("checkInBuch: Fehler, index<0 von 'getAusleiherIndexByName()' erhalten\n");
        return BIBL_ERROR;
    }
    Ausleiher* ptrAusleiherToRemove =  getListData(&(buch->ListeAusleiher), ausleiherIndex);
    free(ptrAusleiherToRemove);
    removeListItem(&(buch->ListeAusleiher),ausleiherIndex);
    return BIBL_SUCCESS;
}

int checkOutBuch(Buch *buch, const char* ausleiherName) {
    if(buch==NULL) return BIBL_ERROR;
    if(buch->ListeAusleiher.length == buch->AnzahlExemplare) return BIBL_ERROR;
    Ausleiher* new_ausleiher = newEmptyAusleiher();
    strncpy(new_ausleiher->name, ausleiherName, MAXBUFFERSIZE);
    new_ausleiher->name[MAXBUFFERSIZE-1]='\0';
    addListItem(&(buch->ListeAusleiher), new_ausleiher);
    return BIBL_SUCCESS;
}

int printBuch(Buch *buch) {
    if (!buch ) return BIBL_ERROR;
    printf("Titel: '%s'\n", buch->Buchtitel);
    printf("Autor: '%s'\n", buch->Buchautor);
    printf("ISBN: %lld\n", buch->ISBN);
    printf("Exemplare: %d\n", buch->AnzahlExemplare);
    printf("Ausleiher:\n");
    for (int i=0; i<buch->ListeAusleiher.length; i++){
        printf("\t%s\n",(char*)getListData(&(buch->ListeAusleiher),i ));
    }
    printf("\n");
    return BIBL_SUCCESS;
}

int printBibliothek(Bibliothek *bibliothek) {
    if (!bibliothek ) return BIBL_ERROR;
    for (int i=0; i<bibliothek->BuecherListe.length; i++){
        printBuch(getListData(&(bibliothek->BuecherListe),i));
    }
    return BIBL_SUCCESS;
}

int getAusleiherIndexByName(LinkedList* ListeAusleiher, const char* ausleiherName){
    for (int i=0; i<ListeAusleiher->length; i++){
        if (!strcmp(getListData(ListeAusleiher,i),ausleiherName)) return i;
    }
    return BIBL_ERROR;
}

int getAusleiherCount(Bibliothek *bib, int buchIndex) {
    if (bib==NULL || buchIndex>bib->BuecherListe.length || buchIndex<0) return BIBL_ERROR;
    return ((Buch*)getListData(&(bib->BuecherListe),buchIndex))->ListeAusleiher.length;
}

Buch *getBuchByIndex(Bibliothek *bib, int index) {
    Buch* tempBuch;
    tempBuch = (Buch*) getListData(&(bib->BuecherListe),index);
    if (tempBuch==NULL && DEBUG_MODE) printf("getBuchByIndex: Fehler, konnte Buch %d nicht finden!\n", index);
    return tempBuch;
}

//!ErrorHasOccured() ??!??! HandleError();