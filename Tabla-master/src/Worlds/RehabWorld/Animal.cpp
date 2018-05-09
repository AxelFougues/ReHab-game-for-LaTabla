//
//  Animal.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "Animal.hpp"

Animal::Animal(SpaceUnit *su){
    habitat = su;
}

int Animal::reproduce(){
    if(settled){
        int n = habitat->getHarvestedNeighbors();
        int pos = habitat->getSpaceUnitPosition();
        switch (pos) {
            case 0:
                if(n <= 1)
                    return 2;
                if(n <= 4)
                    return 1;
                break;
            case 1:
                if(n == 0)
                    return 2;
                if(n <= 2)
                    return 1;
                break;
            case 2:
                if(n == 0)
                    return 2;
                if(n == 1)
                    return 1;
                break;
        }
    }
    return 0;
}

void Animal::settle(){
    if(!settled && habitat->getBiomass() >= habitatSuitabilityThreshold){
        settled = true;
    }
}
