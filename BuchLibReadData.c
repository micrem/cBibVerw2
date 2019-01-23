//
// Created by user64 on 23/12/2018.
//

#include "BuchLibReadData.h"
#include <limits.h>
#include <errno.h>

//returns success on EOF!
//expects \n in every line!
int readLine(char *buf, FILE* inputStream) {
    int charBuffer;

    //liesst maximal MAXBUFFERSIZE-1 chars ein (inklusive '\n') und terminiert Buffer mit '\0'
    //(also MAXBUFFERSIZE-2 verwertbare chars)
    if (fgets(buf, MAXBUFFERSIZE, inputStream) == NULL) {
        if (feof(inputStream)) return BIBL_SUCCESS;
        if(DEBUG_MODE ) printf("readLine: Fehler beim Aufruf von fgets, konnte Zeile nicht einlesen!\n");
        return BIBL_ERROR; //keine Eingabe
    }
    // Falls Eingabe zu lang (=vorletztes Zeichen nicht '\n'), Eingabebuffer leeren
    if (buf[strlen(buf)-1] != '\n') {
        if (DEBUG_MODE) printf("readLine:buffer nicht leer\n");
        while ( ((charBuffer = fgetc(inputStream)) != '\n')  && (charBuffer != EOF)) {//EOF fuer FILE*, nicht fuer stdin
            if (DEBUG_MODE>1) printf("readLine:leere buffer:%d\n",charBuffer);
        }
        if (DEBUG_MODE>1) printf("readLine:buffer geleert\n");
        return BIBL_ERROR;
    }
    // String zur Sicherheit abschliessen
    buf[strlen(buf)-1] = '\0';
    return BIBL_SUCCESS;
}

int readNumber( long long* longlongPtr, const char* str, long long minVal, long long maxVal) {
    int i;
    char* lastChar=NULL;
    if (longlongPtr==NULL) {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, pointer-parameter=NULL!\n"); return BIBL_ERROR;}
    if (str==NULL || *str=='\0') {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, Parameterfehler!\n"); return BIBL_ERROR;}
    if (minVal>maxVal){if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, Wertebereich falsch definiert!\n"); return BIBL_ERROR;}
    for(i=0; i<MAXBUFFERSIZE;i++) if(str[i]=='\0') break;
    if (i==MAXBUFFERSIZE) {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, String nicht terminiert!\n"); return BIBL_ERROR;}
    errno=0;
    *longlongPtr = strtoll(str, &lastChar, 10);
    if (lastChar != str+strlen(str)) {
        if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen\n");
        return BIBL_ERROR;
    }
    if (DEBUG_MODE>1) printf("(Eingabe:%lld)\n", *longlongPtr);
    if (DEBUG_MODE>1) printf("(errno:%d)\n", errno);
    if ( (*longlongPtr < minVal || *longlongPtr > maxVal)
        || (errno != 0 && *longlongPtr == 0)
        || (*longlongPtr>=LLONG_MAX || *longlongPtr <= LLONG_MIN) )
    {
        if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen! (Wertebereich: %lld bis %lld)\n",minVal,maxVal);
        return BIBL_ERROR;
    }
    return BIBL_SUCCESS;
}

int readIntegerInput(int *intPtr) {
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readIntegerInput Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readInteger(intPtr, buf);
    return ret;
}

int readISBNInput(long long *longlongPtr) {
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readISBNInput Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readISBN(longlongPtr,buf);
    return ret;
}

int readStringInput(char *buf) {
    int ret = readLine(buf, stdin);
    if (DEBUG_MODE>1) printf("(Eingabe:%s)\n", buf);
    return ret;
}

int readISBN(long long *longlongPtr, char *str) {
    int ret = readNumber(longlongPtr,str, 0,9999999999999);
    return ret;
}

int readInteger(int *intPtr, char *str) {
    long long tempLL;
    int ret = readNumber(&tempLL, str, INT_MIN, INT_MAX);
    if (ret==BIBL_SUCCESS) *intPtr  = (int) tempLL;
    return ret;
}

int readStringFile(FILE* fp, char* buf){
    int ret = readLine(buf, fp);
    if (DEBUG_MODE>1) printf("(Eingabe:%s)\n", buf);
    return ret;
}

int readISBNFile(FILE* fp, long long *longlongPtr){
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,fp)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readISBNFile Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readISBN(longlongPtr,buf);
    return ret;
}

int readIntegerFile(FILE* fp, int* intPtr){
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,fp)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readIntegerFile Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readInteger(intPtr, buf);
    return ret;
}


