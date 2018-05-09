//
//  Plant.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "Plant.hpp"


Plant::Plant(SpaceUnit *su){
    noHarvestPeriod = 0;
    biomass = rand() % maxBiomass + 1;
    habitat = su;
}

void Plant::decreaseBiomassBy(int x){
    if(biomass >= minBiomass + x)
        biomass -= x;
    else
        biomass = minBiomass;
}

void Plant::increaseBiomassBy(int x){
    if(biomass <= maxBiomass - x)
        biomass += x;
    else
        biomass = maxBiomass;
}

void Plant::renewBiomass(int harvesters){
    if(harvesters > 1){
        decreaseBiomassBy(harvesters);
    }else if (harvesters == 0){
        if(noHarvestPeriod == 0){
            increaseBiomassBy(1);
        }else{
            if(noHarvestPeriod > 1)
                decreaseBiomassBy(1);
        }
    }
    
    updateNoHarvestPeriod(harvesters>0);
}

void Plant::updateNoHarvestPeriod(bool harvested){
    if(harvested)
        noHarvestPeriod = 0;
    else
        noHarvestPeriod++;
}
