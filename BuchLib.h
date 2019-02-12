#ifndef BUCHLIB_H_INCLUDED
#define BUCHLIB_H_INCLUDED

#include <string.h>

#include "BuchLibSettings.h"
#include "LinkedList.h"
#include "ctype.h"

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
int removeBuch(Bibliothek* bib, int buchIndex);
int checkInBuchByIndex(Buch *buch, int auslIndex);
int checkInBuchByName(Buch *buch, const char *ausleiherName);
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
Buch * getNextBuchByString(char *searchStr, Bibliothek *bib, int *searchIndex);

#endif // BUCHLIB_H_INCLUDED
