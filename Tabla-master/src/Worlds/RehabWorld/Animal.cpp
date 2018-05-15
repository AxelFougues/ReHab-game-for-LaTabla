//
//  Animal.cpp
//  Tabla
//
//  Created by afougu01 on 09/05/2018.
//

#include "Animal.hpp"

void Animal::reproduce(int n, int pos){
    if(settled){
        kids = 0;
        switch (pos) {
            case 0:
                if(n <= 1)
                    kids = 2;
                if(n <= 4)
                    kids = 1;
                break;
            case 1:
                if(n == 0)
                    kids = 2;
                if(n <= 2)
                    kids = 1;
                break;
            case 2:
                if(n == 0)
                    kids = 2;
                if(n == 1)
                    kids = 1;
                break;
        }
    }
}

void Animal::settle(int b){
    if(!settled && b >= habitatSuitabilityThreshold){
        settled = true;
    }
}

void Animal::unSettle(){
    settled = false;
    
}


