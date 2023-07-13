#ifndef NODEPP_PROCESS
#define NODEPP_PROCESS

#include "string.h"
#include "array.h"

namespace process {
    void delay( uint time ){ ::delay( time ); }
    uint now(){ return ::millis(); }
    array_t<string_t> args;
    int threads = 0;
}

#endif