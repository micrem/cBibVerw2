//
// Created by user64 on 23/12/2018.
//

#ifndef READDATA_H_INCLUDED
#define READDATA_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "BuchLib.h"

int readStringInput(char *buf);
int readISBNInput(long long *longlongPtr);
int readIntegerInput(int *intPtr);

int readISBN( long long* longlongPtr, char *str);
int readInteger(int *intPtr, char* str);

int readStringFile(FILE* fp, char* buf);
int readISBNFile(FILE* fp, long long *longlongPtr);
int readIntegerFile(FILE* fp, int* intPtr);

int readLine(char *buf, FILE* inputStream);


#endif //READDATA_H_INCLUDED
