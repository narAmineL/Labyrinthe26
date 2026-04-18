#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "node.h"
#include "labyrinthe.h"
#include "player.h"

//l'api du prof
#include "../LAB26_subject/labyrinthe26/labyrinthAPI.h"



typedef struct {
    t_player player;
    t_player opponent;

    t_node extraTile;

    int isOpponentTurn;
    int turnNumber;
    
    //variables booléennes qui disent si un des deux a gagné.
    int playerWon, opponentWon; 

} t_game;


int main(void) {

    //|--- CONNEXION ET INIT ---|
    connectToServer("vps-1a2cee88.vps.ovh.net", 1234, "narAmyn"); //se connecter au jeu.
    


    t_game g; //structure qui définit le jeu.
    


    //|--- CREATION DU LABYRINTHE ---|
    vector2i labSize = {.x=0, .y=0};
    char labName[50];

    waitForLabyrinth("TRAINING DONTMOVE start=1", labName, (int*)&labSize.x, (int*)&labSize.y); //start=1 -> opp commence
    t_node** lab = createLabyrinth(labSize); //création du labyrinthe.
    fillLabyrinth(lab, labSize, &g.isOpponentTurn, &g.extraTile); //le remplir selon getLabData de l'api et renvoyer l'extra tile. dit aussi qui commence
    

    

    //|--- INIT LE JOUEUR ---|
    g.player.pos = vect2iSub(labSize, newVect2i(1, 1)); //le joueur se trouve toujours en sizeX-1 sizeY-1 (temporaire)
    g.player.nextTreasureVal=24;
    g.player.isAscendingOrder=0;

    g.playerWon=0; g.opponentWon=0; g.turnNumber=1;

    //|--- BOUCLE PRINCIPALE ---|
    while(!g.playerWon || !g.opponentWon) {

        //|--- AFFICHAGE DEBUG: ---|
        printf("TOUR NUMERO: %d\n\n", g.turnNumber);
        g.turnNumber++;
        printf("POSITION DU JOUEUR: %d,%d\nPOSITION DE ADV: %d,%d\n\n", g.player.pos.x, g.player.pos.y, g.opponent.pos.x, g.opponent.pos.y);
        //printLabyrinth();
        myPrintLabyrinth(lab, g.extraTile, labSize, &g.player, &g.opponent);

        char opponentMove[64], msg[64];
        t_return_code returnCode;

        //|--- RECUPERER TOUR DE L'ADVERSAIRE ---|
        if (g.isOpponentTurn) {

            returnCode = getMove(opponentMove, msg); //RECUP COUP ADVERSAIRE
            
            computeOpponentMove(lab, labSize, &g.extraTile, opponentMove, msg, &g.player, &g.opponent); //LE PROCESS

            if (returnCode==WINNING_MOVE) g.opponentWon=1;
            else if (returnCode==LOSING_MOVE) printf("Un coup illégal a été joué par l'adversaire !");

            g.isOpponentTurn=0; //finir tour adversaire.

        } 


        //|--- TOUR DU JOUEUR ---|
        else {
     
            returnCode = sendMove( calculateNextMove(lab, labSize, &g.extraTile, &g.player, &g.opponent) , msg);
            g.isOpponentTurn=1;
        }

        //clear.
        system("clear");

    }



    //couper la connexion et finir le jeu.
    closeConnection();
    return EXIT_SUCCESS;
}

