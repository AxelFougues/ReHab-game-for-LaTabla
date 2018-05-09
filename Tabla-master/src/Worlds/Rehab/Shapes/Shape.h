//
//  Shape.hpp
//  Tabla
//
//  Created by Axel Fougues on 11/04/2018.
//
//

#ifndef Shape_h
#define Shape_h

class Shape {
public:
    Shape(){}
    Shape(vec2 loc){ mLoc = loc;}
    virtual ~Shape(){}
    vec2 mLoc = vec2(0,0);
    vec2 mLastLoc = vec2(0,0);
    vec2 mAccel = vec2(0,0);
    float mScale = 2.0;
    ColorA mColor = ColorA(1,1,1);
    ColorA mRibbonColor = ColorA(1,1,1) ;
    
    
    
    
    
    void  setMass( float m ) { mMass = m ; }
    float getMass() const { return mMass ; }
    float getInvMass() const { return 1.f / getMass() ; }
    void setLoc( vec2 l ) { vec2 v=getVel(); mLoc=l; setVel(v); }
    void setVel( vec2 v ) { mLastLoc = mLoc - v ; }
    vec2 getVel() const { return mLoc - mLastLoc ; }
    void noteSquashImpact( vec2 directionAndMagnitude ){if ( length(directionAndMagnitude) > length(mSquash) ) mSquash = directionAndMagnitude ;};
    virtual void drawShape(){
        gl::drawSolidCircle( mLoc, 1 ) ;    };
    void setDefaultCollision(){
        mCollideWithContours = 0;
        mPausePhysics = 0;
        mCapVelocity = 1;
    }
    
    
    
    
    
    vec2  mSquash ; // direction and magnitude
    // flags
    unsigned int mCollideWithContours : 1;
    unsigned int mPausePhysics : 1;
    unsigned int mCapVelocity : 1;
    uint32_t     mCollisionMask    = 1;
    // history (for ribbons)
    boost::circular_buffer<vec2> mHistory;
    
protected:
    float    mMass = 1.f ;
    
};

#endif /* Shape_h */
