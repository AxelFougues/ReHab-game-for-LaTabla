//
//  Token.hpp
//  Tabla
//
//  Created by afougu01 on 18/05/2018.
//

#ifndef Token_hpp
#define Token_hpp

#include <stdio.h>
#include "Harvester.hpp"

using namespace std;
using namespace cinder;

class Token{
public:
    
    Token(Harvester *h, vec2 p){
        owner = h;
        pos = p;
    }
    
    void increaseValue(int i);
    
    Harvester *owner;
    int currentValue = 0;
    vec2 pos;
};

#endif /* Token_hpp */
