/**
 * @file Funktionen zur Menuverwaltung
 */

#ifndef BUCHLIBMENU_H_INCLUDED
#define BUCHLIBMENU_H_INCLUDED

#include "BuchLib.h"
#include "BuchLibReadData.h"
#include "BuchLibLoadSave.h"

int getMenuSelection(char menu_text[][MAXBUFFERSIZE]);

extern char main_menu_text[][MAXBUFFERSIZE];

extern char menu1_text[][MAXBUFFERSIZE];

extern char menu2_text[][MAXBUFFERSIZE];


#endif //BUCHLIBMENU_H_INCLUDED

