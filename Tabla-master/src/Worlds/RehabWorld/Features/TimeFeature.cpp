#include "../RehabWorld.h"

#ifdef isUsingTime
float RehabWorld::getTimeSinceStart(){
    return ci::app::getElapsedSeconds() - worldEnterTime;
}
#endif
