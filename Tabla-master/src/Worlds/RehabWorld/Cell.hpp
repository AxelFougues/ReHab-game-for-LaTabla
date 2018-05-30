//
//  SpaceUnit.hpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#ifndef SpaceUnit_hpp
#define SpaceUnit_hpp

#include <stdio.h>
#include "Plant.hpp"
#include "Animal.hpp"
#include "Token.hpp"

class Cell{
    
public:
    
    list<Token*> getHarvesterTokens(){return harvesterTokens;}
    /*!
     * @discussion Sets the cell's grid position.
     * @param x X coordinate.
     * @param y Y coordinate.
     */
    void setPos(int x, int y){posX = x; posY = y;}
    /*!
     * @discussion Gets cell's biomass.
     * @return biomass of cell's plant.
     */
    int getBiomass(){return plant.getBiomass();}
    /*!
     * @discussion Gets cell's access.
     * @return true if prtected.
     */
    bool getAccess(){return access;}
    /*!
     * @discussion Gets cell's position.
     * @return 0 for center, 1 for edge, 2 for corner.
     */
    int getSpaceUnitPosition();
    /*!
     * @discussion Checks if animal has settled here.
     * @return True is is settled.
     */
    bool isSettled(){return animal.settled;}
    /*!
     * @discussion Gets total amount of birds here.
     * @return Parent + kids in this cell.
     */
    int getPopulation(){if(!animal.settled) return 0; return animal.kids+1;}
    /*!
     * @discussion Adds harvester token to cell.
     * @param Token The token to be added.
     */
    void addHarvesterToken(Token *t){harvesterTokens.push_front(t);}
    /*!
     * @discussion Clears all harvester tokens from cell.
     */
    void clearHarvesterTokens(){harvesterTokens.clear();}
    /*!
     * @discussion flip flop on the cells access.
     * @return new access value.
     */
    bool changeAccess(){access = !access; return access;}
    /*!
     * @discussion Unsettles animal from cell.
     */
    void unSettle(){animal.unSettle();}
    /*!
     * @discussion Calls animal's setteling function.
     */
    void nidificate();
    /*!
     * @discussion Calls animal's reproduction function.
     * @param pos cell position.
     * @param neighborsHarvested Amount of neighbors containing harvester tokens.
     */
    void reproduce(int pos, int neighborsHarvested);
    /*!
     * @discussion Calls plant's harvesting functon.
     */
    void harvest();
    /*!
     * @discussion Calls plant's renewing functon.
     */
    void renew();

    
private:
    
    bool access = true;
    int posX, posY;
    Plant plant;
    Animal animal;
    list<Token*> harvesterTokens;
    
};

#endif /* SpaceUnit_hpp */
