#ifndef __NODE_H__
#define __NODE_H__


typedef enum {NORTH, SOUTH, WEST, EAST} e_direction;

//structure d'une node: chaque position est un bool (=0/1) qui indique si
//la position est passante ou non.
//treasureID vaut -1 si il n'y a pas de trésor, et vaut l'ID du trésor si il y en a un.
typedef struct {
    int NORTH;
    int SOUTH;
    int EAST;
    int WEST;

    int treasureVal;
} t_node;



void rotateNode90CCW(t_node* node);
int isNeighborConnected(t_node** labyrinth, vector2i pos, e_direction dir);

#endif