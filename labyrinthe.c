#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"

#include "vector2.h"
#include "labyrinthe.h"
#include "node.h"
#include "player.h"

//fonction qui rend un pointeur qui donne vers une matrice2D (sizeX*sizeY) allouée de nodes (cf node.h)
t_node** createLabyrinth(vector2i labSize) {
    int sizeX, sizeY;
    sizeX = labSize.x; sizeY = labSize.y;

    t_node** labyrinth = malloc(sizeof(t_node*)*sizeY);
    
    for(int i=0; i<sizeY; i++) {
        labyrinth[i] = malloc(sizeof(t_node)*sizeX);
    }
    return labyrinth;
}


//fct qui désalloue la mémoire d'un labyrinthe
void destroyLabyrinth(t_node** labyrinth, vector2i labSize) {
    for(int i=0; i<labSize.y; i++) {
        free(labyrinth[i]);
    }
    free(labyrinth);
}


//copie le contenu du labyrinthe from dans le lab to. les 2 doivent avoir la même taille.
void copyLabyrinth(t_node** from, t_node** to, vector2i labSize) {
    int sizeX=labSize.x, sizeY=labSize.y;

    for(int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            to[i][j] = from[i][j];
        }
    }
}

//Renvoie l'extraTile, qui peut être replacée selon les règles de labyrinthe.
//modifie la valeur de isOppStarting (cf getLabyrinth) et remplit extraTile
void fillLabyrinth(t_node** lab, vector2i labSize, int* isOpponentStarting, t_node* extraTile) {


    int sizeX = labSize.x, sizeY = labSize.y;

    char* labyData = malloc( sizeof(char) * ( (sizeX*sizeY+5)*11 + 1) ); //+1 pour le eos
    *isOpponentStarting = getLabyrinth(labyData); //remplit le string labydata des données du lab.


    char* ptr = labyData;
    for(int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            //cf docu 3.1 pour le scan des data.
            int n;

            sscanf(ptr, "%d %d %d %d %d%n", //n=nb de char que j'ai mangé.
            &lab[i][j].NORTH,
            &lab[i][j].EAST,
            &lab[i][j].SOUTH,
            &lab[i][j].WEST,
            &lab[i][j].treasureVal,

            &n
            );

            ptr+=n; //on avance des n char que j'ai mangé.

        }
    }

    sscanf(ptr, "%d %d %d %d %d",
            &extraTile->NORTH,
            &extraTile->EAST,
            &extraTile->SOUTH,
            &extraTile->WEST,
            &extraTile->treasureVal
            );


    free(labyData);
}



//fct qui insère la tile extraTile dans le labyrinth du côté dirToInsert (NORTH=en haut, WEST=à gauche...)
//pas besoin de modifier la valeur de extraTile, elle se trouve dans le msg
//ATTENTION: MODIFIE PAS EXTRA TILE
//s'occupe aussi de déplacer le joueur / l'adversaire avec les tiles.
void insertTile(t_node** lab, vector2i labSize, t_node extraTile, t_insertion insertion, t_player* player, t_player* opponent) {
    int sizeX = labSize.x, sizeY = labSize.y;

    e_direction dirToInsert = insertion.insertDir;
    int insertIndex = insertion.insertIndex;


    //effectuer les rotations de la tuile avant de la rentrer.
    extraTile = getRotatedNode90CW(insertion.nbRotations, extraTile);

    switch(dirToInsert) {

        //|--- INSERER LA CASE EN HAUT ---|
        case NORTH:

            for(int i=sizeY-1; i>0; i--) {
                lab[i][insertIndex] = lab[i-1][insertIndex];
            }
            
            if (insertIndex==player->pos.x) movePlayerOneTile(labSize, player, SOUTH);
            if (insertIndex==opponent->pos.x) movePlayerOneTile(labSize, opponent, SOUTH);

            lab[0][insertIndex] = extraTile;
        break;



        //|--- INSERER LA CASE EN BAS ---|
        case SOUTH:

            for(int i=0; i<sizeY-1; i++) {
                lab[i][insertIndex] = lab[i+1][insertIndex];
            }

            if (insertIndex==player->pos.x) movePlayerOneTile(labSize, player, NORTH);
            if (insertIndex==opponent->pos.x) movePlayerOneTile(labSize, opponent, NORTH);

            lab[sizeY-1][insertIndex] = extraTile;
        break;



        //|--- INSERER LA CASE A GAUCHE ---|
        case WEST:

            for(int j=sizeX-1; j>0; j--) {
                lab[insertIndex][j] = lab[insertIndex][j-1];
            }
            
            if (insertIndex==player->pos.y) movePlayerOneTile(labSize, player, EAST);
            if (insertIndex==opponent->pos.y) movePlayerOneTile(labSize, opponent, EAST);

            lab[insertIndex][0] = extraTile;
        break;



        //|--- INSERER LA CASE A DROITE ---|
        case EAST:

            for(int j=0; j<sizeX-1; j++) {
                lab[insertIndex][j] = lab[insertIndex][j+1];
            }
            
            if (insertIndex==player->pos.y) movePlayerOneTile(labSize, player, WEST);
            if (insertIndex==opponent->pos.y) movePlayerOneTile(labSize, opponent, WEST);

            lab[insertIndex][sizeX-1] = extraTile;
        break;
    }
}




//MODIFIE EXTRA TILE
void computeOpponentMove(t_node** lab, vector2i labSize, t_node* extraTile, char* moveString, char* msg, t_player* player, t_player* opponent) {

    t_insertion insertion;
    vector2i posToReach; //position à atteindre après l'insertion de la tuile.
    
    int insertDirInt; /*direction d'insertion (cf docu pdf)
        •0 pour une insertion d’une ligne `a gauche
        •1 pour une insertion d’une ligne `a droite
        •2 pour une insertion d’une colonne en haut
        •3 pour une insertion d’une colonne en bas
    */

    sscanf(moveString, "%d %d %d %d %d", //n=nb de char que j'ai mangé.
            &insertDirInt,
            &insertion.insertIndex, //Indice de la ligne ou de la colonne où on insert la tuile
            &insertion.nbRotations, //Nombre de rotation de la tuile avant de l’insérer (de 0 à 3 quart de tour dans le sens horaire)
            &posToReach.x,
            &posToReach.y
            );

    switch (insertDirInt) { //DETERMINER LA DIRECTION D'ENTREE
        case 0: insertion.insertDir = WEST; break;
        case 1: insertion.insertDir = EAST; break; 
        case 2: insertion.insertDir = NORTH; break;
        case 3: insertion.insertDir = SOUTH; break;
    }

    //scanner le message et remplir extraTile et le prochain trésor de l'opponent.
    sscanf(msg, "%d %d %d %d %d %d", //n=nb de char que j'ai mangé.
            &extraTile->NORTH,
            &extraTile->EAST,
            &extraTile->SOUTH,
            &extraTile->WEST,
            &extraTile->treasureVal,

            &opponent->nextTreasureVal
            );

    printf("L'ADVERSAIRE A JOUE:");
    printInsertion(insertion);
    insertTile(lab, labSize, *extraTile, insertion, player, opponent); //INSERER LA TUILE
    opponent->pos = posToReach; //déplacer l'adversaire où il doit aller.
}


void myPrintLabyrinth(t_node** lab, t_node extraTile, vector2i labSize, t_player* player, t_player* opponent) {
    
    int sizeX = labSize.x, sizeY = labSize.y;

    //CREER LA MATRICE D'AFFICHAGE
    char** dispMatrix = malloc(sizeof(char*)* (sizeY*2+1) );
    for(int i=0; i<sizeY*2+1; i++) dispMatrix[i] = malloc(sizeof(char) * (sizeX*3+1) );
    
    /* de la forme:

    +--+--+--+
    |13|  |  |
    +--+--+--+
    |  |01|  |
    +--+--+--+
    
    */

    //|---FAIRE LE QUADRILLAGE ---|
    for(int i=0; i<sizeY*2+1; i++) {
        for(int j=0; j<sizeX*3+1; j++) {
            if (j%3==0 && i%2==0) dispMatrix[i][j]='+';
            else dispMatrix[i][j]=' ';
        }   
    }

    //|---ENTRER LES INFOS DES TUILES
    for(int i=0; i<sizeY; i++) {
        for(int j=0; j<sizeX; j++) {
            if (lab[i][j].NORTH==IS_WALL) {
                dispMatrix[i*2][j*3+1] = '-';
                dispMatrix[i*2][j*3+2] = '-';
            }
            if (lab[i][j].SOUTH==IS_WALL) {
                dispMatrix[i*2+2][j*3+1] = '-';
                dispMatrix[i*2+2][j*3+2] = '-';
            }
            if (lab[i][j].EAST==IS_WALL) {
                dispMatrix[i*2+1][j*3+3] = '|';
            }
            if (lab[i][j].WEST==IS_WALL) {
                dispMatrix[i*2+1][j*3] = '|';
            }

            if (lab[i][j].treasureVal > 0) {
                dispMatrix[i*2+1][j*3+1] = (char)('0' + lab[i][j].treasureVal/10);
                dispMatrix[i*2+1][j*3+2] = (char)('0' + lab[i][j].treasureVal%10);
            }
        }
    }



    for(int i=0; i<sizeY*2+1; i++) {
        for(int j=0; j<sizeX*3+1; j++) {
            printf("%c", dispMatrix[i][j]);
        }
    printf("\n");
    }

    printf("EXTRA TILE:\n");
    printNode(extraTile);

}