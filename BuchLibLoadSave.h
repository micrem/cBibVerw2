/**
 * @file BuchLibLoadSave.h
 * Header fuer Implementierung von Lade- und Speicherfunktionen einer Bibliothek in eine Datei
 */

#ifndef LOADSAVE_H_INCLUDED
#define LOADSAVE_H_INCLUDED

#include <stdio.h>
#include "BuchLib.h"
#include "BuchLibReadData.h"

int saveBib(Bibliothek *bib);
Bibliothek* loadBib();

#endif //LOADSAVE_H_INCLUDED
