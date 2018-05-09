//
//  RehabGame.h
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef RehabGame_h
#define RehabGame_h

#include "SpaceUnit.hpp"
class SpaceUnit;

class RehabGame{
    
public:
    
    static const int boardWidth = 5;
    static const int boardHeight = 4;
    
private:
    
    SpaceUnit *board[RehabGame::boardWidth][RehabGame::boardHeight];
    
};

#endif /* RehabGame_h */
