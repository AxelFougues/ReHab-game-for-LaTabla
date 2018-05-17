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
            if(keyboardValidation){
                keyboardValidation = false;
                state = harvestersInput;
            }
            break;
            
        case harvestersInput:
            reconCubeInputs();
            if(cubeInputs.size() == 3 && keyboardValidation){
                registerHarvesterTokens(cubeInputs);
                state = harvest;
            }
            keyboardValidation = false;
            break;
            
        case harvest:
            board.harvest();
            state = results;
            break;
            
        case results:
            if(keyboardValidation)
                state = arrival;
            keyboardValidation = false;
            break;
    }
}

void RehabWorld::prepareToDraw(){ //Called before each draw call
    glEnable(GL_BLEND);
}

void RehabWorld::draw( DrawType drawType ){//Called many times per frame
    switch(state){
        case naturalistInput:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), true, true);
            drawGrid(Colorf(0, 1.0, 1.0));
            break;
            
        case harvestersInput:
            
            drawFillGrid(ColorA(0.0, 0.2, 0.0, 0.5), false, true);
            drawGrid(ColorA(0, 1.0, 1.0, 0.5));
            if(cubeInputs.size() == 3)
                drawCubeInputs(ColorA(0, 1, 0));
            else
                drawCubeInputs(ColorA(1, 0, 0));
            break;

        case results:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), true, false);
            drawGrid(Colorf(0, 1.0, 1.0));
            break;
    }
}



void RehabWorld::drawGrid(ColorA c){
    drawWorldBounds(c);
    gl::color(c);
    for (int i = 1; i < RehabGame::boardWidth; i++) {
        gl::drawLine(vec2(i*gridWidth, upperLeft.y), vec2(i*gridWidth, lowerLeft.y));
    }
    for (int i = 1; i < RehabGame::boardHeight; i++) {
        gl::drawLine(vec2(upperLeft.x, i*gridHeight), vec2(upperRight.x, i*gridHeight));
    }
}

void RehabWorld::drawFillGrid(ColorA c, bool birds, bool access){
    for (int y = 0; y < RehabGame::boardHeight; y++) {
        for (int x = 0; x < RehabGame::boardWidth; x++) {
            int bio = board.board[x][y].getBiomass();
            Rectf rect = Rectf(x * gridWidth, y * gridHeight, (x+1) * gridWidth, (y+1) * gridHeight);
            gl::color(c.r * bio, c.g * bio, c.b * bio, c.a);
            
            //BIOMASS DISPLAY
            gl::drawSolidRect(rect);
            //gl::color(1,1,1);
            //gl::drawString(to_string(board.board[x][y].getPopulation()), vec2(x * gridWidth +  2, y * gridHeight + 2));
            
            //BIRD DISPLAY
            if(birds){
                gl::color(1, 1, 1);
                float birdSize = 4;
                for (int i = 0; i < board.board[x][y].getPopulation(); i++) {
                    float xb = gridWidth * (x + 0.5) - (birdSize * ((float)board.board[x][y].getPopulation()/2) - birdSize / 2 ) + i * birdSize;
                    float yb = gridHeight * (y + 0.5);
                    gl::drawSolidCircle(vec2(xb, yb), birdSize / 3);
                }
            }
            //ACCCESS DISPLAY
            if(access && !board.board[x][y].getAccess()){
                gl::color(0.5, 0.1, 0.1, c.a);
                gl::drawSolidRoundedRect(rect, 3);
            }
        }
    }
}

void RehabWorld::registerHarvesterTokens(list<Contour> tokens){
    for(Contour c : tokens){
        vec2 gLoc = coordsToGrid(c.mCenter);
        board.board[(int)gLoc.x][(int)gLoc.y].addHarvesterToken();
    }
}

vec2 RehabWorld::coordsToGrid(vec2 coords){
    return vec2(int(coords.x/gridWidth),int(coords.y/gridHeight));
}

void RehabWorld::computeGrid(){
    gridWidth = (upperRight.x - upperLeft.x ) / RehabGame::boardWidth;
    gridHeight = (lowerLeft.y - upperLeft.y) /RehabGame::boardHeight;
}

void RehabWorld::naturalistClick(vec2 loc){
    loc  = coordsToGrid(loc);
    if(!(board.board[(int)loc.x][(int)loc.y].getAccess() && accessCount == 3)){
        if(board.board[(int)loc.x][(int)loc.y].changeAccess())
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
                keyboardValidation = true;
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









