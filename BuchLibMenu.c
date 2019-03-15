/**
 * @file BuchLibMenu.c Implementierung von Benutzermenue Funktionen
 */

#include "BuchLibMenu.h"

//Texte fuer jeweilige Menues, Ausgabe mit getMenuSelection()
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

/**
 * Interne Hilfsfunktion, wartet auf Eingabe der Enter-Taste, analog zu Pause
 */
void waitUntilEnter(){
    char buf[MAXBUFFERSIZE];
    printf("(Weiter mit Enter)\n");
    readStringInput(buf); //Eingabe wird verworfen
}

/**
 *  Interne Hilfsfunktion, gibt uebergebene Nachricht aus und wartet auf Enter-Eingabe
 * @param message Nachricht die ausgegeben werden soll
 */
void printMsgConfirm(char *message){
    printf("%s\n",message);
    waitUntilEnter();
}

/**
 * Zeigt uebergebenes String-Array als Menu an, gibt Index des ausgewaehltes Menupunktes zurueck.
 * Letztes Element des char[][]-Arrays muss ein leerer String sein ("" bzw. '\0').
 * Menupunkt "Abbrechen" ist immer enthalten als Auswahl 0
 * @param menu_text String-Array der Menu-Auswahlpunkte
 * @return Index des Ausgewaehlten Menus
 */
int getMenuSelection(char menu_text[][MAXBUFFERSIZE]){
    int i=0;
    int menuSelection=-1;
    //loop solange bis korrekte Auswahl erkannt
    while (menu_text[i][0]!='\0'){
        printf("%d] %s\n",i+1,menu_text[i]);
        i++;
    }
    printf("0] Abbrechen\n"); //Abbrechen Menupunkt
    do{
        printf("(Bitte Nummer(0-%d) eingeben und mit 'Enter' bestaetigen)>",i);
        readIntegerInput(&menuSelection); //Return-Fehler koennen ignoriert werden, solange Wert korrekt ist
        printf("\n");
    }while (menuSelection<0 || menuSelection>i);
    return menuSelection;
}

/**
 * Funktion um Logik des Hauptmenues zu verwalten
 * @param bib Bibliothek auf der operiert werden soll
 */
void main_menu(Bibliothek* bib){
    int end = 0; //flag fuer Abbruch des Programms
    do {
        switch (getMenuSelection(main_menu_text)) { //je nach Auswahl entsprechendes Untermenu aufrufen
            case 0: //"Abbruch" Menupunkt
                end = 1;
                printf("Bibliothek vor Beenden speichern?\n");
                if(menu_confirm()){ //Wenn "ja"
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
            case 4: //Bibliothen neu laden
                if (freeBib(bib)==BIBL_SEVERE){
                    printMsgConfirm(
                            "Schwerwiegender Speicherfehler, unvollstaendige Bibliothek konnte nicht aus Speicher entfernt werden.\nProgramm wird beendet!");
                    printf("(Enter zum Beenden)");
                    getc(stdin); //Wartet auf Char-Eingabe, eingelesener Char wird verworfen
                    exit(-1); //Beendet mit Fehlercode
                }
                bib = loadBib(); //Bibliothek neu aus Datei einlesen
                if (bib==NULL){
                    printf("Bibliothek konnte nicht geladen werden!\n Leere Bibliothek wird angelegt.\n");
                    bib = newEmptyBibliothek();
                } else {
                    printMsgConfirm("Bibliothek wurde erfolgreich geladen!");
                }
                break;
            case 5: //Speichern
                if(saveBib(bib)!=BIBL_SUCCESS){
                    printMsgConfirm("Bibliothek konnte nicht gespeichert werden!");
                }else {
                    printMsgConfirm("Bibliothek wurde erfolgreich gespeichert!");
                };


            default:
                break;
        };
    }while(!end);
    //freebib() soll aufgerufen werden wo Bibliothek angelegt wurde
}

/**
 * Funktion die Logik fuer Menupunkt "Buch auswaehlen" verwaltet
 * Mit eingegebenem Suchstring werden Buecher gesucht, und sukzessive angezeigt,
 * mit weiteren Bearbeitungsoptionen per gefundenem Buch.
 * @param bib Bibliothek in der nach einem Buch gesucht werden soll
 */
void menu1_auswahl(Bibliothek* bib){
    char searchStr[MAXBUFFERSIZE];
    int buchSearchIndex=0; //Suchindex wo Suche startet, wird jeweils aufs naechste gefundene Buch gesetzt
    int end=0; //flag fuer Abbruch
    Buch* buchPtr; //Pointer aufs gefundene Buch

    //Fehlerchecks
    if (bib==NULL ){ printMsgConfirm("Noch keine Bibliothek angelegt.");return;}
    if (bib->BuecherListe.length==0){ printMsgConfirm("Keine Buecher in Bibliothek.");return;}

    //Suchstringeingabe und Fehlerchecks
    printf("Bitte Suchtext eingeben:\n");
    printf("(Ganz oder Teil von Titel, Author, Ausleiher oder ISBN)\n");
    //prueft auf Einfache 'Enter' (='\n') Eingabe separat, da es als korrekter String gilt aber nicht zur Suche eignet
    if (readStringInput(searchStr)!=BIBL_SUCCESS || *searchStr=='\n') { printMsgConfirm("Eingabe nicht erkannt.");return;}

    //erstes gefundenes Buch nach Index
    //wenn Buch gefunden, wird buchSearchIndex weiter gesetzt um dort mit Suche nach naechstem zutreffendem Buch fortzufahren
    buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
    if (buchPtr==NULL){ printMsgConfirm("Buch nicht gefunden.");return;}

    do { //weitere Suche
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
                    //falls nach Bearbeiten gar kein Buch mehr nach Suchkriterien gefunden wird
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
            case 5: //Buch loeschen
                if (buchPtr->ListeAusleiher.length > 0){
                    printMsgConfirm("Buch hat noch ausgeliehene Exemplare, konnte Buch nicht entfernen!");
                    break;
                }
                printf("Ausgewaehltes Buch entfernen?\n");
                if(menu_confirm()){ //wenn "ja" ausgewaehlt
                    if(removeBuch(bib,buchSearchIndex)!=BIBL_SUCCESS){
                        printMsgConfirm("Konnte Buch nicht entfernen!");
                        waitUntilEnter();
                    }else{
                        printf("Buch entfernt!\n");
                        waitUntilEnter();
                        //falls kein weiteres Buch Suchkriterien entspricht
                        //Buch-Pointer muss neu gesetzt werden, damit in naechster loop nicht
                        //das bereits geloeschte Buch referenziert wird
                        buchPtr = getNextBuchByString(searchStr, bib, &buchSearchIndex);
                        if (buchPtr==NULL){ printMsgConfirm("Kein weiteres Buch gefunden.");return;}
                    }
                }
                break;
            default:
                break;
        };
    }while(!end);

}

/**
 * Buch buch ausleihen, mit Eingabe des Ausleiher-Namens und checks nach Verfuegbarkeit von Exemplaren
 * @param buch
 */
void menu12_ausleihen(Buch* buch){
    //Namen eingeben
    char auslName[MAXBUFFERSIZE];
    int ret =0;
    //Verfuegbarkeit pruefen, klappt auch fuer Buecher mit 0 Exemplaren
    if (buch->ListeAusleiher.length == buch->AnzahlExemplare) {
        printMsgConfirm("Keine freien Exemplare zum Ausleihen vorhanden!");
        return;
    };

    if (getMenuSelection(menu12_ausleihen_text)==0) return; //Falls Menu "Abbrechen" gewaehlt wurde
    do{ //loop Eingaben bis akzeptabler Name erkannt wurde
        printf("Bitte Ausleiher Namen eingeben (ohne Umlaute):\n");
        ret = readStringInput(auslName);
    } while (ret != BIBL_SUCCESS);
    if(checkOutBuch(buch, auslName)!=BIBL_SUCCESS){ //eigentliches Ausleihen
        printMsgConfirm("Konnte Buch nicht ausleihen!");
    }else{
        printf("Buch ausgeliehen!\n");
        waitUntilEnter();
    };
}

/**
 * Buch Rueckgabe eines Buches, Ausleiher waehlen und aus Liste austragen
 * @param buch Buch das zurueckgegeben wird
 */
void menu13_zurueckgeben(Buch* buch){
    int menuSelection=-1;
    int auslCount=buch->ListeAusleiher.length;
    if (auslCount==0) {
        printMsgConfirm("Keine verliehenen Exemplare vorhanden!");
        return;
    };
    if (getMenuSelection(menu13_zurueckgeben_text)==0) return; //"Abbrechen" gewaehlt

    do{//loop solange bis gueltige Menueingabe erkannt
        //Namen und Index aller Ausleiher ausgeben, Index-eingabe verwerten
        for (int auslIndex = 0; auslIndex < buch->ListeAusleiher.length; auslIndex++) {
            printf("\t%d)%s", auslIndex+1, (char *) getListData(&(buch->ListeAusleiher),auslIndex));
        }
        printf("(Bitte Nummer(1-%d) eingeben und mit 'Enter' bestaetigen)>",auslCount);
        readIntegerInput(&menuSelection);
        printf("\n");
    }while (menuSelection<1 || menuSelection>auslCount);

    if(checkInBuchByIndex(buch, menuSelection-1) != BIBL_SUCCESS){ //Buch zurueckgeben
        printMsgConfirm("Konnte Buch nicht zurueckgeben!");
    }else{
        printMsgConfirm("Buch zurueckgegeben!");
    };
}

/**
 * Buch bearbeiten und beliebiges Feld anpassen
 * @param buch
 */
void menu14_bearbeiten(Buch* buch){
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
            do{
                printf("Anzahl Ausleiher:%d\n",buch->ListeAusleiher.length);
                printf("(Neue Anzahl Exemplare darf Anzahl aktueller Ausleiher nicht unterschreiten)\n");
                printf("Bitte neue Anzahl der Exemplare eingeben(mind. 1):\n");
            }while (readIntegerInput(&tempInt) //nicht erkannte Zahl eingegeben
                    || tempInt<1 //nicht unter 1 Exemplar setzen
                    || tempInt<buch->ListeAusleiher.length); //nicht weniger Exemplare als aktuelle Ausleiher
            buch->AnzahlExemplare=tempInt;
            break;
        default:
            break;
    }

}

/**
 * Buch neu anlegen, mit allen Feldern
 * @param bib Bibliothek in die das neue Buch eingetragen wird
 */
void menu2_buchneu(Bibliothek* bib){
    char tempStr[MAXBUFFERSIZE];
    long long tempLL;
    int tempInt;
    Buch* tempBuch=newEmptyBuch(); //neues Buch im Speicher anlegen
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
    }while (readIntegerInput(&tempInt) || tempInt<1); //nicht erkannte Zahl oder unter 1
    tempBuch->AnzahlExemplare=tempInt;
    printf("\n");
    printf("Buch aufnehmen?\n");
    if(menu_confirm()==1 && addBuch(bib,tempBuch)==BIBL_SUCCESS){
        printf("Buch aufgenommen!\n");
        waitUntilEnter();
    }else{
        printMsgConfirm("Konnte Buch nicht aufnehmen!");
        free(tempBuch); //Falls Buch nicht angelegt werden konnte, reservierten Speicher wieder freigeben
    }
}

/**
 * Fragt nach Bestaetigung vom Benutzer, Returnwert 1 = Bestaetigen, 0 = ABbrechen
 * @return 1 = Bestaetigen, 0 = Abbrechen
 */
int menu_confirm(){
    return getMenuSelection(menu_confirm_text);
}

