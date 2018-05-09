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

#include "Plant.hpp"
class RehabWorld : public GameWorld{
public:
    
    enum States{initial, arrival, naturalistInput, harvestersInput, harvest, results};
    int round = 0;
    States state = initial;
    
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
	ContourVector mContours;
	ContourVector::Filter mContourFilter;
    void updateVision( const Vision::Output& c, Pipeline& ) override { setContours(c.mContours); }
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
    list<vec2> getPointsFromInput();
#endif
    
#ifdef isUsingGestures
    vec2 VALEY_ANGLE_RANGE = vec2(20,80);
    vec2 PEAK_ANGLE_RANGE = vec2(2,60);
    
    vector<Contour> wGestures, vGestures, oGestures;
    bool wDetected = false, vDetected = false, oDetected = false;
    
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
	int	 mRibbonSampleRate= 1;
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

	ShapeShapeCollisionVec	ShapeShapeCollisions;
	ShapeContourCollisionVec	ShapeContourCollisions;
	ShapeWorldCollisionVec	ShapeWorldCollisions;
    const ShapeShapeCollisionVec&     getShapeShapeCollisions() const { return ShapeShapeCollisions; }
    const ShapeContourCollisionVec&  getShapeContourCollisions() const { return ShapeContourCollisions; }
    const ShapeWorldCollisionVec&    getShapeWorldCollisions() const { return ShapeWorldCollisions; }
    int	mNumIntegrationSteps	= 1;
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


