#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "node.h"


//fct qui tourne une node de 90 degrés dans le sens trigonométrique. (CounterClockWise)
void rotateNode90CCW(t_node* node) {
    int tempNorth = node->NORTH;
    node->NORTH = node->EAST;
    node->EAST = node->SOUTH;
    node->SOUTH = node->WEST;
    node->WEST = tempNorth;
}

//fct qui renvoie 1 si la node lab[posY][posX] est connectée à la node qui se trouve à la direction dir
//renvoie 0 si non connecté
int isNeighborConnected(t_node** labyrinth, vector2i pos, e_direction dir) {
    int posX=pos.x;
    int posY=pos.y;

    t_node currentNode = labyrinth[posY][posX];
    switch (dir) {
        case NORTH:
            if ( (labyrinth[posY-1][posX].SOUTH == 1) && (currentNode.NORTH == 1) ) return 1;
        break;

        case SOUTH:
            if ( (labyrinth[posY+1][posX].NORTH == 1) && (currentNode.SOUTH == 1) ) return 1;
        break;

        case WEST:
            if ( (labyrinth[posY][posX-1].EAST == 1) && (currentNode.WEST == 1) ) return 1;
        break;

        case EAST:
            if ( (labyrinth[posY][posX+1].WEST == 1) && (currentNode.EAST == 1) ) return 1;
        break;

        }
    return 0;
}
