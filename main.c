#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"
#include "BuchLibMenu.h"

int test1();
int test2();
int test3();
int test4();



int main() {
    char tempMenuStr[][MAXBUFFERSIZE] = {{"test!"},{"test2"},{""}};


    printf("t:%s\n",tempMenuStr[1]);
    //test3();
    printf("menu: %d", getMenuSelection(tempMenuStr));
    return 0;

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
        buecher[i]->ISBN=(i+1)*12345678910;
        sprintf(buecher[i]->Buchautor, "AutorNr%d!\n",i+1);
        sprintf(buecher[i]->Buchtitel, "TitelNr%d!\n",i+1);
        buecher[i]->AnzahlExemplare=1+i*2;
        addBuch(bib, buecher[i]);
    }
    checkOutBuch(getBuchByIndex(bib,2),ausl[1]->name);
    checkOutBuch(getBuchByIndex(bib,2),ausl[2]->name);
    printBuch(getBuchByIndex(bib, 2));
    printBibliothek(bib);
    saveBib(bib);
    if (freeBib(bib)==0) bib=NULL;
    printf("freebib:%p\n",(void*)bib);
    bib = loadBib();
    printf("(any key to continue)\n");
    //getc(stdin);
    if (!saveBib(bib)) printf("saved\n");
    printBibliothek(bib);
    freeBib(bib);

    //printBibliothek(bib);
    return 0;
}

int test3() {
    Bibliothek* bib = newEmptyBibliothek();
    Buch* buecher[3];
    Ausleiher* ausl[3];

    for(int i = 0;i<3; i++){
        buecher[i]=newEmptyBuch();
        ausl[i]=newEmptyAusleiher();
        sprintf(ausl[i]->name,"Ausleiher%d\n",i);
    }

    for(int i = 0;i<3; i++) {
        buecher[i]->ISBN=(i+1)*12345678910;
        sprintf(buecher[i]->Buchautor, "AutorNr%d!\n",i+1);
        sprintf(buecher[i]->Buchtitel, "TitelNr%d!\n",i+1);
        buecher[i]->AnzahlExemplare=1+i*2;
        addBuch(bib, buecher[i]);
    }
    checkOutBuch(getBuchByIndex(bib,1),ausl[1]->name);
    checkOutBuch(getBuchByIndex(bib,2),ausl[0]->name);
    checkOutBuch(getBuchByIndex(bib,2),ausl[2]->name);

    char* suchStr = "";
    //printBibliothek(bib);
    int b = getNextBuchByString(suchStr,bib,0);
    while (b>=0 ){
        printf("-------------\n");
        printf(" %s found in buch:%d\n",suchStr, b);
        printBuch(getBuchByIndex(bib, b));
        b=(getNextBuchByString(suchStr,bib,b+1));
    }
}

int test4(){
char beef[MAXBUFFERSIZE];
strToLower("abcdef ABCDEF",beef);
    printf("%s", beef);
}
