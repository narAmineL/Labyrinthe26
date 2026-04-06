#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "node.h"
#include "labyrinthe.h"

//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"




int main(void) {


    connectToServer("vps-1a2cee88.vps.ovh.net", 1234, "narAmyn"); //se connecter au jeu.
    
    vector2i labSize = {.x=0, .y=0};
    char labName[50];

    waitForLabyrinth("TRAINING DONTMOVE start=1", labName, (int*)&labSize.x, (int*)&labSize.y);

    t_node** lab = createLabyrinth(labSize); //création du labyrinthe.

    int isOpponentStarting;
    t_node extraTile;
    fillLabyrinth(lab, labSize, &isOpponentStarting, &extraTile); //le remplir selon getLabData de l'api et renvoyer l'extra tile.
    

    printLabyrinth();

    return EXIT_SUCCESS;
}

