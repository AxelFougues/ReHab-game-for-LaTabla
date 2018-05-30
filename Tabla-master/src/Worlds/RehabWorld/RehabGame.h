//
//  RehabGame.h
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef RehabGame_h
#define RehabGame_h

#include "Cell.hpp"
#include "Harvester.hpp"
#include "Token.hpp"

using namespace std;
using namespace cinder;

class RehabGame{
    
public:
    
    RehabGame(){
        for(int i = 0; i < nbPlayers; i++)
            harvesters[i] = new Harvester();
    }
    
    static const int boardWidth = 5;
    static const int boardHeight = 4;
    static const int nbPlayers = 2;
    static const int nbTokenTotal = 2;
    Cell board[RehabGame::boardWidth][RehabGame::boardHeight];
    Harvester *harvesters[nbPlayers];
    int births = 0;
    list<int> animalPopulation;
    
    /*!
     * @discussion Initialises the game grid with brand new cells.
     */
    void fillBoard();
    /*!
     * @discussion Nidificates each cell.
     */
    void nidificate();
    /*!
     * @discussion Reproduction in each cell.
     */
    void reproduce();
    /*!
     * @discussion Harvests each cell.
     */
    void harvest();
    /*!
     * @discussion Renews each cell.
     */
    void renew();
    /*!
     * @discussion Deals points to harvesters.
     */
    void attributePoints();
    /*!
     * @discussion Gets amount of harvested neighbors of a cell.
     * @param pos cell position.
     * @return Amount fo neighbors.
     */
    int neighborsHarvested(vec2 pos);
    /*!
     * @discussion Creates and links token to cell and owner.
     * @param i owner index.
     * @param pos cell position.
     * @param loc token world location.
     */
    void addHarvesterToken(int i, vec2 pos, vec2 loc);
    
};

#endif /* RehabGame_h */
