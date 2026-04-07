#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "node.h"
#include "labyrinthe.h"
#include "player.h"

//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"




int main(void) {

    //|--- CONNEXION ET INIT ---|
    connectToServer("vps-1a2cee88.vps.ovh.net", 1234, "narAmyn"); //se connecter au jeu.
    



    

    //|--- CREATION DE L'ADVERSAIRE ---|
    player opponent;
    int isOpponentTurn;

    //|--- CREATION DU LABYRINTHE ---|
    vector2i labSize = {.x=0, .y=0};
    char labName[50];
    t_node extraTile;

    waitForLabyrinth("TRAINING RANDOM start=1", labName, (int*)&labSize.x, (int*)&labSize.y); //start=1 -> opp commence

    t_node** lab = createLabyrinth(labSize); //création du labyrinthe.
    fillLabyrinth(lab, labSize, &isOpponentTurn, &extraTile); //le remplir selon getLabData de l'api et renvoyer l'extra tile. dit aussi qui commence
    
    //variables booléennes qui disent si un des deux a gagné.
    int playerWon = 0, opponentWon = 0;

    //|--- BOUCLE PRINCIPALE ---|
    while(!playerWon || !opponentWon) {
        printLabyrinth();

        char opponentMove[64];
        char msg[64];  
        t_return_code returnCode;

        //|--- RECUPERER TOUR DE L'ADVERSAIRE ---|
        if (isOpponentTurn) {
            returnCode = getMove(opponentMove, msg);
            computeOpponentMove(lab, labSize, &extraTile, opponentMove, msg, &opponent);

            if (returnCode==WINNING_MOVE) opponentWon=1;
            else if (returnCode==LOSING_MOVE) printf("Un coup illégal a été joué par l'adversaire !");

            isOpponentTurn=0; //finir tour adversaire.
        } 
        //|--- TOUR DU JOUEUR ---|
        else {


        }


    }



    //couper la connexion et finir le jeu.
    closeConnection();
    return EXIT_SUCCESS;
}

