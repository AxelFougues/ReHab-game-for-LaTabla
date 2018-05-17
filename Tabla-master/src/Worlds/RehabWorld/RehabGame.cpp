//
//  RehabGame.cpp
//  Tabla
//
//  Created by afougu01 on 14/05/2018.
//

#include <stdio.h>
#include "RehabGame.h"

void RehabGame::fillBoard(){
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            board[x][y].setPos(x, y);
        }
    }
}

void RehabGame::nidificate(){
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            board[x][y].nidificate();
        }
    }
}

void RehabGame::harvest(){
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            int i = 0;
            if( x == 0 || x == RehabGame::boardWidth ) i++;
            if( y == 0 || y == RehabGame::boardHeight ) i++;
            board[x][y].harvest(i, neighborHarvesters(cinder::vec2(x,y)));
        }
    }
}

int RehabGame::neighborHarvesters(cinder::vec2 pos){
    int h = 0;
    for (int y = pos.y - 1; y <= pos.y + 1; y++) {
        for (int x = pos.x - 1; x <= pos.x + 1; x++) {
            if(x>=0 && x<boardWidth && y>=0 && y<boardHeight)
                h += board[x][y].getHarvesterTokens();
        }
    }
    return h;
}

