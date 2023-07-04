#ifndef NODEPP_PROCESS
#define NODEPP_PROCESS

#include "algorithm.h"

namespace process {

    void delay( uint time ){ ::delay(time); }

    uint now(){ return millis(); }

}

#endif