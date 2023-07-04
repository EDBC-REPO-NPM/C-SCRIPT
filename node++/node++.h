#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

#include "algorithm.h"
#include "process.h"
#include "console.h"
#include "worker.h"
#include "event.h"
#include "loop.h"

class nodepp { public:

   ~nodepp(){

        process::worker::add([=](){
        while( process::poll::is_active() || process::loop::is_active() ){ 
               process::poll::next(); process::delay(3);
        }
        });
        
        while( process::poll::is_active() || process::loop::is_active() || process::threads > 1 ){ 
               process::loop::next(); process::delay(3);
        }
        
    }
   
    nodepp( int argc, char** args ){ 
        int i=1; while( i ++< argc ) process::args.push(args[i]);
    }

    nodepp(){  }

};

#endif