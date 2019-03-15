/**
 * @file BuchLobMenu.h Funktionen zur Menuverwaltung
 */

#ifndef BUCHLIBMENU_H_INCLUDED
#define BUCHLIBMENU_H_INCLUDED

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"

int getMenuSelection(char menu_text[][MAXBUFFERSIZE]);

void main_menu(Bibliothek* bib);
void menu1_auswahl(Bibliothek* bib);
void menu12_ausleihen(Buch* buch);
void menu13_zurueckgeben(Buch* buch);
void menu14_bearbeiten(Buch* buch);
void menu2_buchneu(Bibliothek* bib);
int menu_confirm();


#endif //BUCHLIBMENU_H_INCLUDED

