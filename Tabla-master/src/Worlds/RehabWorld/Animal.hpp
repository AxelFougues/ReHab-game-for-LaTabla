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
    void reproduce(int h, int pos);
    void settle(int b);
    void unSettle();
    
    
private:
    
    const int habitatSuitabilityThreshold = 2;
    
};
#endif /* Animal_hpp */
