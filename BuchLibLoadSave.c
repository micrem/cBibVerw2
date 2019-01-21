//
// Created by user64 on 02/01/2019.
//

#include "BuchLibLoadSave.h"

int loadBib (Bibliothek *bib) {
    FILE *fp;
    char str[60];

    /* opening file for reading */
    fp = fopen("file.txt" , "r");
    if(fp == NULL) {
        perror("Error opening file");
        return(-1);
    }
    if( fgets (str, 60, fp)!=NULL ) {
        /* writing content to stdout */
        puts(str);
    }
    fclose(fp);

    return(0);
}

int saveBib(Bibliothek *bib) {
    FILE *fp;
    char str[MAXBUFFERSIZE];
    int buchIndex=0;
    int ausleiherIndex=0;
    LLNode* tempBuchNode=NULL;
    LLNode* tempAuslNode=NULL;
    Buch *tempBuch;
    Ausleiher *tempAusleher;

    fp = fopen(SAVEFILENAME, "w");
    if(fp==NULL) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datei nicht oeffnen!\n"); return BIBL_ERROR;}
    for(buchIndex=0;buchIndex<bib->BuecherListe.length;buchIndex++){
        if(buchIndex==0) {tempBuchNode=bib->BuecherListe.first;}
        else {tempBuchNode=tempBuchNode->next;}
        tempBuch=(Buch*)(tempBuchNode->data);
        fputs(tempBuch->Buchtitel,fp);
        fputs(tempBuch->Buchautor,fp);
        fprintf(fp, "ISBN: %lld\n",tempBuch->ISBN);
        fprintf(fp, "Exemplare: %d\n",tempBuch->AnzahlExemplare);
        fprintf(fp, "AnzAusleiher: %d\n",tempBuch->ListeAusleiher.length);
        fflush(fp);

        for(ausleiherIndex=0;ausleiherIndex<getAusleiherCount(bib,buchIndex);ausleiherIndex++) {
            if (ausleiherIndex == 0) { tempAuslNode = tempBuch->ListeAusleiher.first; }
            else { tempAuslNode = tempAuslNode->next; }
            tempAusleher = (Ausleiher*) (tempAuslNode->data);
            fputs(tempAusleher->name,fp);
        }
    }


    if(fclose(fp)) {if (DEBUG_MODE) printf("saveBib: Fehler, konnte Datei nicht schliessen!\n"); return BIBL_ERROR;}
    return BIBL_SUCCESS;

    /* opening file for reading */
    int i=0;
    fp = fopen(SAVEFILENAME , "r");
    if(fp == NULL) {
        perror("Error opening file");
        return(-1);
    }
    while( fgets (str, MAXBUFFERSIZE, fp)!=NULL ) {
        /* writing content to stdout */
        printf(":");
        i=0;
        while (str[i]!='\0'){
            printf("%d '%c'\n", str[i],str[i]); i++;}
    }
    fclose(fp);

    return 0;
}
