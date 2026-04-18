#ifndef __VECTOR2_H__
#define __VECTOR2_H__


typedef enum {NORTH, SOUTH, WEST, EAST} e_direction;

typedef struct {
    int x;
    int y;

} vector2i;


vector2i vect2iSub(vector2i V1, vector2i V2);
vector2i vect2iAdd(vector2i V1, vector2i V2);
int isVect2iEmpty(vector2i V);
vector2i newVect2i(int x, int y);
int areVectEq(vector2i V1, vector2i V2);
vector2i getVectFromDir(e_direction dir);
void printVect2i(vector2i V);
e_direction getOppositeDir(e_direction* dir);
vector2i scalarMult(int a, vector2i V);


#endif
