//
// Created by user64 on 02/01/2019.
//

#ifndef LOADSAVE_H_INCLUDED
#define LOADSAVE_H_INCLUDED

#include <stdio.h>
#include "BuchLib.h"
#include "BuchLibReadData.h"

int saveBib(Bibliothek *bib);
Bibliothek* loadBib();

#endif //LOADSAVE_H_INCLUDED
