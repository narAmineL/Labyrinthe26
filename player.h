#ifndef __PLAYER_H__
#define __PLAYER_H__

//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"

#include "vector2.h"

#include "node.h"


typedef struct {

    vector2i pos;
    int nextTreasureVal;

    int isAscendingOrder; //vaut 1 si on doit manger en ordre croissant, 0 sinon.
} t_player;

#include "labyrinthe.h"

int shortestPathLength(t_node** lab, vector2i labSize, vector2i posDepart, vector2i posArrivee);
t_insertion getBestInsertion(t_node** lab, vector2i labSize, t_node* extraTile, t_player* player);
char* calculateNextMove(t_node** lab, vector2i labSize, t_node* extraTile, t_player* player);
void movePlayerOneTile(vector2i labSize, t_player* player, e_direction moveDir);
vector2i getBestPlayerPos(t_node** lab, vector2i labSize, t_player* player);

#endif