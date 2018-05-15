//
//  RehabWorld.cpp
//  Tabla
//
//  Created by Axel Fougues on 09/04/2018.
//
//
#include "RehabWorld.h"
#include <ctime>

#ifdef isVisible
static GameCartridgeSimple sCartridge(nameOfWorld, [](){
    return std::make_shared<RehabWorld>();
});
#endif




void RehabWorld::initialize(){ //Called at world creation
    srand(time(NULL));
}

void RehabWorld::update(){ //Called each frame
    switch(state){
        case initial:
            computeFieldLayout();
            computeGrid();
            state = arrival;
            break;
            
        case arrival:
            board.nidificate();
            state = naturalistInput;
            accessCount = 0;
            
            break;
            
        case naturalistInput:
            playerTokens.clear();
            break;
            
        case harvestersInput:
            reconCubeInputs();
            if(playerTokens.size() == 3)
                state = harvest;
            break;
            
        case harvest:
            
            //state = results;
            break;
            
        case results:
            
            //state = arrival;
            break;
    }
}

void RehabWorld::prepareToDraw(){ //Called before each draw call
    
}

void RehabWorld::draw( DrawType drawType ){//Called many times per frame
    switch(state){
        case naturalistInput:
            drawFillGrid(Colorf(0.0, 0.2, 0.0), true, true);
            drawGrid(Colorf(0, 1.0, 1.0));
            break;
            
        case harvestersInput:
            drawFillGrid(Colorf(0.0, 0.2, 0.0), false, true);
            drawGrid(Colorf(0, 1.0, 1.0));
            
            break;
            
        case harvest:
            
            break;
            
        case results:
            
            break;
    }
}

void RehabWorld::drawGrid(ColorA c){
    drawWorldBounds(c);
    for (int i = gridWidth; i < RehabGame::boardWidth; i += gridWidth) {
        gl::drawLine(vec2(i, upperLeft.y), vec2(i, lowerLeft.y));
    }
    for (int i = gridHeight; i < RehabGame::boardHeight; i += gridHeight) {
        gl::drawLine(vec2(upperLeft.x, i), vec2(upperRight.x, i));
    }
}

void RehabWorld::drawFillGrid(ColorA c, bool birds, bool access){
    for (int y = 0; y < RehabGame::boardHeight; y++) {
        for (int x = 0; x < RehabGame::boardWidth; x++) {
            int bio = board.board[x][y].getBiomass();
            Rectf rect = Rectf(x * gridWidth, y * gridHeight, (x+1) * gridWidth, (y+1) * gridHeight);
            gl::color(c.r * bio, c.g * bio, c.b * bio);
            //BIOMASS DISPLAY
            gl::drawSolidRect(rect);
            //BIRD DISPLAY
            if(birds && board.board[x][y].isSettled()){
                gl::color(1, 1, 1);
                gl::drawSolidCircle(vec2(x * gridWidth + 2, y * gridHeight + 2), 1);
            }
            //ACCCESS DISPLAY
            if(access && !board.board[x][y].getAccess()){
                gl::color(0.5, 0.1, 0.1);
                gl::drawSolidRoundedRect(rect, 3);
            }
        }
    }
}

void RehabWorld::computeGrid(){
    gridWidth = (upperRight.x - upperLeft.x ) / RehabGame::boardWidth;
    gridHeight = (lowerLeft.y - upperLeft.y) /RehabGame::boardHeight;
}

void RehabWorld::naturalistClick(vec2 loc){
    int x = int(loc.x/gridWidth), y = int(loc.y/gridHeight);
    if(!(board.board[x][y].getAccess() && accessCount == 3)){
        if(board.board[x][y].changeAccess())
            accessCount--;
        else
            accessCount++;
        cout << accessCount << endl;
    }
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
            case KeyEvent::KEY_RETURN:
                if(state == naturalistInput)
                    state = harvestersInput;
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
        if(state == naturalistInput){
            naturalistClick(getMousePosInWorld());
        }
        cout << "Rehab mouse clicked." << endl;
    }


    void RehabWorld::drawMouseDebugInfo( vec2 position){
        
    }

	void RehabWorld::drawPointer(Color color, int size){
		gl::drawSolidCircle( getMousePosInWorld(), size);
	}
#endif










