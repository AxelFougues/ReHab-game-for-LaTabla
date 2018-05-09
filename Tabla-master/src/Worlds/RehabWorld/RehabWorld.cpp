//
//  RehabWorld.cpp
//  Tabla
//
//  Created by Axel Fougues on 09/04/2018.
//
//
#include "RehabWorld.h"

#ifdef isVisible
static GameCartridgeSimple sCartridge(nameOfWorld, [](){
    return std::make_shared<RehabWorld>();
});
#endif




void RehabWorld::initialize(){ //Called at world creation
    
}

void RehabWorld::update(){ //Called each frame
    switch(state){
        case initial:
            state = arrival;
            break;
        case arrival:
            state = naturalistInput;
            break;
        case naturalistInput:
            state = harvestersInput;
            break;
        case harvestersInput:
            state = harvest;
            break;
        case harvest:
            state = results;
            break;
        case results:
            state = arrival;
            break;
    }
}

void RehabWorld::prepareToDraw(){ //Called before each draw call
    
}

void RehabWorld::draw( DrawType drawType ){//Called many times per frame
    drawWorldBounds(Colorf(0,1.0,1.0));
    drawAllContours(Colorf(1.0,1.0,0));
    drawAllGestures(Colorf(1.0,0,0), true);
    
}



#ifdef isUsingXml

    void RehabWorld::setParams( XmlTree xml ){
        //Get needed variables from xml file (assets/config/NameOfWorld.xml) Example:
        //getXml(xml,"ParentNode/ChildNode", variable);
        cout << "Rehab Xml loading works." << endl;
    }
#endif

//INPUT

#ifdef isUsingKeyboard

    void RehabWorld::keyDown( KeyEvent event){
        cout << "Rehab keyboard key down used." << endl;
        switch ( event.getCode() ){
            case KeyEvent::KEY_s:
                break ;
                
            case KeyEvent::KEY_b:
                break ;
        }
    }

    void RehabWorld::keyUp( KeyEvent event){
        cout << "Rehab keyboard key up used." << endl;
        switch ( event.getCode() ){
            case KeyEvent::KEY_a:
                break ;
                
            case KeyEvent::KEY_b:
                break ;
        }
    }
#endif

#ifdef isUsingGamepad

    void RehabWorld::gamepadEvent( const GamepadManager::Event& ) {
        cout << "Rehab gamepad used." << endl;
    }
#endif


#ifdef isUsingMouse
    /*Inherited:
     vec2 getMousePosInWorld();
        */


    void RehabWorld::mouseClick( vec2 event ){
        cout << "Rehab mouse clicked." << endl;
    }


    void RehabWorld::drawMouseDebugInfo( vec2 position){
        
    }

	void RehabWorld::drawPointer(Color color, int size){
		gl::drawSolidCircle( getMousePosInWorld(), size);
	}
#endif










