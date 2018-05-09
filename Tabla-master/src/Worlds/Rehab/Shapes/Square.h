//
//  Shape.hpp
//  Tabla
//
//  Created by Axel Fougues on 11/04/2018.
//
//

#ifndef Square_h
#define Square_h

#include "cinder/GeomIo.h"
#include "Shape.h"
#include <math.h>

class Square : public Shape {
	
    public:
        int rotation = 10;
        Square(vec2 loc): Shape(loc){
            setDefaultCollision();
		}
	
		virtual void drawShape(){
			gl::color(mColor);
            gl::drawSolidRect( Rectf( mLoc.x-mScale, mLoc.y-mScale, mLoc.x+mScale, mLoc.y+mScale ) );
		}
};

#endif /* Square_h */
