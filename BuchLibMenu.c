/**
 * @file
 */

#include "BuchLibMenu.h"


char main_menu_text[][MAXBUFFERSIZE]= {
        {"Buch auswaehlen"},
        {"Buch neu anlegen"},
        {"Bibliothek anzeigen"},
        {"Bibliothek neu aus Datei laden (nichtgespeicherte Aenderungen gehen verloren)"},
        {"Bibliothek speichern"},
        {""}
};

char menu1_auswahl_text[][MAXBUFFERSIZE]= {
        {"Naechstes Buch anzeigen"},
        {"Buch ausleihen"},
        {"Buch zurueckgeben"},
        {"Buch bearbeiten"},
        {"Buch entfernen"},
        {""},
};

char menu12_ausleihen_text[][MAXBUFFERSIZE]= {
        {"Ausleiher Namen eingeben"},
        {""}
};


char menu13_zurueckgeben_text[][MAXBUFFERSIZE]= {
        {"Ausleiher Nummer eingeben"},
        {""}
};


char menu14_bearbeiten_text[][MAXBUFFERSIZE]= {
        {"Titel neu eingeben"},
        {"Author neu eingeben"},
        {"ISBN neu eingeben"},
        {"Anzahl Exemplare neu eingeben"},
        {""}
};

char menu_confirm_text[][MAXBUFFERSIZE]= {
        {"Bestaetigen"},
};


void waitUntilEnter(){
    char buf[MAXBUFFERSIZE];
    printf("(Weiter mit Enter)\n");
    readStringInput(buf);
}

void printMsgConfirm(char *message){
    printf("%s\n",message);
    waitUntilEnter();
}

/**
 * testComment
 * @param menu_text
 * @return
 */
int getMenuSelection(char menu_text[][MAXBUFFERSIZE]){
    int i=0;
    int menuSelection=-1;
    while (menu_text[i][0]!='\0'){
        printf("%d] %s\n",i+1,menu_text[i]);
        i++;
    }
    printf("0] Abbrechen\n");
    do{
        printf("(Bitte Nummer(0-%d) eingeben und mit 'Enter' bestaetigen)>",i);
        readIntegerInput(&menuSelection);
        printf("\n");
    }while (menuSelection<0 || menuSelection>i);
    return menuSelection;
}

void main_menu(Bibliothek* bib){
    int end = 0;
    do {
        switch (getMenuSelection(main_menu_text)) {
            case 0:
                end = 1;
                printf("Bibliothen vor Beenden speichern?\n");
                if(menu_confirm()){
                    if(saveBib(bib)!=BIBL_SUCCESS){
                        printMsgConfirm("Bibliothek konnte nicht gespeichert werden!");
                    }else {
                        printMsgConfirm("Bibliothek wurde erfolgreich gespeichert!");
                    };
                };
                break;
            case 1:
                menu1_auswahl(bib);
                break;
            case 2:
                menu2_buchneu(bib);
                break;
            case 3:
                printBibliothek(bib);
                break;
            case 4:
                if (freeBib(bib)==BIBL_SEVERE){
                    printMsgConfirm(
                            "Schwerwiegender Speicherfehler, unvollstaendige Bilbiothek konnte nicht aus Speicher entfernt werden.\nProgramm wird beendet!");
                    printf("(Enter zum Beenden)");
                    getc(stdin);
                    exit(-1);
                }
                bib = loadBib();
                if (bib==NULL){
                    printf("Bibliothek konnte nicht geladen werden!\n Leere Bibliothek wird angelegt.\n");
                    bib = newEmptyBibliothek();
                } else {
                    printMsgConfirm("Bibliothek wurde erfolgreich geladen!");
                }
                break;
            case 5:
                if(saveBib(bib)!=BIBL_SUCCESS){
                    printMsgConfirm("Bibliothek konnte nicht gespeichert werden!");
                }else {
                    printMsgConfirm("Bibliothek wurde erfolgreich gespeichert!");
                };


            default:
                break;
        };
    }while(!end);
    //freebib() optional, Programmende
}

void menu1_auswahl(Bibliothek* bib){
    char searchStr[MAXBUFFERSIZE];
    int buchSearchIndex=0;
    int end=0;
    Buch* buchPtr;

    if (bib==NULL ){ printMsgConfirm("Noch keine Bibliothek angelegt.");return;}
    if (bib->BuecherListe.length==0){ printMsgConfirm("Keine Buecher in Bibliothek.");return;}

    printf("Bitte Suchtext eingeben:\n");
    printf("(Ganz oder Teil von Titel, Author, Ausleiher oder ISBN)\n");
    if (readStringInput(searchStr)!=BIBL_SUCCESS || *searchStr=='\n') { printMsgConfirm("Eingabe nicht erkannt.");return;}
    buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
    if (buchPtr==NULL){ printMsgConfirm("Buch nicht gefunden.");return;}

    do {
        printBuch(buchPtr);
        switch (getMenuSelection(menu1_auswahl_text)) {
            case 0:
                end = 1;
                break;
            case 1:
                buchSearchIndex++;
                buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
                if (buchPtr==NULL){
                    printf("Kein weiteres Buch gefunden.\n");
                    printf("Suche von Vorne beginnen?\n");
                    if(menu_confirm()) buchSearchIndex=0;
                    buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
                    if (buchPtr==NULL){ printMsgConfirm("Kein Buch gefunden.");return;}
                }
                break;
            case 2:
                menu12_ausleihen(buchPtr);
                break;
            case 3:
                menu13_zurueckgeben(buchPtr);
                break;
            case 4:
                menu14_bearbeiten(buchPtr);
                break;
            case 5:
                if (buchPtr->ListeAusleiher.length > 0){
                    printMsgConfirm("Buch hat noch ausgeliehene Exemplare, konnte Buch nicht entfernen!");
                    break;
                }
                printf("Ausgewaehltes Buch entfernen?\n");
                if(menu_confirm()){
                    if(removeBuch(bib,buchSearchIndex)!=BIBL_SUCCESS){
                        printMsgConfirm("Konnte Buch nicht entfernen!");
                    }else{
                        printf("Buch entfernt!\n");
                        waitUntilEnter();
                        buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
                        if (buchPtr==NULL){ printMsgConfirm("Kein weiteres Buch gefunden.");return;}
                    }
                }
                break;
            default:
                break;
        };
    }while(!end);
    //printbook
    //show menu
}

void menu12_ausleihen(Buch* buch){
    //namen eingeben
    char auslName[MAXBUFFERSIZE];
    int ret =0;
    if (buch->ListeAusleiher.length == buch->AnzahlExemplare) {
        printMsgConfirm("Keine freien Exemplare zum Ausleihen vorhanden!");
        return;
    };
    if (getMenuSelection(menu12_ausleihen_text)==0) return;
    do{
        printf("Bitte Ausleiher Namen eingeben (ohne Umlaute):\n");
        ret = readStringInput(auslName);
    } while (ret != BIBL_SUCCESS);
    if(checkOutBuch(buch, auslName)!=BIBL_SUCCESS){
        printMsgConfirm("Konnte Buch nicht ausleihen!");
    }else{
        printf("Buch ausgeliehen!\n");
        waitUntilEnter();
    };
}

void menu13_zurueckgeben(Buch* buch){
    int menuSelection=-1;
    int auslCount=buch->ListeAusleiher.length;
    if (auslCount==0) {
        printMsgConfirm("Keine verliehenen Exemplare vorhanden!");
        return;
    };
    if (getMenuSelection(menu13_zurueckgeben_text)==0) return;
    do{
        for (int auslIndex = 0; auslIndex < buch->ListeAusleiher.length; auslIndex++) {
            printf("\t%d)%s", auslIndex+1, (char *) getListData(&(buch->ListeAusleiher),auslIndex));
        }
        printf("(Bitte Nummer(1-%d) eingeben und mit 'Enter' bestaetigen)>",auslCount);
        readIntegerInput(&menuSelection);
        printf("\n");
    }while (menuSelection<1 || menuSelection>auslCount);

    if(checkInBuchByIndex(buch, menuSelection-1) != BIBL_SUCCESS){
        printMsgConfirm("Konnte Buch nicht zurueckgeben!");
    }else{
        printMsgConfirm("Buch zurueckgegeben!");
    };
}

void menu14_bearbeiten(Buch* buch){
    //select menu14

    char tempStr[MAXBUFFERSIZE];
    long long tempLL;
    int tempInt;
    printf("Buch bearbeiten (bitte alle Angaben ohne Umlaute):\n");
    switch (getMenuSelection(menu14_bearbeiten_text)) {
        case 0: //Abbrechen
            break;
        case 1: //Titel
            do{
                printf("Bitte neuen Titel eingeben:\n");
            }while (readStringInput(tempStr));
            strncpy(buch->Buchtitel,tempStr,MAXBUFFERSIZE);
            break;
        case 2: //Author
            do{
                printf("Bitte neuen Author eingeben:\n");
            }while (readStringInput(tempStr));
            strncpy(buch->Buchautor,tempStr,MAXBUFFERSIZE);
            break;
        case 3: //ISBN
            do{
                printf("Bitte neue ISBN eingeben:\n");
            }while (readISBNInput(&tempLL));
            buch->ISBN=tempLL;
            break;
        case 4: //Exemplare
            //!! error check: if (exemplare < ausleiherCount) error
            do{
                printf("Anzahl Ausleiher:%d\n",buch->ListeAusleiher.length);
                printf("(Neue Anzahl Exemplare darf Anzahl Ausleiher nicht unterschreiten)\n");
                printf("Bitte neue Anzahl der Exemplare eingeben(mind. 1):\n");
            }while (readIntegerInput(&tempInt)
                    || tempInt<1
                    || tempInt<buch->ListeAusleiher.length);
            buch->AnzahlExemplare=tempInt;
            break;
        default:
            break;
    }

}

void menu2_buchneu(Bibliothek* bib){
    char tempStr[MAXBUFFERSIZE];
    long long tempLL;
    int tempInt;
    Buch* tempBuch=newEmptyBuch();
    printf("Neues Buch anlegen (bitte alle Angaben ohne Umlaute):\n");
    //Titel
    do{
        printf("Bitte Titel eingeben:\n");
    }while (readStringInput(tempStr));
    strncpy(tempBuch->Buchtitel,tempStr,MAXBUFFERSIZE);
    //Author
    do{
        printf("Bitte Author eingeben:\n");
    }while (readStringInput(tempStr));
    strncpy(tempBuch->Buchautor,tempStr,MAXBUFFERSIZE);

    //ISBN
    do{
        printf("Bitte ISBN eingeben:\n");
    }while (readISBNInput(&tempLL));
    tempBuch->ISBN=tempLL;

    //Exemplare
    do{
        printf("Bitte Anzahl der Exemplare eingeben (mind. 1):\n");
    }while (readIntegerInput(&tempInt) || tempInt<1);
    tempBuch->AnzahlExemplare=tempInt;
    printf("\n");
    printf("Buch aufnehmen?\n");
    if(menu_confirm()==1 && addBuch(bib,tempBuch)==BIBL_SUCCESS){
        printf("Buch aufgenommen!\n");
        waitUntilEnter();
    }else{
        printMsgConfirm("Konnte Buch nicht aufnehmen!");
        free(tempBuch);
    }
}

int menu_confirm(){
    return getMenuSelection(menu_confirm_text);
}

