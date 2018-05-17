//
//  SpaceUnit.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef SpaceUnit_hpp
#define SpaceUnit_hpp

#include <stdio.h>
#include "Plant.hpp"
#include "Animal.hpp"


class SpaceUnit{
    
public:
    
    void setPos(int x, int y){posX = x; posY = y;}
    int getBiomass(){return plant.getBiomass();}
    bool getAccess(){return access;}
    bool changeAccess(){access = !access; return access;}
    int getSpaceUnitPosition();
    void nidificate();
    bool isSettled(){return animal.settled;}
    void addHarvesterToken(){harvesterTokens++;}
    void harvest(int pos, int harv);
    int getHarvesterTokens(){return harvesterTokens;}
    int getPopulation(){if(!animal.settled) return 0; return animal.kids+1;}
    
private:
    
    bool access = true;
    int posX, posY;
    Plant plant;
    Animal animal;
    int harvesterTokens = 0;
    
};

#endif /* SpaceUnit_hpp */
