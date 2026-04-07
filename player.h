#ifndef __PLAYER_H__
#define __PLAYER_H__

//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"

#include "vector2.h"
#include "labyrinthe.h"
#include "node.h"


typedef struct {

    vector2i pos;
    int nextTreasureVal;

} player;

int shortestPathLength(t_node** lab, vector2i labSize, vector2i posDepart, vector2i posArrivee);

#endif