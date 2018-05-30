//
//  RehabWorld.hpp
//  Tabla
//
//  Created by Axel Fougues on 09/04/2018.
//
//

#ifndef TestWorld_hpp
#define TestWorld_hpp
#include <stdio.h>
#include "GameWorld.h"
#include "RehabGame.h"


/*###FEATURES###*/
//Uncomment the features you wish to use.
#define isVisible
//#define isUsingXml
//#define isUsingAssets
#define isUsingMouse
#define isUsingContours
//#define isUsingPhysics
#define isUsingVision
//#define isUsingUserSettings
//#define isUsingOrientation
#define isUsingKeyboard
//#define isUsingTime
//#define isUsingGamepad
//#define isUsingSound
//#define isUsingShapes
#define isUsingGestures


#include "cinder/Area.h"
#include "cinder/Rand.h"

#ifdef isUsingGestures
    #define _USE_MATH_DEFINES
    #include <cmath>
    #define isUsingContours
#endif
#ifdef isUsingPhysics
	#include "Utilities/geom.h"
	#define isUsingShapes
	#define isUsingContours
#endif
#ifdef isUsingShapes
	#include <boost/circular_buffer.hpp>
	#include "Shapes/Shape.h"
	#include "Shapes/Square.h"
	#include "Shapes/Ball.h"
#endif
#ifdef isUsingContours
	#include "Contour.h"
	#define isUsingVision
#endif
#ifdef isUsingXml
	#include "cinder/Xml.h"
#endif
static const string nameOfWorld = "RehabWorld";
using namespace std;

/*!
 * @class RehabWorld
 * @warning Should be refactored to the NameWorld you wish.
 */

class RehabWorld : public GameWorld{
public:
    
    enum States{initial, arrival, naturalistInput, harvestersInput, harvest, results, renew, end};
   
    int accessCount = 0, timer = 0;
    States state = initial;
    float gridWidth, gridHeight;
    RehabGame *board;
    bool keyboardValidation = false;
    
    /*!
     * @discussion Draws grid lines in chosen color.
     * @param c color used for drawing.
     * @param scale multiplicateur de taille.
     */
    void drawGrid(ColorA c, float scale);
    /*!
     * @discussion Initialises grid dimensions according to world size.
     */
    void computeGrid();
    /*!
     * @discussion Draws grid squares according to each cell's biomass.
     * @param c color used to represent biomass.
     * @param access display or not red rectangles on protected zones.
     * @param scale multiplicateur de taille.
     */
    void drawFillGrid(ColorA c,bool access, float scale);
    /*!
     * @discussion Draws birds as white dots in the middle of the cells.
     * @param c color used for drawing.
     */
    void drawBirds(ColorA c);
    /*!
     * @discussion Called at mouse click to set access of cells
     * @param loc Click location
     */
    void naturalistClick(vec2 loc);
    /*!
     * @discussion Translates world coordinates to cell indexes.
     * @param coords coordinates to translate.
     * @return translated coordinates.
     */
    vec2 coordsToGrid(vec2 coords);
    /*!
     * Checks if all necessary tokens are placed.
     */
    bool checkTokenValidity();
    /*!
     * @discussion Saves the tokens in the board for the next steps.
     * @param tokens the list of all validated tokens.
     */
    void registerHarvesterTokens(list<Contour> tokens);
    /*!
     * @discussion Token detection.
     */
    void findTokens();
    /*!
     * @discussion Sets token and player relationship.
     * @param tokens of corresponding type to player.
     * @param player index of player.
     */
    void distributeTokens(list<Contour> tokens, int player);
    /*!
     * @discussion Draws token contours.
     * @param c drawing color.
     */
    void drawTokens(Color color);
    /*!
     * @discussion Draws individual token score as a number on the token.
     * @param c drawing color.
     */
    void drawTokenScore(ColorA c);
    /*!
     * @discussion Draws all information of the end screen.
     */
    void drawEndScreen();
    
    
    //tokens collections used to store detected tokens according to type.
    list<Contour> type1Token; //square
    list<Contour> type2Token; //triangle
    list<Contour> type3Token; //toBeDetermined
    list<Contour> type4Token; //toBeDetermined
    
    //Universal
    void initialize();
    RehabWorld(){initializeTime();/*initialize();*/}
    string getSystemName() const override { return nameOfWorld; }
	/*!
	 * @discussion Only used to notify the app that the world is started.
	 */
    void gameWillLoad() override {};
	/*!
	 * @discussion Called at each frame, should be the main function to complete.
	 * @warning Please fill this function with your own code if used.
	 */
    void update() override;
	/*!
	 * @discussion Called before each draw() for pre-draw operations.
	 * @warning Please fill this function with your own code if used.
	 */
    void prepareToDraw() override;
	/*!
	 * @discussion Used for drawing the world, not frame-synced.
	 * @warning Please fill this function with your own code if used.
	 */
    void draw( DrawType ) override;
    
    vec2 mCenterLine[2];
    vec2 mCenter;        // c
    vec2 upperLeft; //0
    vec2 upperRight; //1
    vec2 lowerLeft; //3
    vec2 lowerRight; //2
    
    vec2 fieldCorner( int i ) const{
        int j = i ;
        return getWorldBoundsPoly().getPoints()[ j % getWorldBoundsPoly().getPoints().size() ];
    }
    
    void computeFieldLayout(){
        assert( getWorldBoundsPoly().size()==4 );
        PolyLine2 p = getWorldBoundsPoly();
        upperLeft = p.getPoints()[0];
        upperRight = p.getPoints()[1];
        lowerRight = p.getPoints()[2];
        lowerLeft = p.getPoints()[3];
        mCenterLine[0] = lerp(upperLeft, upperRight, .5f);
        mCenterLine[1] = lerp(lowerRight, lowerRight, .5f);
        mCenter = lerp( mCenterLine[0], mCenterLine[1], .5f );
    }

#ifdef isUsingXml
    /*!
     * @discussion Called by the app, use getXml(xml,"ParentNode/ChildNode", variable) to
     * import from the corresponding XmlTree (Same name as the class in assets/config).
     * @param xml Is handled by the app.
     * @warning Please fill this function with your own code if used.
     */
    void setParams( XmlTree ) override;
#endif
    
#ifdef isUsingVision
    Pipeline::StageRef sourceImage;
    ContourVector mContours;
    ContourVector::Filter mContourFilter;
    void updateVision( const Vision::Output& c, Pipeline& pipeline) override{
        setContours(c.mContours);
        sourceImage = pipeline.getStage("clipped");
    }
    void setContours( const ContourVec& contours, ContourVec::Filter filter=0 ) { mContours = contours; mContourFilter=filter; }
#endif
    
#ifdef isUsingOrientation
    vec2  mDefaultUpVec = vec2(0,1);
    vec2  mUpVec = vec2(0,1);
    float mGravity=0.1f;
    void    initSettings() override;
    XmlTree getUserSettings() const override;
    void    setUserSettings( XmlTree ) override;
    virtual map<string,vec2> getOrientationVecs() const override;
    virtual void setOrientationVec ( string, vec2 ) override;
    /*!
     * @discussion If Orientation is on, gets the up direction
     * @return vec Representing the up vector.
     */
    vec2 getUpVec() const { return mUpVec; }
    /*!
     * @discussion If Orientation is on, gets the left direction
     * @return vec Representing the left vector.
     */
    vec2 getLeftVec() const { return vec2(cross(vec3(mUpVec,0),vec3(0,0,1))); }
    /*!
     * @discussion If Orientation is on, gets the right direction
     * @return vec Representing the right vector.
     */
    vec2 getRightVec() const { return -getLeftVec(); }
    /*!
     * @discussion If Orientation is on, gets the down direction
     * @return vec Representing the down vector.
     */
    vec2 getDownVec() const { return -mUpVec; }
#endif
    
    
private:
    
    float worldEnterTime;
    void initializeTime(){worldEnterTime = ci::app::getElapsedSeconds();cout << "Time initialized." << endl;}
    
    
#ifdef isUsingTime
    /*!
     * @discussion If Orientation is on, gets the down direction
     * @return vec Representing the down vector.
     */
    float getTimeSinceStart();
    /*!
     * @discussion Gets time in seconds since the app started.
     * @return float seconds.
     */
    float getTime(){return ci::app::getElapsedSeconds();}
#endif
    
    
#ifdef isUsingContours
    int CONTOUR_THRESHOLD = 20;
    int MAX_AREA_THRESHOLD = 4000;
    list<Contour> cubeInputs;
    
    /*!
     * @discussion If contours activated, draws all contours.
     * @param color Color used for drawing.
     */
    PolyLine2 getContainerContour(vec2 point);
    
    /*!
     * @discussion If contours activated, gets all contours.
     */
    ContourVector& getContours() { return mContours; }
    
    const ContourVector& getContours() const { return mContours; }
    
    /*!
     * @discussion If contours activated, draws all contours.
     * @param color Color used for drawing.
     */
    void drawAllContours(Color color);
    
    /*!
     * @discussion If contours activated, draws the world bounds (edges).
     * @param color Color used for drawing.
     */
    void drawWorldBounds(Color color);
    
    /*!
     * @discussion If contours activated, draws the contour containing point.
     * @param point X and Y of point to check.
     * @param color Color used for drawing.
     */
    void drawContainerContour(vec2 point, Color color);
    
    /*!
     * @discussion If contours activated, gets points from placed cubes.
     */
    void reconCubeInputs();
    
    /*!
     * @discussion If contours activated, draws all active cube inputs.
     * @param color Color used for drawing.
     */
    void drawCubeInputs(Color color);
    
    /*!
     * @discussion If contours activated, gets mean color of pixels contained by given contour.
     * @param c contour to be used.
     * @return The mean color.
     */
    ColorA getMeanContourColor(Contour c);
#endif
    
#ifdef isUsingGestures
    vec2 VALEY_ANGLE_RANGE = vec2(20,80);
    vec2 PEAK_ANGLE_RANGE = vec2(2,60);
    
    vector<Contour> wGestures, vGestures, oGestures;
    bool wDetected = false, vDetected = false, oDetected = false;
    bool wInputLock = true, vInputLock = true, oInputLock = true;
    
    /*!
     * @discussion If gestures activated, draws all the gestures.
     * @param color Color used for drawing.
     * @param details If true draws gesture center and type.
     */
    void drawAllGestures(Color color, bool details);
    
    /*!
     * @discussion If gestures activated, detects gestures.
     */
    void allGestureRecon();
    
    /*!
     * @discussion If gestures activated, detects gestures.
     */
    void oGestureRecon();
    
    /*!
     * @discussion If gestures activated, detects gestures.
     */
    void vGestureRecon();
    
    /*!
     * @discussion If gestures activated, detects gestures.
     */
    void wGestureRecon();
    
    /*!
     * @discussion If gestures activated, draws "O" gestures.
     * @param color Color used for drawing.
     * @param details If true draws gesture center and type.
     */
    void drawOGestures(Color color, bool details);
    
    /*!
     * @discussion If gestures activated, draws "V" gestures.
     * @param color Color used for drawing.
     * @param details If true draws gesture center and type.
     */
    void drawVGestures(Color color, bool details);
    
    /*!
     * @discussion If gestures activated, draws "W" gestures.
     * @param color Color used for drawing.
     * @param details If true draws gesture center and type.
     */
    void drawWGestures(Color color, bool details);
    
    int totalFingers(int roundness, Contour c);
    int detectConsecutiveFingers(int roundness, Contour c);
    bool angleIsValey(vec2 p1, vec2 p2, vec2 p3, vec2 center);
    bool angleIsPeak(vec2 p1, vec2 p2, vec2 p3, vec2 center);
    float angleBetweenPoints(vec2 p0, vec2 p1, vec2 p2 );
    float distance(vec2 p1, vec2 p2);
#endif
    
#ifdef isUsingWidgets
    class Widget{
    public:
        Widget(){}
        Widget(vec2 ul, vec2 lr){
            upperLeft = ul;
            lowerRight = lr;
            setCenter();
            setRect();
        }
        void updateBar(vec2 ul, vec2 lr){
            upperLeft = ul;
            lowerRight = lr;
            setCenter();
            setRect();
        }
        
        bool triggered = false;
        vec2 trigger;
        vec2 upperLeft;
        vec2 lowerRight;
        vec2 center;
        Rectf rect;
        
        void setCenter(){center = vec2(lowerRight.x - upperLeft.x / 2, lowerRight.y - upperLeft.y / 2);}
        void setRect(){rect = Rectf(upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y);}
        void drawWidget(ColorA color){gl::color(color);gl::drawSolidRect(rect);}
        //void drawWidgetContour(ColorA color){gl::color(color);gl::drawRect(Rectf(upperLeft.x, upperLeft.y, lowerRight.x, lowerRight.y));};
    };
    class SideBar : public Widget{
    public:
        SideBar(){}
        SideBar(vec2 ul, vec2 lr) : Widget(ul, lr){};
        
        void checkBarWidgetTriggers(vector<Contour> o){
            triggered = false;
            for(Contour c : o){
                if(rect.contains(c.mCenter)){
                    triggered = true;
                    trigger = c.mCenter;
                }
            }
        }
        
    };
    
    SideBar horizontalBar;
    SideBar verticalBar;
    
#endif
    
#ifdef isUsingOrientation
    vec2 mCenterLine[2];
    vec2 mCenter;        // c
    vec2 upperLeft; //0
    vec2 upperRight; //1
    vec2 lowerLeft; //3
    vec2 lowerRight; //2
    
    vec2 fieldCorner( int i ) const{
        int j = i ;
        return getWorldBoundsPoly().getPoints()[ j % getWorldBoundsPoly().getPoints().size() ];
    }
    
    void computeFieldLayout(){
        assert( getWorldBoundsPoly().size()==4 );
        PolyLine2 p = getWorldBoundsPoly();
        upperLeft = p.getPoints()[0];
        upperRight = p.getPoints()[1];
        lowerRight = p.getPoints()[2];
        lowerLeft = p.getPoints()[3];
        mCenterLine[0] = lerp(upperLeft, upperRight, .5f);
        mCenterLine[1] = lerp(lowerRight, lowerRight, .5f);
        mCenter = lerp( mCenterLine[0], mCenterLine[1], .5f );
    }
    
#endif
    
    
#ifdef isUsingMouse
    /*!
     * @discussion Event function called on any mouse click.
     * @param event Location of the mouse when clicked.
     * @warning Please fill this function with your own code if used.
     */
    void mouseClick( vec2 position) override;
    /*!
     * @discussion Use this for mouse debuging.
     * @param event Location of the mouse.
     * @warning Please fill this function with your own code if used.
     */
    void drawMouseDebugInfo( vec2 position) override;
    /*!
     * @discussion Used to draw a visible mouse pointer in the world, a solid circle by default.
     * @param color Color of the pointer.
     * @param size Diameter of the pointer.
     * @warning Please fill this function with your own code if used.
     */
    void drawPointer(Color color, int size);
#endif
    
#ifdef isUsingKeyboard
    /*!
     * @discussion Event function called on any keyboard press. Use event.getCode().
     * @param event contains information about the key.
     * @warning Please fill this function with your own code if used.
     */
    void keyDown( KeyEvent ) override;
    /*!
     * @discussion Event function called on any keyboard release. Use event.getCode().
     * @param event contains information about the key.
     * @warning Please fill this function with your own code if used.
     */
    void keyUp( KeyEvent ) override;
#endif
    
#ifdef isUsingGamepad
    /*!
     * @discussion Event function called on any gamepad action. Use event.getCode().
     * @param event contains information about the action.
     * @warning Please fill this function with your own code if used.
     */
    void gamepadEvent( const GamepadManager::Event& ) override;
#endif
    
#ifdef isUsingShapes
    bool mRibbonEnabled = false;
    int     mRibbonSampleRate= 1;
    int mRibbonMaxLenght = 10;
    vector<Shape*> shapes ;
    /*!
     * @discussion Add a default basic shape to world.
     * @param loc Initial location of the shape.
     */
    Shape* newDefaultShape(vec2 loc);
    /*!
     * @discussion Removes all world shapes permanently.
     */
    void clearShapes(){shapes.clear();};
    /*!
     * @discussion Add shape to world.
     * @param shape The shape to be added.
     */
    Shape* addShape(Shape* shape);
    /*!
     * @discussion Returns index of given shape.
     * @param index Index of shape if found.
     */
    int getShapeIndex( const Shape* s ) const;
    /*!
     * @discussion Remove shape by index.
     * @param index Index of shape in list of world.
     */
    void removeShape(int index);
    /*!
     * @discussion Gets all world shapes.
     * @return vector<Shape>& Vector of all shapes, could be empty.
     */
    vector<Shape*>& getShapes() { return shapes; }
    const vector<Shape*>& getShapes() const { return shapes; }
    /*!
     * @discussion Draws all world shapes.
     */
    void drawShapes() const;
    void drawRibbons( DrawType ) const;
    void accumulateShapeHistory();
#endif
    
#ifdef isUsingPhysics
    float mShapeMaxVel = .2f ;
    float   mShapeContourImpactNormalVelImpulse = .1f;
    float   mShapeContourCoeffOfRestitution = 1.f;
    float   mShapeContourFrictionlessCoeff = 1.f;
    /*!
     * @discussion Should be called at each frame if physics is used.
     */
    void updatePhysics();
    vec2 resolveCollisionWithContours ( vec2 p, float r, const Shape* s=0 );
    vec2 resolveCollisionWithInverseContours( vec2 p, float r, const Shape* s=0 );
    vec2 getDenoisedShapeVel( const Shape& ) const;
    struct ShapeShapeCollision {
    public:
        ShapeShapeCollision( int a=-1, int b=-1 ) { mShapeIndex[0]=a; mShapeIndex[1]=b; }
        int mShapeIndex[2];
    };
    typedef vector<ShapeShapeCollision> ShapeShapeCollisionVec;
    
    struct ShapeContourCollision {
    public:
        ShapeContourCollision( int a, int b, vec2 pt )
        : mShapeIndex(a)
        , mContourIndex(b)
        , mPt(pt) {}
        
        ShapeContourCollision() : ShapeContourCollision(-1,-1,vec2(0.f)){}
        
        int mShapeIndex;
        int mContourIndex;
        vec2 mPt;
    };
    typedef vector<ShapeContourCollision> ShapeContourCollisionVec;
    
    struct ShapeWorldCollision {
    public:
        ShapeWorldCollision( int a=-1, vec2 pt=vec2(0.f) ) { mShapeIndex=a; mPt=pt; }
        int mShapeIndex;
        vec2 mPt;
    };
    typedef vector<ShapeWorldCollision> ShapeWorldCollisionVec;
    
    ShapeShapeCollisionVec    ShapeShapeCollisions;
    ShapeContourCollisionVec    ShapeContourCollisions;
    ShapeWorldCollisionVec    ShapeWorldCollisions;
    const ShapeShapeCollisionVec&     getShapeShapeCollisions() const { return ShapeShapeCollisions; }
    const ShapeContourCollisionVec&  getShapeContourCollisions() const { return ShapeContourCollisions; }
    const ShapeWorldCollisionVec&    getShapeWorldCollisions() const { return ShapeWorldCollisions; }
    int    mNumIntegrationSteps    = 1;
    int mLastNumIntegrationSteps = 1;
    int getNumIntegrationSteps() const { return mNumIntegrationSteps; }
    vec2 unlapEdge( vec2 p, float r, const Contour& poly, const Shape* s=0 );
    vec2 unlapHoles( vec2 p, float r, ContourKind kind, const Shape* s=0 );
    void onShapeShapeCollide( const Shape& a, const Shape& b );
    void onShapeContourCollide( const Shape& a, const Contour& b, vec2 pt );
    void onShapeWorldBoundaryCollide  ( const Shape& a, vec2 pt );
    void scaleShapeVelsForIntegrationSteps( int oldSteps, int newSteps );
    void resolveShapeContourCollisions();
    void resolveShapeCollisions() ;
#endif
    
};
#endif


