#include "../RehabWorld.h"

#ifdef isUsingContours


PolyLine2 RehabWorld::getContainerContour(vec2 point){
    const Contour* in = mContours.findLeafContourContainingPoint(point ,mContourFilter) ;
    if (in){
        return in->mPolyLine;
    }
    return getWorldBoundsPoly();
}

void RehabWorld::drawAllContours(Color color){
    ContourVector& contours = getContours();
    gl::color(color);
    for(Contour c : contours){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD)
            gl::draw(c.mPolyLine);
    }
}
void RehabWorld::drawWorldBounds(Color color){
    gl::color(color);
    gl::draw(getWorldBoundsPoly());
}
void RehabWorld::drawContainerContour(vec2 point, Color color){
    gl::color(color);
    gl::draw(getContainerContour(point));
}

void RehabWorld::reconCubeInputs(){
    cubeInputs.clear();
    for(Contour c : mContours){
        if(c.mIsLeaf && c.mPolyLine.getPoints().size()==4 && c.mArea < MAX_AREA_THRESHOLD){
            cubeInputs.push_back(c);
        }
    }
}

ColorA RehabWorld::getMeanContourColor(Contour c){
    Mat1b img = sourceImage->mImageCV.getMat(ACCESS_RW);
    rectangle(img, cv::Rect(0,0,100,100), Scalar(100), CV_FILLED);
    //Rectangle definition
    cv::Point pts[1][4];
    pts[0][0] = cv::Point(c.mPolyLine.getPoints()[0].x,c.mPolyLine.getPoints()[0].y);
    pts[0][1] = cv::Point(c.mPolyLine.getPoints()[0].x,c.mPolyLine.getPoints()[1].y);
    pts[0][2] = cv::Point(c.mPolyLine.getPoints()[0].x,c.mPolyLine.getPoints()[2].y);
    pts[0][3] = cv::Point(c.mPolyLine.getPoints()[0].x,c.mPolyLine.getPoints()[3].y);
    
    const cv::Point* points [1] = {pts[0]};
    int npoints = 4;
    
    //Mask creation
    Mat1b mask(img.rows, img.cols, uchar(0));
    fillPoly(mask, points, &npoints, 1, Scalar(255));
    
    //Mean color of crop
    Scalar average = mean(img, mask);
    
    return ColorA(average.val[0], average.val[1], average.val[2]);
}

void RehabWorld::drawCubeInputs(Color color){
    gl::color(color);
    for(Contour c : cubeInputs){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD){
            gl::draw(c.mPolyLine);
            //gl::color(color.r, color.g, color.b, 0.4);
            //gl::drawSolidRect(Rectf(c.mPolyLine.getPoints()));
        }
    }
}
#endif
