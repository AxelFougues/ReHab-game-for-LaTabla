//
//  SpaceUnit.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef SpaceUnit_hpp
#define SpaceUnit_hpp

#include <stdio.h>
#include "RehabGame.h"
#include "Plant.hpp"
#include "Animal.hpp"

class Plant;
class Animal;

class SpaceUnit{
    
public:
    SpaceUnit(){}
    
    SpaceUnit(int x, int y, RehabGame* game);

    int getBiomass(){return biomass;}
    bool getAccess(){return access;}
    int getHarvestedNeighbors();
    int getSpaceUnitPosition();
    
    
private:
    
    int biomass;
    bool access;
    int posX, posY;
    RehabGame* game;
    Plant* plant;
    Animal* animal;
    
};

#endif /* SpaceUnit_hpp */
