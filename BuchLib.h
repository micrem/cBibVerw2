#ifndef BUCHLIB_H_INCLUDED
#define BUCHLIB_H_INCLUDED

#include <string.h>

#include "BuchLibSettings.h"
#include "LinkedList.h"



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
int checkInBuch(Buch *buch, const char* ausleiherName);
int checkOutBuch(Buch *buch, const char* ausleiherName);

int getAusleiherIndexByName(LinkedList* ListeAusleiher, const char* ausleiherName);
int getAusleiherCount(Bibliothek *bib, int buchIndex);

Buch* getBuchByIndex(Bibliothek *bib, int index);

int printBuch(Buch* buch);
int printBibliothek(Bibliothek* bibliothek);

Buch* newEmptyBuch();
Bibliothek* newEmptyBibliothek();
Ausleiher* newEmptyAusleiher();

//findBuch?

#endif // BUCHLIB_H_INCLUDED
