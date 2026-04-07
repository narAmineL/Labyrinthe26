#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "node.h"


//fct qui tourne une node de 90 degrés dans le sens horaire. (ClockWise)
void rotateNode90CW(t_node* node) {
    int tempNorth = node->NORTH;
    node->NORTH = node->WEST;
    node->WEST = node->SOUTH;
    node->SOUTH = node->EAST;
    node->EAST = tempNorth;
    
}

//fct qui renvoie 1 si la node lab[posY][posX] est connectée à la node qui se trouve à la direction dir
//renvoie 0 si non connecté
int isNeighborConnected(t_node** labyrinth, vector2i pos, e_direction dir) {
    int posX=pos.x;
    int posY=pos.y;

    t_node currentNode = labyrinth[posY][posX];
    switch (dir) {
        case NORTH:
            if ( (labyrinth[posY-1][posX].SOUTH == !IS_WALL) && (currentNode.NORTH == !IS_WALL) ) return 1;
        break;

        case SOUTH:
            if ( (labyrinth[posY+1][posX].NORTH == !IS_WALL) && (currentNode.SOUTH == !IS_WALL) ) return 1;
        break;

        case WEST:
            if ( (labyrinth[posY][posX-1].EAST == !IS_WALL) && (currentNode.WEST == !IS_WALL) ) return 1;
        break;

        case EAST:
            if ( (labyrinth[posY][posX+1].WEST == !IS_WALL) && (currentNode.EAST == !IS_WALL) ) return 1;
        break;

        }
    return 0;
}


void printNode(t_node node) {
    printf("isWall Nord/Sud/Est/Ouest: %d, %d, %d, %d, TV=%d\n",
    node.NORTH==IS_WALL,
    node.SOUTH==IS_WALL,
    node.EAST==IS_WALL,
    node.WEST==IS_WALL,
    node.treasureVal
    );
}