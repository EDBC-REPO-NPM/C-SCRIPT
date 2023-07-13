#include "node++/node++.h"
#include "node++/timer.h"

void $Ready() { ptr_t<int> i = new int(1);

    timer::counter([=](){
        console::log("interval every: 1 second - ",*i," seconds"); (*i)++;
    }, 1000, 10 );

}