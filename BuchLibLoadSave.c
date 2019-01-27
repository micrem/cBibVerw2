//
// Created by user64 on 02/01/2019.
//

#include "BuchLibLoadSave.h"

//interne Hilfsfunktion

int loadError(const char* errorMsg, Bibliothek* bib, FILE* fp){
    if (DEBUG_MODE) printf("%s",errorMsg);
    //Bibliothek loeschen
    if (freeBib(bib)) {printf("Fehler! Unvollstaendig geladene Bibliothek konnte nicht wieder aus Speicher entfernt werden! Programm wird beendet.\n"); exit(-1);}
    //Datenstream schliessen
    fclose(fp);

    return BIBL_SUCCESS;
}


Bibliothek* loadBib () {
    Bibliothek* bib = newEmptyBibliothek();
    FILE *fp;
    char str[MAXBUFFERSIZE];
    int buecherCount=0;
    int auslCount=0;
    int buecherIndex=0;
    int auslIndex=0;
    Buch* tempBuch;

    //Datei oeffnen
    //auf Fehler beim Oeffnen pruefen
    //auf leere Datei pruefen

    fp = fopen(SAVEFILENAME, "r");
    if(fp == NULL) {
        loadError("loadBib: Fehler, konnte Datei nicht oeffnen!\n",bib, fp);
        return(NULL);
    }

    int tempInt;

    //Anzahl Buecher einlesen
    if (readIntegerFile(fp,&tempInt)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
    if(feof(fp) && tempInt!=0) {loadError("loadBib: Fehler, Datei zu kurz!\n",bib,fp); return NULL;}
    if(readInteger(&tempInt,str) || tempInt<0 ) {loadError("loadBib: Fehler, konnte Anzahl Buecher nicht laden!\n",bib,fp); return NULL;}
    buecherCount=tempInt;

    //loop Buecher
    for (buecherIndex=0;buecherIndex<buecherCount;buecherIndex++){
        //Titel
        tempBuch = newEmptyBuch();
        addBuch(bib, tempBuch);
        if (readStringFile(fp,str) || feof(fp)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
        strcpy(tempBuch->Buchtitel,str);
        //Author
        if (readStringFile(fp,str) || feof(fp)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
        strcpy(tempBuch->Buchtitel,str);
        //ISBN
        if (readISBNFile(fp,&(tempBuch->ISBN)) || feof(fp)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
        //Anz Exemplare
        if (readIntegerFile(fp,&(tempBuch->AnzahlExemplare)) || feof(fp)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
        //Anz Ausleiher
        if (readIntegerFile(fp,&auslCount) || feof(fp)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
        //loop Ausleiher
        for (auslIndex=0;auslIndex<auslCount;auslIndex++){
            if (readStringFile(fp,str) || (feof(fp) && /* TODO:CHECK EQ*/ auslIndex+1<auslCount)) {loadError("loadBib: Fehler, konnte aus Datei nicht lesen!\n",bib,fp); return NULL;}
            checkOutBuch(tempBuch,str);
        }
        return bib;
    }
    fclose(fp);
    return(0);
}

int saveBib(Bibliothek *bib) {
    FILE *fp;
    int buchIndex=0;
    int ausleiherIndex=0;
    LLNode* tempBuchNode=NULL;
    LLNode* tempAuslNode=NULL;
    Buch *tempBuch;
    Ausleiher *tempAusleher;

    fp = fopen(SAVEFILENAME, "w");
    if(fp==NULL) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datei nicht oeffnen!\n"); return BIBL_ERROR;}
    fprintf(fp, "%d\n",bib->BuecherListe.length);
    for(buchIndex=0;buchIndex<bib->BuecherListe.length;buchIndex++){
        if(buchIndex==0) {tempBuchNode=bib->BuecherListe.first;}
        else {tempBuchNode=tempBuchNode->next;}
        tempBuch=(Buch*)(tempBuchNode->data);
        fputs(tempBuch->Buchtitel,fp);
        fputs(tempBuch->Buchautor,fp);
        fprintf(fp, "%lld\n",tempBuch->ISBN);
        fprintf(fp, "%d\n",tempBuch->AnzahlExemplare);
        fprintf(fp, "%d\n",tempBuch->ListeAusleiher.length);
        if(fflush(fp)) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datensatz nicht schreiben!\n"); return BIBL_ERROR;}

        for(ausleiherIndex=0;ausleiherIndex<getAusleiherCount(bib,buchIndex);ausleiherIndex++) {
            if (ausleiherIndex == 0) { tempAuslNode = tempBuch->ListeAusleiher.first; }
            else { tempAuslNode = tempAuslNode->next; }
            tempAusleher = (Ausleiher*) (tempAuslNode->data);
            fputs(tempAusleher->name,fp);
        }
    }

    if(fclose(fp)) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datei nicht schliessen!\n"); return BIBL_ERROR;}
    return BIBL_SUCCESS;
}
