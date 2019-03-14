/**
 * @file BuchLib.h
 * Implementiert Bibliothek und Buecher Listen/Funktionen/Structs auf Basis von LinkedList. Nutzt BuchLibSettings.h fuer
 * Konfiguration. Maximale groesse aller verwendeten Strings wird in der Konfiguration festgelegt.
 *
 */

#ifndef BUCHLIB_H_INCLUDED
#define BUCHLIB_H_INCLUDED

#include <string.h>

#include "BuchLibSettings.h"
#include "LinkedList.h"
#include "ctype.h"

/**
 * @brief Struct dass eine Bibliothek darstellt, besteht aus einer LinkedList um Buecher zu hinterlegen.
 * Groesse der Bibliothek damit dynamisch erweiterbar
 */
struct BibliothekS {
    LinkedList BuecherListe;
};

/**
 * @brief Struct dass einen Ausleiher darstellt, enthäelt Namen des Ausleihers
 */
struct AusleiherS {
    char name[MAXBUFFERSIZE];
};

/**
 * @brief Struct dass ein Buch darstellt, mit ISBN als long long int,
 * Titel und Author als Strings, Anzahl der Exemplare und einer LinkedList die Ausleiher enthaelt
 */
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

int addBuch(Bibliothek* bib, Buch* buch);
int removeBuch(Bibliothek* bib, int buchIndex);
int checkInBuchByIndex(Buch *buch, int auslIndex);
int checkOutBuch(Buch *buch, const char* ausleiherName);

//unused //int getAusleiherIndexByName(LinkedList* ListeAusleiher, const char* ausleiherName);
int getAusleiherCount(Bibliothek *bib, int buchIndex);

Buch* getBuchByIndex(Bibliothek *bib, int index);


int freeBib(Bibliothek* bib);
int freeBuch(Bibliothek* bib, int buchIndex);


int printBuch(Buch* buch);
int printBibliothek(Bibliothek* bibliothek);

Buch* newEmptyBuch();
Bibliothek* newEmptyBibliothek();
Ausleiher* newEmptyAusleiher();

int strToLower(const char* strIn, char* strOut);
Buch * getNextBuchByString(char *searchStr, Bibliothek *bib, int *searchIndex);

#endif // BUCHLIB_H_INCLUDED
