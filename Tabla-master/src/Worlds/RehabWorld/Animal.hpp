//
//  Animal.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef Animal_hpp
#define Animal_hpp

#include <stdio.h>
#include "SpaceUnit.hpp"

class Animal{
    
public:
    
    Animal();
    Animal(SpaceUnit *su);
    
    
    bool settled;
    int reproduce();
    void settle();
    
private:
    
    const int habitatSuitabilityThreshold = 2;
    float reproductionProbability;
    bool testReproduction;
    SpaceUnit *habitat;
};
#endif /* Animal_hpp */
