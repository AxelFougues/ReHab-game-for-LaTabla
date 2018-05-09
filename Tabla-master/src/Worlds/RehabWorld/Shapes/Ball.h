//
//  Shape.hpp
//  Tabla
//
//  Created by Axel Fougues on 11/04/2018.
//
//

#ifndef Ball_h
#define Ball_h

class Ball : public Shape {
public:
    Ball(vec2 loc): Shape(loc){
        setDefaultCollision();
    }
    
    virtual void drawShape(){
        gl::color(mColor);
        gl::drawSolidCircle( mLoc, mScale);
    }
		
};

#endif /* Ball_h */
