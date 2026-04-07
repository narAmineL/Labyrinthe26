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

//fonction qui renvoie la position du prochain trésor qu'on doit atteindre.
//direction = +1 -> sens croissant, direction=-1 -> sens decroissant
//si problème ou prochain trésor inexistant, renvoie le vecteur {-1, -1}.
vector2i getNextTreasurePos(t_node** labyrinth, vector2i labSize, int direction) {
    int minimumTreasure = 9999; //+ petit trésor = prochain à visiter.
    vector2i nextTreasurePos = newVect2i(-1, -1);

    for(int i=0; i<labSize.y; i++) {
        for(int j=0; j<labSize.x; j++) {
            int treasureValue = labyrinth[i][j].treasureVal;

            if (treasureValue > 0 && treasureValue < minimumTreasure) {
                minimumTreasure = treasureValue;
                //METTRE A JOUR LA POSITION DU PROCHAIN TRESOR
                nextTreasurePos.y=i; nextTreasurePos.x = j;
            }
        }
    }

    return nextTreasurePos;
}





//renvoie la longueur du plus petit chemin entre les positions posDepart et posArrivee.

int shortestPathLength(t_node** lab, vector2i labSize, vector2i posDepart, vector2i posArrivee) {


    e_direction directions[4] = {NORTH, SOUTH, EAST, WEST};

    //VISITED = matrice2D de taille du lab qui dit si oui ou non la node a été visitée pour la recherche de chemin
    int** visited = malloc(sizeof(int*) * labSize.y);
    for (int i=0; i<labSize.y; i++) {
        visited[i] = calloc(labSize.x, sizeof(int));
    }

    //INITIALISATION
    int front=0, back=0;
    int max = labSize.x * labSize.y;

    vector2i* queue = malloc(sizeof(vector2i)*max);
    int* distances  = malloc(sizeof(int)*max);

    queue[back] = posDepart;
    distances[back] = 0;
    back++;

    visited[posDepart.y][posDepart.x] = 1;

    while (front < back) {
        vector2i current = queue[front];
        int currentDist = distances[front];
        front++;

        //SI ON A TOUCHE LA FIN:
        if ( areVectEq(current, posArrivee) ) {
            for(int i=0; i<labSize.y; i++) free(visited[i]);
            free(visited); free(queue); free(distances);
            return currentDist;
        }

        for(int dir=0; dir<4; dir++) { //dir=0 <=> dir=NORTH [...etc] (cf node.h)
            vector2i neighborPos = current;

            if ( isNeighborConnected(lab, current, directions[dir]) ) {
                neighborPos = vect2iAdd( neighborPos, getVectFromDir(directions[dir]) );
            }
            
            //S'assurer qu'on est pas oob
            if ((neighborPos.x >= 0 && neighborPos.x < labSize.x) && (neighborPos.y >= 0 && neighborPos.y < labSize.y)) {

                if (!visited[neighborPos.y][neighborPos.x]) { 

                    visited[neighborPos.y][neighborPos.x] = 1; //si on a pas visité on visite
                    queue[back] = neighborPos;
                    distances[back] = currentDist + 1;
                    back++;
                } 
            }
        }
    }

    //SI AUCUN CHEMIN TROUVE:
    for (int i = 0; i < labSize.y; i++) free(visited[i]);
    free(visited); free(queue); free(distances);

    return -1;
    
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
void insertTile(t_node** lab, vector2i labSize, t_node* extraTile, e_direction dirToInsert, int insertIndex) {
    int sizeX = labSize.x, sizeY = labSize.y;


    switch(dirToInsert) {

        //|--- INSERER LA CASE EN HAUT ---|
        case NORTH:
            for(int i=sizeY-1; i>0; i--) {
                lab[i][insertIndex] = lab[i-1][insertIndex];
            }
        
            lab[0][insertIndex] = *extraTile;
        break;

        //|--- INSERER LA CASE EN BAS ---|
        case SOUTH:
            for(int i=0; i<sizeY-1; i++) {
                lab[i][insertIndex] = lab[i+1][insertIndex];
            }

            lab[sizeY-1][insertIndex] = *extraTile;
        break;

        //|--- INSERER LA CASE A GAUCHE ---|
        case WEST:
            for(int j=sizeX-1; j>0; j--) {
                lab[insertIndex][j] = lab[insertIndex][j-1];
            }
        
            lab[insertIndex][0] = *extraTile;
        break;

        //|--- INSERER LA CASE A DROITE ---|
        case EAST:
            for(int j=0; j<sizeX-1; j++) {
                lab[insertIndex][j] = lab[insertIndex][j+1];
            }
        

            lab[insertIndex][sizeX-1] = *extraTile;
        break;
    }

}



void computeOpponentMove(t_node** lab, vector2i labSize, t_node* extraTile, char* moveString, char* msg, player* opponent) {

    int insertDirInt; /*direction d'insertion (cf docu pdf)
        •0 pour une insertion d’une ligne `a gauche
        •1 pour une insertion d’une ligne `a droite
        •2 pour une insertion d’une colonne en haut
        •3 pour une insertion d’une colonne en bas
    */
    int insertIndex; //Indice de la ligne ou de la colonne où on insert la tuile
    int nbRotations; //Nombre de rotation de la tuile avant de l’insérer (de 0 à 3 quart de tour dans le sens horaire)

    vector2i posToReach; //position à atteindre après l'insertion de la tuile.

    sscanf(moveString, "%d %d %d %d %d", //n=nb de char que j'ai mangé.
            &insertDirInt,
            &insertIndex,
            &nbRotations,
            &posToReach.x,
            &posToReach.y
            );

    //scanner le message et remplir extraTile et le prochain trésor de l'opponent.
    sscanf(msg, "%d %d %d %d %d %d", //n=nb de char que j'ai mangé.
            &extraTile->NORTH,
            &extraTile->EAST,
            &extraTile->SOUTH,
            &extraTile->WEST,
            &extraTile->treasureVal,

            &opponent->nextTreasureVal
            );

    e_direction dirToInsert;

    switch (insertDirInt) { //DETERMINER LA DIRECTION D'ENTREE
        case 0: dirToInsert = WEST; break;
        case 1: dirToInsert = EAST; break; 
        case 2: dirToInsert = NORTH; break;
        case 3: dirToInsert = SOUTH; break;
    }

    //effectuer les rotations de la tuile.
    for(int i=0; i<nbRotations; i++) rotateNode90CW(extraTile);

    insertTile(lab, labSize, extraTile, dirToInsert, insertIndex); //INSERER LA TUILE

    opponent->pos = posToReach; //déplacer l'adversaire où il doit aller.
}

