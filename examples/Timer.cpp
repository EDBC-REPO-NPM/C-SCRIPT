#include "node++/node++.h"
#include "node++/timer.h"

void $Ready() { ptr_t<int> i = new int(1);

    auto t = timer::interval([=](){
        console::log("interval every: 1 second - ",*i," seconds"); (*i)++;
    },1000);

    timer::timeout([=](){
        console::log("timeout after: 10 seconds ", *t);
        timer::clear(t);
    },10000);

}