#ifndef __LABYRINTHE_H__
#define __LABYRINTHE_H__

#include "node.h"
#include "vector2.h"
#include "player.h"

void fillLabyrinth(t_node** lab, vector2i labSize, int* isOpponentStarting, t_node* extraTile);
void destroyLabyrinth(t_node** labyrinth, vector2i labSize);
t_node** createLabyrinth(vector2i labSize);
void copyLabyrinth(t_node** from, t_node** to, vector2i labSize);
void computeOpponentMove(t_node** lab, vector2i labSize, t_node* extraTile, char* moveString, char* msg, t_player* player, t_player* opponent);
void insertTile(t_node** lab, vector2i labSize, t_node* extraTile, t_insertion insertion, t_player* player, t_player* opponent);
void copyLabyrinth(t_node** from, t_node** to, vector2i labSize);

#endif