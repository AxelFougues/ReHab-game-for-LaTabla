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
            board = new RehabGame();
            computeFieldLayout();
            computeGrid();
            accessCount = 0;
            state = arrival;
            break;
            
        case arrival:
            board->nidificate();
            timer++;
            if(timer >= 50){
                timer = 0;
                state = naturalistInput;
            }
            break;
            
        case naturalistInput:
            if(keyboardValidation){
                keyboardValidation = false;
                state = harvestersInput;
            }
            break;
            
        case harvestersInput:
            if(checkTokenValidity() && keyboardValidation){
                registerHarvesterTokens(cubeInputs);
                board->harvest();
                board->attributePoints();
                state = harvest;
            }
            keyboardValidation = false;
            break;
            
        case harvest:
            
            if(keyboardValidation){
                board->reproduce();
                state = results;
            }
            keyboardValidation = false;
            break;
            
        case results:
            if(keyboardValidation)
                state = renew;
            keyboardValidation = false;
            break;
            
        case renew:
            board->renew();
            state = arrival;
            break;
        
        case end:
            if(keyboardValidation)
                state = initial;
            keyboardValidation = false;
            break;
    }
}

void RehabWorld::prepareToDraw(){ //Called before each draw call
}

void RehabWorld::draw( DrawType drawType ){//Called many times per frame
    float opacity = 0.5;
    switch(state){
            
        case arrival:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), true, 1);
            break;
            
        case naturalistInput:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), true, 1);
            drawBirds(ColorA(1,1,1,1));
            drawGrid(Colorf(0, 1.0, 1.0),1);
            break;
            
        case harvestersInput:
            drawFillGrid(ColorA(0.0, 0.2, 0.0, opacity), true, 1);
            drawGrid(ColorA(0, 1.0, 1.0, opacity),1);
            if(checkTokenValidity())
                drawTokens(ColorA(0, 1, 0));
            else
                drawTokens(ColorA(1, 0, 0));
            break;
        
        case harvest:
            drawFillGrid(ColorA(0, 0.2, 0.0, opacity), false, 1);
            drawGrid(Colorf(0, 1.0, 1.0), 1);
            drawTokenScore(Colorf(1, 1, 1));
            break;

        case results:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), false, 1);
            drawBirds(ColorA(1,1,1,1));
            drawGrid(Colorf(0, 1.0, 1.0),1);
            break;
            
            case end:
            drawFillGrid(ColorA(0.0, 0.2, 0.0), false, 0.3);
            drawGrid(Colorf(0, 1.0, 1.0), 0.3);
            drawEndScreen();
            break;
    }
}



void RehabWorld::drawGrid(ColorA c, float scale){
    drawWorldBounds(c);
    float displacementX = (1-scale) / 2 * lowerRight.x - lowerLeft.x;
    float displacementY = (1-scale) / 2 * lowerRight.y - upperRight.y;
    gl::color(c);
    for (int i = 1; i < RehabGame::boardWidth; i++) {
        gl::drawLine(vec2(i*gridWidth * scale + displacementX, upperLeft.y * scale + displacementY), vec2(i*gridWidth * scale + displacementX, lowerLeft.y * scale + displacementY));
    }
    for (int i = 1; i < RehabGame::boardHeight; i++) {
        gl::drawLine(vec2(upperLeft.x * scale + displacementX, i*gridHeight * scale + displacementY), vec2(upperRight.x * scale + displacementX, i*gridHeight * scale + displacementY));
    }
}

void RehabWorld::drawFillGrid(ColorA c, bool access, float scale){
    float displacementX = (1-scale) / 2 * lowerRight.x - lowerLeft.x;
    float displacementY = (1-scale) / 2 * lowerRight.y - upperRight.y;
    for (int y = 0; y < RehabGame::boardHeight; y++) {
        for (int x = 0; x < RehabGame::boardWidth; x++) {
            int bio = board->board[x][y].getBiomass();
            Rectf rect = Rectf(
                               x * gridWidth * scale + displacementX,
                               y * gridHeight * scale + displacementY,
                               (x+1) * gridWidth * scale + displacementX,
                               (y+1) * gridHeight * scale + displacementY);
            gl::color(c.r * bio, c.g * bio, c.b * bio, c.a);
            
            //BIOMASS DISPLAY
            gl::drawSolidRect(rect);
            //gl::color(1,1,1);
            //gl::drawString(to_string(bio), vec2(x * gridWidth +  2, y * gridHeight + 2));
            
            //ACCCESS DISPLAY , protected area
            if(access && !board->board[x][y].getAccess()){
                gl::color(0.5, 0.1, 0.1, c.a);
                gl::drawSolidRoundedRect(rect, 5);
            }
        }
    }
}

void RehabWorld::drawBirds(ColorA c){
    gl::color(c);
    float birdSize = 4;
    for (int y = 0; y < RehabGame::boardHeight; y++) {
        for (int x = 0; x < RehabGame::boardWidth; x++) {
            for (int i = 0; i < board->board[x][y].getPopulation(); i++) {
                float xb = gridWidth * (x + 0.5) - (birdSize * ((float)board->board[x][y].getPopulation()/2) - birdSize / 2 ) + i * birdSize;
                float yb = gridHeight * (y + 0.5);
                gl::drawSolidCircle(vec2(xb, yb), birdSize / 3);
            }
        }
    }
}

void RehabWorld::drawTokenScore(ColorA c){
    gl::color(c);
    for (int y = 0; y < RehabGame::boardHeight; y++) {
        for (int x = 0; x < RehabGame::boardWidth; x++) {
            for(Token *t : board->board[x][y].getHarvesterTokens()){
                gl::drawString(to_string((t->currentValue)), t->pos);
            }
        }
    }
}

void RehabWorld::drawEndScreen(){
    //GARPH DISPLAY
    gl::color(1, 0.5, 0.5);
    int i = 0;
    float barWidth = (lowerRight.x - lowerLeft.x)/board->animalPopulation.size();
    for(int p : board->animalPopulation){
        Rectf rect = Rectf(lowerLeft.x + (i * barWidth), lowerLeft.y, lowerLeft.x + ((i+1) * barWidth), lowerLeft.y - p);
        gl::drawSolidRect(rect);
        i++;
    }
    //STATS DISPLAY
    gl::color(1, 1, 1);
    for(i = 0; i < RehabGame::nbPlayers; i++){
        gl::drawString("Player " + to_string(i+1) + ":  " + to_string(board->harvesters[i]->getTotalScore()) , vec2(20, 10+i*15));
    }
    i = 0;
    gl::drawString("Births :  " + to_string(board->births) , vec2(upperRight.x / 2 + 10, 10+i*15));
    i++;
    gl::drawString("Turns :  " + to_string(board->animalPopulation.size()) , vec2(upperRight.x / 2 + 10, 10+i*15));
}

void RehabWorld::drawTokens(Color color){
    gl::color(color);
    for(Contour c : type1Token){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD){
            gl::draw(c.mPolyLine);
        }
    }
    for(Contour c : type2Token){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD){
            gl::draw(c.mPolyLine);
        }
    }
    for(Contour c : type3Token){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD){
            gl::draw(c.mPolyLine);
        }
    }
    for(Contour c : type4Token){
        if(c.mPolyLine.size()<CONTOUR_THRESHOLD){
            gl::draw(c.mPolyLine);
        }
    }
    
}

void RehabWorld::findTokens(){
    type1Token.clear();
    type2Token.clear();
    type3Token.clear();
    type4Token.clear();
    for(Contour c : mContours){
        if(c.mIsLeaf && c.mPolyLine.getPoints().size()==4 && c.mArea < MAX_AREA_THRESHOLD){
            type1Token.push_back(c);
        }
        if(c.mIsLeaf && c.mPolyLine.getPoints().size()==3 && c.mArea < MAX_AREA_THRESHOLD){
            type2Token.push_back(c);
        }
        if(c.mIsLeaf && c.mPolyLine.getPoints().size()==5 && c.mArea < MAX_AREA_THRESHOLD){
            type3Token.push_back(c);
        }
        if(c.mIsHole && c.mPolyLine.getPoints().size()==3 && c.mArea < MAX_AREA_THRESHOLD){
            type4Token.push_back(c);
        }
    }
}

bool RehabWorld::checkTokenValidity(){
    findTokens();
    int perPlayer = RehabGame::nbTokenTotal / RehabGame::nbPlayers;
    if(type1Token.size() != perPlayer) return false;
    if(type2Token.size() != perPlayer && RehabGame::nbPlayers >= 2) return false;
    if(type3Token.size() != perPlayer && RehabGame::nbPlayers >= 3) return false;
    if(type4Token.size() != perPlayer && RehabGame::nbPlayers >= 4) return false;
    return true;
}

void RehabWorld::registerHarvesterTokens(list<Contour> tokens){
    distributeTokens(type1Token, 0);
    distributeTokens(type2Token, 1);
    distributeTokens(type3Token, 2);
    distributeTokens(type4Token, 3);
}

void RehabWorld::distributeTokens(list<Contour> tokens, int player){
    if(RehabGame::nbPlayers > player){
        for(Contour c : tokens){
            vec2 gLoc = coordsToGrid(c.mCenter);
            board->addHarvesterToken(player, gLoc, c.mCenter);
        }
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
    if(!(board->board[(int)loc.x][(int)loc.y].getAccess() && accessCount == 3)){
        if(board->board[(int)loc.x][(int)loc.y].changeAccess())
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
        getXml(xml,"boardWidth", RehabGame::);
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
            case KeyEvent::KEY_ESCAPE:
                state = end;
                break;
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









