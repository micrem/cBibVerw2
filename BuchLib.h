#ifndef BUCHLIB_H_INCLUDED
#define BUCHLIB_H_INCLUDED

#include <string.h>

#include "BuchLibSettings.h"
#include "LinkedList.h"
#include "ctype.h"
#include "BuchLibReadData.h"



struct BibliothekS {
    LinkedList BuecherListe;
};

struct AusleiherS {
    char name[MAXBUFFERSIZE];
};

struct BuchS {
    long long ISBN;
    char Buchtitel[MAXBUFFERSIZE];
    char Buchautor[MAXBUFFERSIZE];
    int AnzahlExemplare;
    LinkedList ListeAusleiher;
};

typedef struct BuchS Buch;

typedef struct BibliothekS Bibliothek;

typedef struct AusleiherS Ausleiher;

int addBuch(Bibliothek* bibliothek, Buch* buch);
int removeBuch(Bibliothek* bibliothek, int index);
int checkInBuch(Buch *buch, const char* ausleiherName);
int checkOutBuch(Buch *buch, const char* ausleiherName);

int getAusleiherIndexByName(LinkedList* ListeAusleiher, const char* ausleiherName);
int getAusleiherCount(Bibliothek *bib, int buchIndex);

Buch* getBuchByIndex(Bibliothek *bib, int index);


int freeBib(Bibliothek* bib);
int freeBuch(Bibliothek* bib, int index);
// int freeAusleiher(Ausleiher* ausl);



int printBuch(Buch* buch);
int printBibliothek(Bibliothek* bibliothek);

Buch* newEmptyBuch();
Bibliothek* newEmptyBibliothek();
Ausleiher* newEmptyAusleiher();

int strToLower(const char* strIn, char* strOut);
//findBuch?
int getNextBuchByString(char* searchStr, Bibliothek* bib, int startIndex);

#endif // BUCHLIB_H_INCLUDED
