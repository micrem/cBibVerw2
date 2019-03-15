/**
 * @file BuchLibSettings.h
 * Einstellungen fuer gesamte Bibliothek-Verwaltung.
 * Legt fest: Fehler-Konstanten, String-Laenge, DebugModus, Name der Datei in der Bibliothek gespeichert wird
 */

#ifndef BUCHLIBSETTINGS_H_INCLUDED
#define BUCHLIBSETTINGS_H_INCLUDED

// Maximale Laenge fuer Stringbuffer
#define MAXBUFFERSIZE 255

// Debugmodus
// 0 = keine Debug-Nachrichten
// 1 = normal
// 2 = verbos
#define DEBUG_MODE 1

// Fehlercodes
#define BIBL_SEVERE (-2)
#define BIBL_ERROR (-1)
#define BIBL_SUCCESS (0)

#define SAVEFILENAME "bibSave.bib"

#endif //BUCHLIBSETTINGS_H_INCLUDED
