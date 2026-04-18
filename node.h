#ifndef __NODE_H__
#define __NODE_H__

#define IS_WALL 1

#include "vector2.h"

//structure d'une node: chaque position est un bool (=0/1) qui indique si
//la position est passante ou non. 1=passante 0=coupante
//treasureID vaut 0 si il n'y a pas de trésor, et vaut l'ID du trésor si il y en a un.
typedef struct {
    int NORTH;
    int SOUTH;
    int EAST;
    int WEST;

    int treasureVal;
} t_node;


//structure qui définit une insertion; la insertDir donne NOR=en haut, SUD=en bas...
//insertIndex=l'index d'insertion dans ctte direction
//nbRotations = nombres de rota en sens horaires qu'on fait a extratile avant de l'insérer
typedef struct {
    e_direction insertDir;
    int insertIndex;
    int nbRotations;

} t_insertion;

void printInsertion(t_insertion inser);

void printNode(t_node node);
t_node getRotatedNode90CW(int nbRotations, t_node node);
int isNeighborConnected(t_node** labyrinth, vector2i pos, e_direction dir);

#endif