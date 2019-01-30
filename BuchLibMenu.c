#include "BuchLibMenu.h"

int open_main_menu(){

}

/**
 * testComment
 * @param menu_text
 * @return
 */
int getMenuSelection(char menu_text[][MAXBUFFERSIZE]){
    int i=0;
    int menuSelection=-1;
    while (menu_text[i][0]!='\0'){
        printf("%d] %s\n",i+1,menu_text[i]);
        i++;
    }
    printf("0] Abbrechen\n");
    do{
        printf("(Bitte Nummer(0-%d) eingeben und mit 'Enter' bestaetigen)>",i);
        readIntegerInput(&menuSelection);
        printf("\n");
    }while (menuSelection<0 || menuSelection>i);
    return menuSelection;
}


char main_menu_text[][MAXBUFFERSIZE]= {
        {"main menu1"},
        {"main menu option 2"},
        {"main menu third"},
        {""}
};

char menu1_text[][MAXBUFFERSIZE]= {
        {"menu11"},
        {"menu12"},
        {"menu13"},
        {""}
};

char menu2_text[][MAXBUFFERSIZE]= {
        {"menu21"},
        {"menu22"},
        {"menu23"},
        {""}
};
