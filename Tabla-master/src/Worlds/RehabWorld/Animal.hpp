//
//  Animal.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef Animal_hpp
#define Animal_hpp

#include <stdio.h>

class Animal{
    
public:
    
    bool settled = false;
    
    int kids = 0;
    
    /*!
     * @discussion Produces kids under the right conditions.
     * @param pos position of cell, 0=center, 1=edge, 2=corner.
     * @param neighborsHarvested count of neighbors containing harvester tokens.
     */
    void reproduce(int pos, int neighborsHarvested);
    /*!
     * @discussion If conditions are met the animal settles.
     * @param b biomass of the cell.
     */
    void settle(int b);
    /*!
     * @discussion Unsettles the animal.
    void unSettle();
    */
    void unSettle();
    
private:
    
    const int habitatSuitabilityThreshold = 2;
    
};
#endif /* Animal_hpp */
