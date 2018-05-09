#include "../BlueprintWorld.h"

#ifdef isUsingPhysics
vec2 BlueprintWorld::resolveCollisionWithContours ( vec2 point, float radius, const Shape* s ){
    const Contour* in = mContours.findLeafContourContainingPoint(point,mContourFilter) ;
    if (in){
        if ( !in->mIsHole ){
            vec2 p = point ;
            p = unlapEdge ( p, radius, *in, s ) ; // get off an edge we might be on
            p = unlapHoles( p, radius, ContourKind::Holes, s ) ; // make sure we aren't in a hole
            return p ;
        }else{
            // push us out of this hole
            vec2 x = closestPointOnPoly(point, in->mPolyLine) ;
            if (s) onShapeContourCollide( *s, *in, x );
            return glm::normalize( x - point ) * radius + x ;
        }
    }else{
        // inside of no contour
        // push us into nearest paper
        vec2 x ;
        const Contour* nearest = mContours.findClosestContour(point, &x, 0, ContourVec::getAndFilter( ContourVec::getKindFilter(ContourKind::NonHoles), mContourFilter ));
        
        if ( nearest ){
            if (s)
                onShapeContourCollide( *s, *nearest, x );
            return glm::normalize( x - point ) * radius + x ;
        }else
            return point;
    }
}

vec2 BlueprintWorld::resolveCollisionWithInverseContours( vec2 point, float radius, const Shape* s ){
    const Contour* in = mContours.findLeafContourContainingPoint(point,mContourFilter) ;
    // ok, find closest
    if (in){
        if ( !in->mIsHole ){
            // in paper
            // push us out of this contour
            bool pushOut = true ;
            vec2 x1 = closestPointOnPoly(point, in->mPolyLine) ;
            // but what if it's better to get us into a smaller hole inside?
            vec2 x2 ;
            float x2dist;
            const Contour* interiorHole = mContours.findClosestContour(point,&x2,&x2dist, ContourVec::getAndFilter( ContourVec::getKindFilter(ContourKind::Holes), mContourFilter ));
            if ( interiorHole ){
                // this is closer, so replace x1
                if ( x2dist < glm::distance(x1,point) ){
                    pushOut = false;
                    x1 = x2;
                }
            }
            // compute fix
            point = glm::normalize( x1 - point ) * radius + x1 ;
            // note
            if (s)
                onShapeContourCollide(*s, pushOut ? *in : *interiorHole, x1 );
        }else{
            // in hole
            vec2 p = point ;
            p = unlapEdge ( p, radius, *in, s ) ;
            p = unlapHoles( p, radius, ContourKind::NonHoles, s ) ;
            point = p ;
        }
    }else{
        // in empty space
        // make sure we aren't grazing a contour
        vec2 x;
        float dist;
        const Contour * nearest = mContours.findClosestContour(point, &x, &dist, ContourVec::getAndFilter( ContourVec::getKindFilter(ContourKind::NonHoles), mContourFilter ));
        
        if ( nearest && dist < radius ){
            point = glm::normalize( point - x ) * radius + x ;
            if (s)
                onShapeContourCollide(*s,*nearest,x);
        }
        // make sure we are inside the world (not floating away)
        if ( getWorldBoundsPoly().size() > 0 ){
            vec2 x1 = closestPointOnPoly( point, getWorldBoundsPoly(), 0, 0, &dist );
            if ( getWorldBoundsPoly().contains(point) ){
                if ( dist < radius ){
                    point = glm::normalize( point - x1 ) * radius + x1 ;
                    if (s)
                        onShapeWorldBoundaryCollide(*s,x1);
                }
            }else{
                point = glm::normalize( x1 - point ) * radius + x1 ;
                if (s)
                    onShapeWorldBoundaryCollide(*s,point);
            }
        }
    }
    return point;
}

vec2 BlueprintWorld::getDenoisedShapeVel( const Shape& s) const{
    vec2 vel;
    const int n = s.mHistory.size();
    if ( 1 && n>1 ){
        vel = s.mHistory[n-1] - s.mHistory[n-2];
        vel /= (float)getNumIntegrationSteps(); // make it coherent with mVel, which is scaled to time steps
    }else
        vel = s.getVel();
    return vel;
}

void BlueprintWorld::updatePhysics(){
    ShapeShapeCollisions.clear();
    ShapeContourCollisions.clear();
    ShapeWorldCollisions.clear();
    scaleShapeVelsForIntegrationSteps( mLastNumIntegrationSteps, mNumIntegrationSteps );
    mLastNumIntegrationSteps=mNumIntegrationSteps;
    const int   steps = mNumIntegrationSteps ;
    const float delta = 1.f / (float)steps ;
    for( int step=0; step<steps; ++step ){
        for( auto &b : shapes ){
            if (!b->mPausePhysics){
                b->mLoc += (b->mAccel * (float)steps) * delta*delta ;
                b->mAccel = vec2(0,0) ;
            }
        }
        resolveShapeContourCollisions();
        resolveShapeCollisions() ;
        if (1){
            const float maxVel = mShapeMaxVel * delta;
            
            for( auto &b : shapes ){
                if (b->mCapVelocity){
                    vec2 v = b->getVel() ;
                    if ( length(v) > maxVel ){
                        b->setVel( normalize(v) * maxVel ) ;
                    }
                }
            }
        }
        // inertia
        for( auto &b : shapes ){
            if (!b->mPausePhysics) {
                vec2 vel = b->getVel() ; // rewriting mLastLoc will stomp vel, so get it first
                b->mLastLoc = b->mLoc ;
                b->mLoc += vel ;
            }
        }
    }
    // un-squash
    for( auto &b : shapes ){
        b->mSquash *= .7f ;
    }
    // history
    if (mRibbonEnabled) accumulateShapeHistory();
}

vec2 BlueprintWorld::unlapEdge( vec2 p, float r, const Contour& poly, const Shape* s ){
    float dist ;
    vec2 x = closestPointOnPoly( p, poly.mPolyLine, 0, 0, &dist );
    if ( dist < r ){
        if (s)
            onShapeContourCollide( *s, poly, x );
        return glm::normalize( p - x ) * r + x ;
    }
    else return p ;
}

vec2 BlueprintWorld::unlapHoles( vec2 p, float r, ContourKind kind, const Shape* s ){
    float dist ;
    vec2 x ;
    const Contour * nearestHole = mContours.findClosestContour( p, &x, &dist,
                                                               ContourVec::getAndFilter( ContourVec::getKindFilter(kind), mContourFilter ) );
    if ( nearestHole && dist < r && !nearestHole->mPolyLine.contains(p) ){
        // ensure we aren't actually in this hole or that would be bad...
        if (s)
            onShapeContourCollide( *s, *nearestHole, x );
        return glm::normalize( p - x ) * r + x ;
    }
    else return p ;
}
void BlueprintWorld::onShapeShapeCollide( const Shape& a, const Shape& b ){
    ShapeShapeCollisions.push_back( ShapeShapeCollision(getShapeIndex(&a),getShapeIndex(&b)));
}

void BlueprintWorld::onShapeContourCollide( const Shape& a, const Contour& b, vec2 pt ){
    ShapeContourCollisions.push_back(ShapeContourCollision( getShapeIndex(&a), mContours.getIndex(b), pt ));
}

void BlueprintWorld::onShapeWorldBoundaryCollide  ( const Shape& a, vec2 pt ){
    ShapeWorldCollisions.push_back( ShapeWorldCollision(getShapeIndex(&a),pt) );
}

void BlueprintWorld::scaleShapeVelsForIntegrationSteps( int oldSteps, int newSteps ){
    if (oldSteps==newSteps) return;
    float scale = (float)oldSteps / (float)newSteps;
    for( auto &b : shapes ){
        b->setVel( b->getVel() * scale );
    }
}

void BlueprintWorld::resolveShapeContourCollisions(){
    for( auto &b : shapes ){
        if (!b->mCollisionMask) continue;
        vec2 oldVel = b->getVel() ;
        vec2 oldLoc = b->mLoc ;
        vec2 newLoc;
        if (b->mCollideWithContours)
            newLoc = resolveCollisionWithContours( b->mLoc, b->mScale, b ) ;
        else
            newLoc = resolveCollisionWithInverseContours( b->mLoc, b->mScale, b ) ;
        if ( newLoc != oldLoc ){
            b->mLoc = newLoc ;
            vec2 surfaceNormal = glm::normalize( newLoc - oldLoc ) ;
            vec2 newVel = glm::reflect( oldVel, surfaceNormal );
            if (mShapeContourCoeffOfRestitution < 1.f || mShapeContourFrictionlessCoeff < 1.f){
                vec2 normalVel  = surfaceNormal * dot(newVel,surfaceNormal);
                vec2 tangentVel = newVel - normalVel;
                tangentVel *= mShapeContourFrictionlessCoeff;
                newVel = tangentVel + normalVel * mShapeContourCoeffOfRestitution;
            }
            b->mAccel += surfaceNormal * mShapeContourImpactNormalVelImpulse;
            b->setVel(newVel);
            b->noteSquashImpact( surfaceNormal * length(b->getVel()) ) ;
        }
    }
}
void BlueprintWorld::resolveShapeCollisions(){
    if ( shapes.size()==0 )
        return ;
    for( size_t i=0  ; i<shapes.size()-1; i++ ){
        auto &a = shapes[i] ;
        if ( !a->mCollisionMask )
            continue;
        for( size_t j=i+1; j<shapes.size()  ; j++ ){
            auto &b = shapes[j] ;
            if ( !(a->mCollisionMask & b->mCollisionMask) )
                continue;
            float d  = glm::distance(a->mLoc,b->mLoc) ;
            float rs = a->mScale + b->mScale ;
            if ( d < rs ){
                vec2 a2b ;
                if (d==0.f)
                    a2b = Rand::randVec2() ;
                else
                    a2b = glm::normalize( b->mLoc - a->mLoc ) ;
                float overlap = rs - d ;
                const vec2 avel = a->getVel() ;
                const vec2 bvel = b->getVel() ;
                const float ma = a->getMass() ;
                const float mb = b->getMass() ;
                const float amass_frac = ma / (ma+mb) ;
                const float bmass_frac = 1.f - amass_frac ;
                b->mLoc +=  a2b * overlap * amass_frac ;
                a->mLoc += -a2b * overlap * bmass_frac ;
                const float avelp = dot( avel, a2b ) ;
                const float bvelp = dot( bvel, a2b ) ;
                float avelp_new ;
                float bvelp_new ;
                if (0){
                    avelp_new = bvelp ;
                    bvelp_new = avelp ;
                }else{
                    float cr = 1.f ;
                    avelp_new = (cr * mb * (bvelp - avelp) + ma*avelp + mb*bvelp) / (ma+mb) ;
                    bvelp_new = (cr * ma * (avelp - bvelp) + ma*avelp + mb*bvelp) / (ma+mb) ;
                }
                const vec2 avel_new = avel + a2b * ( avelp_new - avelp ) ;
                const vec2 bvel_new = bvel + a2b * ( bvelp_new - bvelp ) ;
                a->setVel(avel_new) ;
                b->setVel(bvel_new) ;
                a->noteSquashImpact( avel_new - avel ) ;
                b->noteSquashImpact( bvel_new - bvel ) ;
                onShapeShapeCollide(*a,*b);
            }
        }
    }
}
#endif
