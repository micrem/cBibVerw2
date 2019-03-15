/**
 * @file BuchLibLoadSave.h
 * Header fuer Implementierung Bibliothek Laden aus Datei und Speicher in Datei
 */

#ifndef LOADSAVE_H_INCLUDED
#define LOADSAVE_H_INCLUDED

#include <stdio.h>
#include "BuchLib.h"
#include "BuchLibReadData.h"

int saveBib(Bibliothek *bib);
Bibliothek* loadBib();

#endif //LOADSAVE_H_INCLUDED
