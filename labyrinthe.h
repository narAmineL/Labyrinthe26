#ifndef __LABYRINTHE_H__
#define __LABYRINTHE_H__

#include "node.h"
#include "vector2.h"

void fillLabyrinth(t_node** lab, vector2i labSize, int* isOpponentStarting, t_node* extraTile);
int shortestPathLength(t_node** lab, vector2i labSize, vector2i posDepart, vector2i posArrivee);
vector2i getNextTreasurePos(t_node** labyrinth, vector2i labSize, int direction);
void destroyLabyrinth(t_node** labyrinth, vector2i labSize);
t_node** createLabyrinth(vector2i labSize);

#endif