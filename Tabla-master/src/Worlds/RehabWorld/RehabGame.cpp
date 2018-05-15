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

