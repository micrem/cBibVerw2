/**
 * @file BuchLibLoadSave.c
 * Implementiert Bibliothek Laden aus Datei und Speicher in Datei
 */

#include "BuchLibLoadSave.h"

/**
 * Interne Hilfsfunktion. Aufgerufen wenn Fehler beim Laden einer gespeicherten Bibliothek auftreten
 * @param errorMsg String mit Fehlernachricht
 * @param bib Zeiger auf Bibliothek, um den Speicher einer evtl nur teilweise geladenen Bibliothek wieder freizugeben
 * @param fp Zeiger auf offenen Datei-Handler, wird geschlossen
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int loadError(const char* errorMsg, Bibliothek* bib, FILE* fp){
    int ret;
    //Fehlernachricht ausgeben
    if (*errorMsg == '\0') errorMsg = "loadBib: Fehler, konnte aus Datei nicht lesen!\n";
    if (DEBUG_MODE) printf("%s",errorMsg);
    //Bibliothek loeschen, Speicher freigeben
    ret = freeBib(bib);
    if (ret==BIBL_SEVERE) {
        printf("Fehler! Unvollstaendig geladene Bibliothek konnte nicht wieder aus Speicher entfernt werden! Programm wird beendet.\n");
        printf("(Enter zum Beenden)");
        getc(stdin);
        exit(-1);
    } else if (ret==BIBL_ERROR){
        if (DEBUG_MODE) printf("Fehler beim loeschen der Bibliothek!\n");
    }
    //Datenstream schliessen, falls exisitert (nicht NULL)
    if(fp) fclose(fp);

    return BIBL_SUCCESS;
}

/** Legt neue Bibliothek im Speicher an und laedt Daten aus Datei.
 * Bei Fehlern wird Speicher wieder freigegeben, und Funktion gibt NULL zurueck
 * @return gibt Zeiger auf geladene Bibliothek aus
 */
Bibliothek* loadBib () {
    Bibliothek* bib = newEmptyBibliothek();
    FILE *fp;
    char str[MAXBUFFERSIZE];
    int buecherCount=0;
    int auslCount=0;
    int buecherIndex=0;
    int auslIndex=0;
    int tempInt;
    Buch* tempBuch;

    //Datei oeffnen
    //auf Fehler beim Oeffnen pruefen
    //auf leere Datei pruefen
    fp = fopen(SAVEFILENAME, "r");
    if(fp == NULL) {
        loadError("loadBib: Fehler, konnte Datei nicht oeffnen!\n",bib, fp);
        return(NULL);
    }

    //erwartete Anzahl Buecher einlesen
    //pruefen auf Fehler beim Einlesen, zu fruehem Dateiende und negativer Anzahl
    if(readIntegerFile(fp,&tempInt)) {loadError("loadBib: Fehler, konnte Zeile aus Datei nicht lesen!\n",bib,fp); return NULL;}
    if(feof(fp) && tempInt!=0) {loadError("loadBib: Fehler, Datei zu kurz!\n",bib,fp); return NULL;}
    if(tempInt<0) {loadError("loadBib: Fehler, konnte Anzahl Buecher nicht laden!\n",bib,fp); return NULL;}
    buecherCount=tempInt;

    //Schleife Buecher
    for (buecherIndex=0;buecherIndex<buecherCount;buecherIndex++){
        //neues Buch
        tempBuch = newEmptyBuch();
        addBuch(bib, tempBuch);
        //Titel
        if (readStringFile(fp,str) || feof(fp)) {loadError("",bib,fp); return NULL;}
        strcpy(tempBuch->Buchtitel,str);
        //Author
        if (readStringFile(fp,str) || feof(fp)) {loadError("",bib,fp); return NULL;}
        strcpy(tempBuch->Buchautor,str);
        //ISBN
        if (readISBNFile(fp,&(tempBuch->ISBN)) || feof(fp)) {loadError("",bib,fp); return NULL;}
        //Anz Exemplare
        if (readIntegerFile(fp,&(tempBuch->AnzahlExemplare)) || feof(fp)) {loadError("",bib,fp); return NULL;}
        //Anz Ausleiher
        if (readIntegerFile(fp,&auslCount) || feof(fp)) {loadError("",bib,fp); return NULL;}
        //Schleife Ausleiher
        for (auslIndex=0;auslIndex<auslCount;auslIndex++){
            if (readStringFile(fp,str) || (feof(fp) && auslIndex+1<auslCount)) {loadError("",bib,fp); return NULL;}
            if(checkOutBuch(tempBuch,str)) {loadError("",bib,fp); return NULL;}
        }
    }
    fclose(fp);
    return bib;
}

/**
 * Speichert Bibliothek \p bib in Datei.
 * @param bib Zeiger auf Bibliothek die gespeichert werden soll
 * @return gibt BIBL_SUCCESS bei Erfolg aus oder BIBL_ERROR
 */
int saveBib(Bibliothek *bib) {
    FILE *fp;
    int buchIndex=0;
    int ausleiherIndex=0;
    LLNode* tempBuchNode=NULL;
    LLNode* tempAuslNode=NULL;
    Buch *tempBuch;
    Ausleiher *tempAusleher;

    fp = fopen(SAVEFILENAME, "w"); //SAVEFILENAME gesetzt in BuckLibSettings.h
    if(fp==NULL) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datei nicht oeffnen!\n"); return BIBL_ERROR;}

    //Anzahl Buecher
    fprintf(fp, "%d\n",bib->BuecherListe.length);

    //loop Buecher
    for(buchIndex=0;buchIndex<bib->BuecherListe.length;buchIndex++){
        //Buchzeiger tempBuch holen, aus der LinkedList der Bibliothek
        if(buchIndex==0) {tempBuchNode=bib->BuecherListe.first;} //bei erstem Buch
        else {tempBuchNode=tempBuchNode->next;} //bei nachfolgenden Buechern die Liste 'entlanghangeln'
        tempBuch=(Buch*)(tempBuchNode->data);
        //in Datei schreiben
        fputs(tempBuch->Buchtitel,fp);
        fputs(tempBuch->Buchautor,fp);
        fprintf(fp, "%lld\n",tempBuch->ISBN);
        fprintf(fp, "%d\n",tempBuch->AnzahlExemplare);
        fprintf(fp, "%d\n",tempBuch->ListeAusleiher.length);
        if(fflush(fp)) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datensatz nicht schreiben!\n"); return BIBL_ERROR;}
        //loop Ausleiher
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
