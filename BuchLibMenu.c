
#include "BuchLibMenu.h"

int open_menu(){
    // main menu loop
    //switch selection or close

}

/**
 * testComment
 * @param text
 * @return
 */
int getMenuSelection(char text[][MAXBUFFERSIZE]){
    int i=0;
    int menuSelection=-1;
    while (text[i][0]!='\0'){
        printf("%d] %s\n",i+1,text[i]);
        i++;
    }
    printf("0] Abbrechen\n");
    do{
        printf("(Bitte Nummer(0-%d) eingeben und mit 'Enter' bestaetigen)>",i);
        readIntegerInput(&menuSelection);
    }while (menuSelection<0);
    return menuSelection;
}
