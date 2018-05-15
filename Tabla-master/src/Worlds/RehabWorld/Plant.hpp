//
//  Plant.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef Plant_hpp
#define Plant_hpp

#include <stdio.h>
#include <stdlib.h>


class Plant{
  
public:
    
    Plant(){
        biomass = rand() % (maxBiomass + 1);
    }
    
    int getBiomass(){return biomass;}
    void renewBiomass(int harvesters);
    
private:
    
    const int maxBiomass = 3;
    const int minBiomass = 0;
    int biomass;
    int noHarvestPeriod = 0;
    
    void updateNoHarvestPeriod(bool harvested);
    void decreaseBiomassBy(int x);
    void increaseBiomassBy(int x);
    
};

#endif /* Plant_hpp */
