#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"
#include "BuchLibMenu.h"

int main() {
    Bibliothek* bib;
    bib=loadBib();
    if (bib==NULL){
        printf("Bibliothek konnte nicht geladen werden!\n Leere Bibliothek wird angelegt.\n");
        bib = newEmptyBibliothek();
    }
    printf("--------------------------\n");
    printf("*  Bibliothekverwaltung  *\n");
    printf("--------------------------\n");
    main_menu(bib);
}
