//
//  SpaceUnit.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "SpaceUnit.hpp"


void SpaceUnit::nidificate(){
    animal.settle(plant.getBiomass());
}

void SpaceUnit::harvest(int pos, int harv){
    if(harvesterTokens == 0){
        animal.reproduce(pos, harv);
    }else{
        plant.renewBiomass(harvesterTokens);
    }
}
