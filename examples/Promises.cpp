#include "node++/node++.h"
#include "node++/timer.h"

void $Ready() { ptr_t<int> i = new int(1);

    auto t = timer::interval([=](){
        console::log("time interval - ",*i," seconds"); (*i)++;
    },1000);
    
    timer::promise<int,int>([=]( auto res, auto rej ){

        timer::timeout([=](){
            if( rand() % 2 == 0 )
                 res(100);
            else rej(100);
        },10000);

    },
    [=]( int res ){
        console::log("resolved: ",res); timer::clear(t);
    },
    [=]( int rej ){
        console::log("rejected: ",rej); timer::clear(t);
    });

}