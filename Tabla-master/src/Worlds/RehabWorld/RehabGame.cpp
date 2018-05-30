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
            board[x][y].harvest();
        }
    }
}

void RehabGame::renew(){
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            board[x][y].renew();
            board[x][y].unSettle();
            board[x][y].clearHarvesterTokens();
        }
    }
}

void RehabGame::reproduce(){
    int pop = 0;
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            int i = 0;
            if( x == 0 || x == RehabGame::boardWidth ) i++;
            if( y == 0 || y == RehabGame::boardHeight ) i++;
            board[x][y].reproduce(i, neighborsHarvested(cinder::vec2(x,y)));
            if(board[x][y].getPopulation()>1) births++;
            pop += board[x][y].getPopulation();
        }
    }
    animalPopulation.push_back(pop);
}

int RehabGame::neighborsHarvested(cinder::vec2 pos){
    int h = 0;
    for (int y = pos.y - 1; y <= pos.y + 1; y++) {
        for (int x = pos.x - 1; x <= pos.x + 1; x++) {
            if(x>=0 && x<boardWidth && y>=0 && y<boardHeight)
                h += board[x][y].getHarvesterTokens().size();
        }
    }
    return h;
}

void RehabGame::attributePoints(){
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            int bio = board[x][y].getBiomass();
            list<Token*> cellTokens = board[x][y].getHarvesterTokens();
            if(cellTokens.size() > 0){
                int toDistribute = 0;
                if(board[x][y].getBiomass() == 1) toDistribute = 0;
                if(board[x][y].getBiomass() == 2) toDistribute = 2;
                if(board[x][y].getBiomass() == 3) toDistribute = 3;
                if(board[x][y].getBiomass() == 3 && cellTokens.size() == 1) toDistribute = 2;
                
                for(Token* t : cellTokens){
                    t->owner->giveHarvestPoints(toDistribute/cellTokens.size());
                    t->increaseValue(toDistribute/cellTokens.size());
                }
                cellTokens.back()->increaseValue(toDistribute%cellTokens.size());
            }
        }
    }
    for(Harvester *h : harvesters){
        h->validateHarvestPoints();
        cout << "points :" << h->harvestPoints.back();
    }
}

void RehabGame::addHarvesterToken(int i, vec2 pos, vec2 loc){
    Token* t = new Token(harvesters[i], loc);
    board[(int)pos.x][(int)pos.y].addHarvesterToken(t);
}

