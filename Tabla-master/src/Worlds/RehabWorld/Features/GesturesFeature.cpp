#include "../RehabWorld.h"

#ifdef isUsingGestures
float RehabWorld::angleBetweenPoints(vec2 p0, vec2 p1, vec2 p2 ){
    float a = pow((p1.x-p0.x),2) + pow((p1.y-p0.y),2);
    float b = pow((p1.x-p2.x),2) + pow((p1.y-p2.y),2);
    float c = pow((p2.x-p0.x),2) + pow((p2.y-p0.y),2);
    return acos( (a+b-c) / sqrt(4*a*b) ) * 180/M_PI;
}

float RehabWorld::distance(vec2 p1, vec2 p2){
    return abs(p1.x-p2.x)+abs(p1.y-p2.y);
}

bool RehabWorld::angleIsValey(vec2 p1, vec2 p2, vec2 p3, vec2 center){
    float dc = distance(p2, center);
    float angle = angleBetweenPoints(p1, p2, p3);
    return (distance(p1, center)> dc && distance(p3, center)> dc) && angle > VALEY_ANGLE_RANGE.x && angle < VALEY_ANGLE_RANGE.y;
}

bool RehabWorld::angleIsPeak(vec2 p1, vec2 p2, vec2 p3, vec2 center){
    float dc = distance(p2, center);
    float angle = angleBetweenPoints(p1, p2, p3);
    return (distance(p1, center)< dc && distance(p3, center)< dc) && angle > PEAK_ANGLE_RANGE.x && angle < PEAK_ANGLE_RANGE.y;
}

int RehabWorld::detectConsecutiveFingers(int roundness, Contour c){
    if(!c.mIsHole && c.mPolyLine.size()<CONTOUR_THRESHOLD){
        vector<vec2> points = c.mPolyLine.getPoints();
        vec2 center  = c.mCenter;
        int maxConsecutiveFingers = 0;
        int fingerCount = 0;
        bool valley, prevValey = false, peak, prevPeak = false, looped = false, nothing = true;
        int r = roundness;
        int index = 0, total = 0;
        while(index < points.size()-2){
            peak = angleIsPeak(points[index], points[index+1], points[index+2], center);
            valley = angleIsValey(points[index], points[index+1], points[index+2], center);
            
            if(peak && !prevValey){
                fingerCount = 1;
                total++;
                nothing = false;
            }
            
            if(peak && prevValey){
                fingerCount++;
                total++;
                if(maxConsecutiveFingers < fingerCount) maxConsecutiveFingers = fingerCount;
                nothing = false;
            }
            
            if(prevPeak && valley){
                prevValey = true;
                nothing = false;
            }else{
                prevValey = false;
            }
            
            if(nothing && r<=0){
                fingerCount = 0;
                r = roundness;
            }else if(nothing){
                r--;
            }
            
            prevPeak = peak;
            index++;
            
            if((peak || valley) && index == points.size()-2 && !looped){
                index = 0;
                looped = true;
            }
        }
        //if(total == maxConsecutiveFingers)
        return maxConsecutiveFingers;
        //else
        //return -total;
    }
    return 0;
}

int RehabWorld::totalFingers(int roundness, Contour c){
    return abs(detectConsecutiveFingers(roundness, c));
}

void RehabWorld::allGestureRecon(){
    wGestureRecon();
    oGestureRecon();
    vGestureRecon();
}

void RehabWorld::wGestureRecon(){
    wDetected = false;
    wGestures.clear();
    for(Contour c : mContours){
        if(detectConsecutiveFingers(3, c)>=4){
            wGestures.push_back(c);
            if(!wInputLock)
                wDetected = true;
        }
    }
    if(wGestures.empty()){
        wInputLock = false;
    }else{
        wInputLock = true;
    }
    
}

void RehabWorld::vGestureRecon(){
    vDetected = false;
    vGestures.clear();
    for(Contour c : mContours){
        if(detectConsecutiveFingers(3, c)==2){
            vGestures.push_back(c);
            if(!vInputLock)
                vDetected = true;
        }
    }
    if(vGestures.empty()){
        vInputLock = false;
    }else{
        vInputLock = true;
    }
}

void RehabWorld::oGestureRecon(){
    oDetected = false;
    oGestures.clear();
    for(Contour c : mContours){
        if(c.mIsHole && c.mPolyLine.size()<CONTOUR_THRESHOLD){
            //oGestures.push_back(mContours[c.mParent]);
            oGestures.push_back(c);
            if(!oInputLock)
                oDetected = true;
        }
    }
    if(oGestures.empty()){
        oInputLock = false;
    }else{
        oInputLock = true;
    }
}

void RehabWorld::drawAllGestures(Color color, bool details){
    drawOGestures(color, details);
    drawVGestures(color, details);
    drawWGestures(color, details);
}

void RehabWorld::drawOGestures(Color color, bool details){
    //if(oDetected){
    gl::color(color);
    for(Contour p : oGestures){
        gl::draw(p.mPolyLine);
        if(details){
            gl::drawString("O", p.mCenter);
            gl::drawSolidCircle(p.mCenter, 4);
        }
    }
    //}
}
void RehabWorld::drawVGestures(Color color, bool details){
    //if(vDetected){
    gl::color(color);
    for(Contour p : vGestures){
        gl::draw(p.mPolyLine);
        if(details){
            gl::drawString("V", p.mCenter);
            gl::drawSolidCircle(p.mCenter, 4);
        }
    }
    //}
}
void RehabWorld::drawWGestures(Color color, bool details){
    //if(wDetected){
    gl::color(color);
    for(Contour p : wGestures){
        gl::draw(p.mPolyLine);
        if(details){
            gl::drawString("W", p.mCenter);
            gl::drawSolidCircle(p.mCenter, 4);
        }
    }
    //}
}
#endif
