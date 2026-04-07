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
    t_player opponent;
    t_player player;
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

    //|--- INIT LE JOUEUR ---|
    player.pos = vect2iSub(labSize, newVect2i(1, 1)); //le joueur se trouve toujours en sizeX-1 sizeY-1 (temporaire)
    player.nextTreasureVal=24;
    player.isAscendingOrder=0;


    //|--- BOUCLE PRINCIPALE ---|
    while(!playerWon || !opponentWon) {

        //|--- AFFICHAGE DEBUG: ---|
        printf("POSITION DU JOUEUR: %d,%d\nPOSITION DE ADV: %d,%d\n\n", player.pos.x, player.pos.y, opponent.pos.x, opponent.pos.y);
        printLabyrinth();

        char opponentMove[64], msg[64];
        t_return_code returnCode;

        //|--- RECUPERER TOUR DE L'ADVERSAIRE ---|
        if (isOpponentTurn) {

            returnCode = getMove(opponentMove, msg);
            
            computeOpponentMove(lab, labSize, &extraTile, opponentMove, msg, &player, &opponent);

            if (returnCode==WINNING_MOVE) opponentWon=1;
            else if (returnCode==LOSING_MOVE) printf("Un coup illégal a été joué par l'adversaire !");

            isOpponentTurn=0; //finir tour adversaire.

        } 


        //|--- TOUR DU JOUEUR ---|
        else {
     
            returnCode = sendMove( calculateNextMove(lab, labSize, &extraTile, &player) , msg);
            isOpponentTurn=1;
        }

        //clear.
        //system("clear");

    }



    //couper la connexion et finir le jeu.
    closeConnection();
    return EXIT_SUCCESS;
}

