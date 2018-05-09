//
//  SpaceUnit.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "SpaceUnit.hpp"

SpaceUnit::SpaceUnit(int x, int y, RehabGame* game){
    biomass = 2;
    posX = x;
    posY = y;
    access = true;
    game = game;
    plant = new Plant(this);
    animal = new Animal(this);
}

int SpaceUnit::getSpaceUnitPosition(){
    int i = 0;
    if( posX == 0 || posX == RehabGame::boardWidth ) i++;
    if( posY == 0 || posY == RehabGame::boardHeight ) i++;
    return i;
}

int SpaceUnit::getHarvestedNeighbors(){
    
}
