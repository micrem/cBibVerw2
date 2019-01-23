//
// Created by user64 on 02/01/2019.
//

#include "BuchLibLoadSave.h"

//interne Hilfsfunktion

int loadError(const char* errorMsg, Bibliothek* bib, FILE* fp){
    if (DEBUG_MODE) printf("%s\n",errorMsg);
    //Bibliothek loeschen
    if (freeBib(bib)) {printf("Fehler! Unvollstaendige Bibliothek konnte nicht wieder aus Speicher entfernt werden! Programm wird beendet.\n"); exit(-1);}
    //Datenstream schliessen
    fclose(fp);

    return BIBL_SUCCESS;
}

Bibliothek* loadBib () {
    Bibliothek* bib = newEmptyBibliothek();
    FILE *fp;
    char str[MAXBUFFERSIZE];

    //Datei oeffnen
    //auf Fehler beim Oeffnen pruefen
    //auf leere Datei pruefen

    fp = fopen(SAVEFILENAME, "r");
    if(fp == NULL) {
        loadError("saveBib: Fehler, konnte Datei nicht oeffnen!\n",bib, fp);
        return(NULL);
    }
    int a;
    while(!feof(fp)){
        if(readStringFile(fp,str)) break;
        printf("'%s'\n",str);
        if(!readInteger(&a,str)) printf("(Num: %d)\n",a);
    }
    //if(readIntegerFile(fp,&a)) printf("err");
    //printf("a:%d\n",a);
    //Anzahl Buecher einlesen
    //loop

    //Titel

    //Author

    //ISBN

    //Anz Exemplare

    //Anz Ausleiher

    //loop Ausleiher


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
