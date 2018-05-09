#include "../BlueprintWorld.h"

#ifdef isUsingTime
float BlueprintWorld::getTimeSinceStart(){
    return ci::app::getElapsedSeconds() - worldEnterTime;
}
#endif
