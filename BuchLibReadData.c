//
// Created by user64 on 23/12/2018.
//

#include "BuchLibReadData.h"
#include <limits.h>
#include <errno.h>

int readLine(char *buf, FILE* inputStream) {
    int charBuffer;

    //liesst maximal MAXBUFFERSIZE-1 chars ein (inklusive '\n') und terminiert Buffer mit '\0'
    //(also MAXBUFFERSIZE-2 verwertbare chars)
    if (fgets(buf, MAXBUFFERSIZE, inputStream) == NULL) {
        if(DEBUG_MODE) printf("readLine: fgets Fehler\n");
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
    char buffer[FILENAME_MAX];
    char* lastChar=NULL;
    if (longlongPtr==NULL) {if (DEBUG_MODE) printf("readInputNumber Fehler: konnte Zahl nicht einlesen, pointer-parameter=NULL!\n"); return BIBL_ERROR;}
    if (str==NULL || *str=='\0') {if (DEBUG_MODE) printf("readInputNumber Fehler: konnte Zahl nicht einlesen, Parameterfehler!\n"); return BIBL_ERROR;}
    if (minVal>maxVal){if (DEBUG_MODE) printf("readInputNumber Fehler: konnte Zahl nicht einlesen, Wertebereich falsch definiert!\n"); return BIBL_ERROR;}
    for(i=0; i<MAXBUFFERSIZE;i++) if(str[i]=='\0') break;
    if (i==MAXBUFFERSIZE) {if (DEBUG_MODE) printf("readInputNumber Fehler: konnte Zahl nicht einlesen, String nicht terminiert!\n"); return BIBL_ERROR;}
    errno=0;
    *longlongPtr = strtoll(buffer, &lastChar, 10);
    if (lastChar != buffer+strlen(buffer)) {
        if (DEBUG_MODE) printf("Fehler: konnte Zahl nicht einlesen\n");
        return BIBL_ERROR;
    }
    if (DEBUG_MODE>1) printf("(Eingabe:%lld)\n", *longlongPtr);
    if (DEBUG_MODE>1) printf("(errno:%d)\n", errno);
    if ( (*longlongPtr < minVal || *longlongPtr > maxVal)
        || (errno != 0 && *longlongPtr == 0)
        || (*longlongPtr>=LLONG_MAX || *longlongPtr <= LLONG_MIN) )
    {
        if (DEBUG_MODE) printf("Fehler: konnte Zahl nicht einlesen! (Wertebereich: %lld-%lld)\n",minVal,maxVal);
        return BIBL_ERROR;
    }
    return BIBL_SUCCESS;
}

int readInputInteger(int *intPtr) {
    long long tempLL;
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readInputInteger Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readNumber(&tempLL, buf, INT_MIN, INT_MAX);
    if (ret==BIBL_SUCCESS) *intPtr  = (int) tempLL;
    return ret;
}

int readInputISBN(long long *longlongPtr) {
    char buf[MAXBUFFERSIZE];
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readInputISBN Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    int ret = readNumber(longlongPtr,buf, 0,9999999999999);
    return ret;
}

int readInputString(char *buf) {
    int ret = readLine(buf, stdin);
    if (DEBUG_MODE) printf("(Eingabe:%s)\n", buf);
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



