#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "player.h"




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

        for (int dir = 0; dir < 4; dir++) {
            // Ne traiter que si la connexion existe réellement
            if (!isNeighborConnected(lab, current, directions[dir])) continue;

            vector2i neighborPos = vect2iAdd(current, getVectFromDir(directions[dir]));

            if (neighborPos.x < 0 || neighborPos.x >= labSize.x ||
                neighborPos.y < 0 || neighborPos.y >= labSize.y)
                continue;

            if (!visited[neighborPos.y][neighborPos.x]) {
                visited[neighborPos.y][neighborPos.x] = 1;
                // Garde contre dépassement du tableau
                if (back >= max) {
                    for (int i = 0; i < labSize.y; i++) free(visited[i]);
                    free(visited); free(queue); free(distances);
                    return -1;
                }
                queue[back] = neighborPos;
                distances[back] = currentDist + 1;
                back++;
            }
        }
    }

    //SI AUCUN CHEMIN TROUVE:
    for (int i = 0; i < labSize.y; i++) free(visited[i]);
    free(visited); free(queue); free(distances);

    return -1;
    
}



//fonction qui renvoie la position du prochain trésor qu'on doit atteindre.
//si problème ou prochain trésor inexistant, renvoie le vecteur {-1, -1}.
vector2i getNextTreasurePos(t_node** labyrinth, vector2i labSize, t_player* player) {

    vector2i nextTreasurePos = newVect2i(-1, -1);

    for(int i=0; i<labSize.y; i++) {
        for(int j=0; j<labSize.x; j++) {

            if (labyrinth[i][j].treasureVal == player->nextTreasureVal) {
                nextTreasurePos.y=i; nextTreasurePos.x=j;
            }
        }
    }

    return nextTreasurePos;
}



//ALGO PRINCIPAL DU PROGRAMME QUI RENVOIE NOTRE COUP A JOUER DE LA FORME:
//1 5 1 1 2 qui valent dans l'ordre insertDirInt, insertIndx, nbRot, posToReach.x, posToReach.y
char* calculateNextMove(t_node** lab, vector2i labSize, t_node* extraTile, t_player* player, t_player* opponent) {
    
    static char result[20];
    int insertDirInt;

    
  
    t_insertion bestInsertion = getBestInsertion(lab, labSize, extraTile, player);
    insertTile(lab, labSize, *extraTile, bestInsertion, player, opponent);

    //TEMPORAIRE: on reste immobile.
    vector2i posToReach=getBestPlayerPos(lab, labSize, player); //position qu'on veut joindre après l'insertion de la tuile.
    
    if (areVectEq( posToReach, newVect2i(-1, -1) )) { //si vecteur erreur; rester immobile.
        posToReach=player->pos;
    }

    switch (bestInsertion.insertDir) { //DETERMINER LA DIRECTION D'ENTREE
        case WEST: insertDirInt = 0; break;
        case EAST: insertDirInt = 1; break; 
        case NORTH: insertDirInt = 2; break;
        case SOUTH: insertDirInt = 3; break;
    }

    //écrire dans result: conversion de t_insertion vers string
    sprintf(result, "%d %d %d %d %d",
        insertDirInt,
        bestInsertion.insertIndex,
        bestInsertion.nbRotations,
        posToReach.x,
        posToReach.y
    );

    printf("MEILLEURE INSERTION TROUVEE: %s\nTRADUCTION: ", result);
    printInsertion(bestInsertion);
    return result;
}



//fonction qui déplace le joueur de 1 unité dans la direction dir en prenant en compte le fonctionnement de tore.
void movePlayerOneTile(vector2i labSize, t_player* player, e_direction moveDir) {
    int sizeX=labSize.x, sizeY=labSize.y;

    switch(moveDir) {
        case NORTH:
            if (player->pos.y == sizeY-1) player->pos.y = 0; //si joueur en bas revenir en haut
                    else player->pos.y+=1; //sinon on descend
        break;

        case SOUTH:
            if (player->pos.y == 0) player->pos.y = sizeY-1; //si joueur en haut revenir en bas
                    else player->pos.y-=1; //sinon on monte
        break;

        case EAST:
            if (player->pos.x == 0) player->pos.x = sizeX-1; //si joueur à gauche revenir à droite
                    else player->pos.x-=1; //sinon aller à gauche
        break;

        case WEST:
            if (player->pos.x == sizeX-1) player->pos.x = 0; //si joueur à droite revenir à gauche
                    else player->pos.x+=1; //sinon on va à droite
        break;
    } 
}


//fonction qui calcule la meilleure insertion à faire, et qui mets à jour les variables en param selon cette
//insertion
//TEMPORAIRE: toujours décaler au nord en 0 et sans rotation.
//ATTENTION: on ne peut insérer que dans les rangs impairs mdrr
t_insertion getBestInsertion(t_node** lab, vector2i labSize, t_node* extraTile, t_player* player) {
    int sizeX=labSize.x, sizeY=labSize.y, minDistToTreasure = 99999;

    return (t_insertion){.insertDir=NORTH, .insertIndex=1, .nbRotations=0};
}





//fct qui renvoie le vect2i de la meilleure position qu'on peut prendre (jouée après getBestInsertion)
//TEMPORAIRE: immobile.
vector2i getBestPlayerPos(t_node** lab, vector2i labSize, t_player* player) {


    return player->pos;
}

