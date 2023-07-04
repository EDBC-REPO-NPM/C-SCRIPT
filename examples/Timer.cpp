#include "node++/node++.h"
#include "node++/timer.h"

int i = 1;

int $Ready() { 

    int* t = timer::interval([&](){
        console::log("interval every: 1 second - ${0} seconds",i); i++;
    },1000);

    timer::timeout([=](){
        console::log("timeout after: 10 seconds ${0}", *t);
        timer::clear_interval(t);
    },10000);

    return 0; 
}