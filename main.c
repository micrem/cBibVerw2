/**
 * @file main.c
 * Hauptprogramm Bibliothekverwaltung
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"
#include "BuchLibMenu.h"

int main() {

    //Bibliothek-Pointer
    Bibliothek* bib;

    printf("--------------------------\n");
    printf("*  Bibliothekverwaltung  *\n");
    printf("--------------------------\n");

    //Versucht vorhandene Bibliothek aus Datei auszulesen
    bib=loadBib();

    //bei Einlesefehler leere Bibliothek im Speicher anlegen
    if (bib==NULL){
        printf("Bibliothek konnte nicht geladen werden!\n Leere Bibliothek wird angelegt.\n");
        bib = newEmptyBibliothek();
    }

    //Hauptmenu aufrufen
    main_menu(bib);
}
