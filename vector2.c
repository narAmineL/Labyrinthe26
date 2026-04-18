#include "vector2.h"
#include "node.h"

//créer un nouveau vect2i et le renvoyer
vector2i newVect2i(int x, int y) {
    vector2i result;
    result.x = x;
    result.y = y;
    return result;
}

//renvoie 1 si vecteur vide, 0 sinon.
int isVect2iEmpty(vector2i V) {
    if (V.x == 0 && V.y == 0) return 1;
    else return 0;
}

int areVectEq(vector2i V1, vector2i V2) {
    if (V1.x == V2.x && V1.y == V2.y) return 1;
    else return 0;
}

//multiplication par un scalaire entier, ici a
vector2i scalarMult(int a, vector2i V) {
    return (vector2i){.x=V.x*a, .y=V.y*a};
}

//renvoie un vecteur résultant de l'addition de V1 et V2
vector2i vect2iAdd(vector2i V1, vector2i V2) {
    return newVect2i(V1.x + V2.x, V1.y + V2.y);
}

//renvoie un vecteur résultant de l'addition de V1 et V2
vector2i vect2iSub(vector2i V1, vector2i V2) {
    return newVect2i(V1.x - V2.x, V1.y - V2.y);
}


//renvoie un vecteur qui pointe dans la direction donnée par dir
//renvoie vecteur nul en cas de pb.
vector2i getVectFromDir(e_direction dir) {
    switch (dir) {
        case NORTH:
            return (vector2i){.x=0, .y=-1};
        break;
        case SOUTH:
            return (vector2i){.x=0, .y=+1};
        break;
        case EAST:
            return (vector2i){.x=1, .y=0};
        break;
        case WEST:
            return (vector2i){.x=-1, .y=0};
        break;
    }
    return (vector2i){.x=0, .y=0};
}

void printVect2i(vector2i V) {
    printf("{%d,%d}", V.x, V.y);
}

//fct qui renvoie la direction opposée à dir. sud -> nord, est -> ouest...
//renvoie nord par défaut. jsp. J'avais envie.
e_direction getOppositeDir(e_direction* dir) {
    switch(*dir) {
        case NORTH: return SOUTH; break;
        case SOUTH: return NORTH; break;
        case EAST: return WEST; break;
        case WEST: return EAST; break;
    }
    return NORTH;
}