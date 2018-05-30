//
//  SpaceUnit.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "Cell.hpp"


void Cell::nidificate(){
    animal.settle(plant.getBiomass());
}

void Cell::harvest(){
    if(harvesterTokens.size() > 0)
        plant.harvest(harvesterTokens.size());
}

void Cell::reproduce(int pos, int neighborsHarvested){
    if(harvesterTokens.size() == 0)
        animal.reproduce(pos, neighborsHarvested);
    else
        animal.unSettle();
}

void Cell::renew(){
    if(harvesterTokens.size() == 0)
        plant.renewBiomass();
}
