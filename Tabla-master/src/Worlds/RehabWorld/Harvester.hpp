//
//  Harvester.hpp
//  Tabla
//
//  Created by afougu01 on 18/05/2018.
//

#ifndef Harvester_hpp
#define Harvester_hpp

#include <stdio.h>

using namespace std;

class Harvester{
public:
    
    Harvester(){
        turnPoints = 0;
        cout << "player added " << endl;
    }
    
    list<int> harvestPoints;
    int turnPoints;
    
    void giveHarvestPoints(int pts){
        turnPoints += pts;
    }
    
    void validateHarvestPoints(){
        harvestPoints.push_back(turnPoints);
        turnPoints = 0;
    }
    
    void clear(){
        harvestPoints.clear();
    }
    
    int getTotalScore(){
        int i = 0;
        for(int s : harvestPoints){
            i += s;
        }
        return i;
    }
};

#endif /* Harvester_hpp */
