#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"
#include "BuchLibMenu.h"

int main() {
    char test[MAXBUFFERSIZE];
    readLine(test,stdin);
    for(int i=0;i<strlen(test);i++) printf("%d %c\n",test[i],test[i]);
    Bibliothek* bib;
    bib=loadBib();
    printf("--------------------------\n");
    printf("*  Bibliothekverwaltung  *\n");
    printf("--------------------------\n");
    main_menu(bib);
}
