/**
 * @file BuchLibReadData.c Implementiert Funktionen fuer Einlesen von Daten aus Benutzereingaben und Dateien
 */

#include "BuchLibReadData.h"
#include <limits.h>
#include <errno.h>
/**
 * @brief liesst maximal MAXBUFFERSIZE-1 Chars ein (inklusive '\n') und terminiert Buffer mit '\0'
 *
 * Erreichen von EOF wird als Erfolg gewertet/wiedergegeben!
 * erwartet dass jede Zeile mit '\n' abschliesst und leert Eingabebuffer bei zu langen Eingaben.
 * Wird von allen anderen Funktionen fuer Benutzereingaben aufgerufen.
 *
 * @param buf speichert Ergebniss
 * @param inputStream Quelle fuer Text
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readLine(char *buf, FILE* inputStream) {
    int charBuffer;

    //liesst maximal MAXBUFFERSIZE-1 chars ein (inklusive '\n') und terminiert Buffer mit '\0'
    //(also MAXBUFFERSIZE-2 verwertbare chars)
    if (fgets(buf, MAXBUFFERSIZE, inputStream) == NULL) {
        if (feof(inputStream)) return BIBL_SUCCESS;
        if(DEBUG_MODE ) printf("readLine: Fehler beim Aufruf von fgets, konnte Zeile nicht einlesen!\n");
        return BIBL_ERROR; //keine Eingabe
    }
    //pruefen auf "wide" oder UTF8 chars
    for(int i=0;i<MAXBUFFERSIZE;i++){
        if(buf[i]=='\0') break; //String bis Ende scannen
        if(buf[i]<0){ //falls Char negativen Wert hat, Fehler melden und abbrechen
            if(DEBUG_MODE) printf("readLine: falscher ASCII-Wert in Eingabe! c:'%c' d:'%d'\n",buf[i],buf[i]);
            //if(DEBUG_MODE) printf("readLine: falscher ASCII-Wert in Eingabe!\n");
            return BIBL_ERROR;
        }
    }
    // Falls Eingabe zu lang (=vorletztes Zeichen im eingelesenen String nicht '\n'), Reste aus Eingabebuffer leeren
    if (buf[strlen(buf)-1] != '\n') {
        if (DEBUG_MODE) printf("readLine:buffer nicht leer\n");
        while ( ((charBuffer = fgetc(inputStream)) != '\n')  && (charBuffer != EOF)) {
            if (DEBUG_MODE>1) printf("readLine:leere buffer:%d\n",charBuffer);
        }
        if (DEBUG_MODE>1) printf("readLine:buffer geleert\n");
        return BIBL_ERROR;
    }
    return BIBL_SUCCESS;
}

/**
 * Liesst eine Nummer im Format long long int aus uebergebenem String aus.
 * Maximale / Minimale Eingabewerte werden mit Parametern festgelegt.
 * Wird von allen anderen Funktionen fuer Zahleinlesen verwendet
 * @param longlongPtr Zeiger auf long long int in der das Ergebniss abgelegt wird
 * @param str String aus dem die Zahl ausgelesen werden soll
 * @param minVal minimaler akzeptierter Eingabewert
 * @param maxVal maximaler akzeptierter Eingabewert
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readNumber( long long* longlongPtr, const char* str, long long minVal, long long maxVal) {
    int i;
    char* lastCharPtr=NULL;

    //Fehlerchecks
    if (longlongPtr==NULL) {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, pointer-parameter=NULL!\n"); return BIBL_ERROR;}
    if (str==NULL || *str=='\0') {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, Parameterfehler!\n"); return BIBL_ERROR;}
    if (minVal>maxVal){if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, Bereich fuer Grenzwerte falsch definiert!\n"); return BIBL_ERROR;}
    for(i=0; i<MAXBUFFERSIZE;i++) if(str[i]=='\0') break; //Stelle suchen wo String terminiert wird, bis maximal an Stelle MAXBUFFERSIZE
    if (i==MAXBUFFERSIZE) {if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen, String nicht terminiert!\n"); return BIBL_ERROR;}
    errno=0; //errno ist globale Variable, wird auf Null gesetzt um neue Fehler in strtoll() bemerken zu koennen

    *longlongPtr = strtoll(str, &lastCharPtr, 10); //Auslesen der Zahl (in base 10)

    //Fehlerchecks
    if (lastCharPtr != &str[strlen(str)-1]) {
        if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl in Eingabe nicht eindeutig erkennen\n");
        return BIBL_ERROR;
    }
    //Verbose Debug Nachrichten
    if (DEBUG_MODE>1) printf("(Eingabe:%lld)\n", *longlongPtr);
    if (DEBUG_MODE>1) printf("(errno:%d)\n", errno);
    if ( (*longlongPtr < minVal || *longlongPtr > maxVal) //falls ausserhalb Definitionsbereich
        || (errno != 0) )//oder Fehler beim Einlesen bzw. "out-of-bounds"
    {
        if (DEBUG_MODE) printf("readNumber Fehler: konnte Zahl nicht einlesen! (Wertebereich: %lld bis %lld)\n",minVal,maxVal);
        return BIBL_ERROR;
    }
    return BIBL_SUCCESS;
}

/**
 * Integer vom Benutzer einlesen
 * @param intPtr Pointer auf Integer in den Ergebniss geschrieben wird
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readIntegerInput(int *intPtr) {
    char buf[MAXBUFFERSIZE];
    //String einlesen
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readIntegerInput Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    //Zahl aus String auslesen
    int ret = readInteger(intPtr, buf);
    return ret; //Fehler aus readInteger() weiterreichen
}

/**
 * ISBN vom Benutzer einlesen
 * @param longlongPtr Pointer auf long long int in den die ISBN als Zahl geschrieben wird
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readISBNInput(long long *longlongPtr) {
    char buf[MAXBUFFERSIZE];
    //String einlesen
    if (readLine(buf,stdin)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readISBNInput Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    //Zahl aus String auslesen
    int ret = readISBN(longlongPtr,buf);
    return ret; //Fehler weiterreichen
}

/**
 * String vom Benutzer einlesen
 * @param buf Pointer auf String in den Eingabe geschrieben wird
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readStringInput(char *buf) {
    int ret = readLine(buf, stdin); //String einlesen aus stdin
    if (DEBUG_MODE>1) printf("(Eingabe:%s)\n", buf);
    return ret; //Fehler weiterreichen
}

/**
 * Liesst ISBN aus String \p str in \p longlongPtr ein.
 * Benutzt readNumber() mit Wertebereich 0-9.999.999.999.999 (13 Stellen)
 * @param longlongPtr Speichert Ausgabe
 * @param str String aus dem Zahl gelesen wird
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readISBN(long long *longlongPtr, char *str) {
    int ret = readNumber(longlongPtr,str, 0,9999999999999);
    return ret; //Fehler weiterreichen
}

/**
 * Liesst aus String \p str einen Integer nach \p intPtr ein.
 * Benutzt readNumber() mit Wertebereich INT_MIN bis INT_MAX
 * @param intPtr Speichert Ausgabe
 * @param str String aus dem Zahl gelesen wird
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readInteger(int *intPtr, char *str) {
    long long tempLL;
    int ret = readNumber(&tempLL, str, INT_MIN, INT_MAX);
    if (ret==BIBL_SUCCESS) *intPtr  = (int) tempLL; //Cast nur wenn keine Fehler beim Einlesen
    return ret; //Fehler weiterreichen
}

/**
 * Liesst naechste Zeile aus Datei als String in \p buf ein.
 * Stellt sicher dass \p buf mit '\n' abschliesst.
 * Nutzt readLine() mit Dateipointer \p fp als Parameter.
 * @param fp Datei aus der Zeile ausgelesen wird
 * @param buf Speichert Ausgabe
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readStringFile(FILE* fp, char* buf){
    int ret = readLine(buf, fp); //String mit readLine() aus Datei einlesen
    int len = 0;
    if (ret==0 && DEBUG_MODE>1) printf("(Eingabe:%s)\n", buf);
    if (ret) return ret;
    len = (int) strlen(buf);
    if(len<1 || len>=MAXBUFFERSIZE) {if (DEBUG_MODE) printf("Fehler, konnte String nicht aus Datei lesen!\n");return BIBL_ERROR;}
    if (DEBUG_MODE>1) printf("String aus Datei eingelesen: %s", buf);
    if (buf[len-1]!='\n') { //Letztes Zeichen im String auf NewLine setzen
        if (DEBUG_MODE>1) printf("Letztes Zeichen im String nicht \\n, wird ersetzt '%s'", buf);
        buf[len-1]='\n';
    }
    return BIBL_SUCCESS;
}

/**
 *
 * @param fp Datei aus der Zeile ausgelesen wird
 * @param longlongPtr Speichert Ausgabe
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readISBNFile(FILE* fp, long long *longlongPtr){
    char buf[MAXBUFFERSIZE];
    //Zeile einlesen
    if (readLine(buf,fp)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readISBNFile Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    //Zahl aus Zeile auslesen
    int ret = readISBN(longlongPtr,buf);
    return ret; //Fehler weiterreichen
}

/**
 *
 * @param fp Datei aus der Zeile ausgelesen wird
 * @param intPtr Speichert Ausgabe
 * @return BIBL_ERROR(<0) oder BIBL_SUCCESS(=0)
 */
int readIntegerFile(FILE* fp, int* intPtr){
    char buf[MAXBUFFERSIZE];
    //Zeile einlesen
    if (readLine(buf,fp)!=BIBL_SUCCESS) {if (DEBUG_MODE) printf("readIntegerFile Fehler: konnte String nicht einlesen!\n"); return BIBL_ERROR;}
    //Zahl aus Zeile auslesen
    int ret = readInteger(intPtr, buf);
    return ret; //Fehler weiterreichen
}


