#ifndef NODEPP_NODEPP
#define NODEPP_NODEPP

#include "algorithm.h"
#include "process.h"
#include "loop.h"

namespace process {

    void start( int argc, char** args ){
        int i=1; while( i ++< argc ) 
        process::args.push(args[i]);
        #ifndef ARDUINO
            process::signal();
        #endif
    }

    void next(){
        static bool b = 0; if( !b )
             process::poll::next();
        else process::loop::next(); b=!b; 
        #ifndef ARDUINO
            process::delay(3);
        #endif
    }

    bool empty(){
        return ( process::poll::empty() && process::loop::empty() && process::threads<1 );
    }

}

#endif