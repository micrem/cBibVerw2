//
// Created by user64 on 23/12/2018.
//

#ifndef READDATA_H_INCLUDED
#define READDATA_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BuchLib.h"

int readInputString(char *buf);
int readInputISBN(long long *longlongPtr);
int readInputInteger(int *intPtr);
int readLine(char *buf, FILE* inputStream);


int readISBN( long long* longlongPtr, char *str);
int readInteger(int *intPtr, char* str);


#endif //READDATA_H_INCLUDED
