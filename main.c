#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"

int test1();
int test2();
int test3();
int test4();



int main() {

    test2();

    return 0;


/*

Bibliothek* bib = newEmptyBibliothek();
Buch* buecher[3];
Ausleiher* ausl[3];

for(int i = 0;i<3; i++){
    buecher[i]=newEmptyBuch();
    ausl[i]=newEmptyAusleiher();
}

for(int i = 0;i<3; i++) {
    buecher[i]->ISBN=i*12345;
    sprintf(buecher[i]->Buchautor, "AutorNr%d!",i+1);
    sprintf(buecher[i]->Buchtitel, "TitelNr%d!",i+1);
    buecher[i]->AnzahlExemplare=1+i*2;
    addBuch(bib, buecher[i]);
}

printBibliothek(bib);

printf("checkOutBuch %d\n", checkOutBuch(getListData(&(bib->BuecherListe),1),"Salahaddin"));

printBuch(getListData(&(bib->BuecherListe),1));

printf("checkInBuch %d\n",checkInBuch( getListData(&(bib->BuecherListe),1),"Salahaddin"));

printBuch(getListData(&(bib->BuecherListe),1));

do {
    int input_menu;
    printf("\n1) buch hinzufuegen \n2) buch ausleihen \n3) buch zurueckgeben \n4) bibliothek ausdrucken\n5) beenden\n");
    printf("Bitte menupunkt auswaehlen: ");
    input_menu = readInputInteger(NULL);
    switch (input_menu){
        case 1:{
            Buch* newBuch = newEmptyBuch();
            char buffer[MAXBUFFERSIZE];
            printf("\nBitte Titel eingeben: ");
            readInputString(buffer);
            strcpy(newBuch->Buchtitel, buffer );
            printf("\nBitte Autor eingeben: ");
            readInputString(buffer);
            strcpy(newBuch->Buchautor, buffer);
            printf("\nBitte ISBN eingeben: ");
            newBuch->ISBN= readISBNInput(0);
            printf("\nBitte Exemplaranzahl eingeben: ");
            newBuch->AnzahlExemplare= readInputInteger(NULL);
            printf(addBuch(bib, newBuch) ? "\nFehler beim Buch Anlegen!" : "Buch angelegt!\n");
            break;}
        case 2:{
            printf("\nBitte Buchindex zum Ausleihen eingeben: ");
            Buch* buch = getListData(&(bib->BuecherListe),(int) readInputInteger(NULL));
            printBuch(buch);
            printf("\nBitte Ausleihernamen eingeben: ");
            char buffer[MAXBUFFERSIZE];
            readInputString(buffer);
            printf(checkOutBuch( buch, buffer)?"\nFehler beim Ausleihen!": "Buch ausgeliehen!\n");}
            break;
        case 3:
        {   printf("\nBitte Buchindex zum Wiedergeben eingeben: ");
            Buch* buch = getListData(&(bib->BuecherListe),(int) readIntegerInput(NULL));
            printBuch(buch);
            printf("\nBitte Ausleihernamen eingeben: ");
            char buffer[MAXBUFFERSIZE];
            readStringInput(buffer);
            printf(checkInBuch( buch, buffer)?"\nFehler beim Wiedergeben!": "Buch wiedergegeben!\n");}
            break;
        case 4:
            printf("Bibliothek:\n");
            printBibliothek(bib);
            break;
        case 5:
            return 0;


        default:
            printf("Auswahl nicht erkannt\n");
            continue;
    }
} while ("false");

*/

/*
    LinkedList list = {NULL, 0};
    LinkedList *listPtr = &list;

    int *data1 = malloc(sizeof(int));
    int *data2 = malloc(sizeof(int));
    int *data3 = malloc(sizeof(int));
    *data1=11;
    *data2=22;
    *data3=33;

    printf("%d\n",sizeof(LLNode));

    printf("addListItem:%d\n",addListItem( listPtr, data1 ));
    printf("addListItem:%d\n",addListItem( listPtr, data2 ));
    printf("addListItem:%d\n",addListItem( listPtr, data3 ));
    printf("\n");

    for (int i=0; i< list.length;i++ ){
        printf("%d:%d\n",i,*(int*)getListData(listPtr,i));
    };
*/

}


int test1() {
    char buf[MAXBUFFERSIZE]={0};
    long long  ll;
    int ret;

    printf(">");
    ret = readISBNInput(&ll);
    printf("main return val:%d\n",ret);
    !ret ? printf("input:%lld\n",ll): printf("error, main return val:%d\n",ret);
    printf(">");
    !(ret= readStringInput(buf)) ? printf("input:%s, length: %ld\n",buf, strlen(buf)): printf("error, main return val:%d\n",ret);

    printf("input: ");
    for(int i=0;i<MAXBUFFERSIZE;i++){
        printf(" '%c' (%d) ", buf [i],buf [i]);
    }
    printf("\n");
    printf("zum Beenden (ENTER) druecken");
    getchar();
    return 0;
}

int test2() {
    Bibliothek* bib = newEmptyBibliothek();
    Buch* buecher[3];
    Ausleiher* ausl[3];

    for(int i = 0;i<3; i++){
        buecher[i]=newEmptyBuch();
        ausl[i]=newEmptyAusleiher();
        sprintf(ausl[i]->name,"Ausleiher%d\n",i);
    }

    for(int i = 0;i<3; i++) {
        buecher[i]->ISBN=i*12345678910;
        sprintf(buecher[i]->Buchautor, "AutorNr%d!\n",i+1);
        sprintf(buecher[i]->Buchtitel, "TitelNr%d!\n",i+1);
        buecher[i]->AnzahlExemplare=1+i*2;
        addBuch(bib, buecher[i]);
    }
    checkOutBuch(getBuchByIndex(bib,2),ausl[1]->name);
    checkOutBuch(getBuchByIndex(bib,2),ausl[2]->name);
    printBuch(getBuchByIndex(bib, 2));
    saveBib(bib);
    printf("freebib:%d\n",freeBib(bib));
    loadBib();
    return 0;
}

int test3() {
    Bibliothek* bib=loadBib();
    printBibliothek(bib);
    long long ll=99;
    int a=0;
    readISBN(&ll, "123123123123456");
    printf("%lld\n",ll);
}

int test4(){
    int i=0;
    char buf[MAXBUFFERSIZE]={0};
    readStringInput(buf);
    return 0;
}
