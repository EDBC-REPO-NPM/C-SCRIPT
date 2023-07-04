#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

#include "algorithm.h"
#include "process.h"
#include "event.h"
#include "loop.h"

namespace process {
    void next(){
        if( process::loop::is_active() ) process::loop::next();
        if( process::poll::is_active() ) process::poll::next();
    }
}

#endif