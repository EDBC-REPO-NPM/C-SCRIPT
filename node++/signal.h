#ifndef NODEPP_SIGNAL
#define NODEPP_SIGNAL

#include "event.h"
#include <csignal>

namespace process {
    
    event_t<int> onFPE;  //on Floating Point Exception
    event_t<int> onSEGV; //on Segmentation Violation
    event_t<int> onILL;  //on Illegal Instruction
    event_t<int> onSINT;  //on Signal Interrupt
    event_t<int> onTERM; //on Terminate
    event_t<int> onABRT; //on Abort
    event_t<int> onERR;  //on Error

    void signal() {
        ::signal( SIGFPE,  []( int param ){ onFPE.emit(param);  onERR.emit(param); exit(param); });
        ::signal( SIGILL,  []( int param ){ onILL.emit(param);  onERR.emit(param); exit(param); });
        ::signal( SIGINT,  []( int param ){ onSINT.emit(param); onERR.emit(param); exit(param); });
        ::signal( SIGTERM, []( int param ){ onTERM.emit(param); onERR.emit(param); exit(param); });
        ::signal( SIGABRT, []( int param ){ onABRT.emit(param); onERR.emit(param); exit(param); });
        ::signal( SIGSEGV, []( int param ){ onSEGV.emit(param); onERR.emit(param); exit(param); });
    }

}

#endif