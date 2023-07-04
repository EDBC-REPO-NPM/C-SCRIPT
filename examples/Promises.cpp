#include "node++/node++.h"
#include "node++/timer.h"

int i = 1;

int $Ready() { 

    int* t = timer::interval([&](){
        console::log("han pasado ${0} segundos",i); i++;
    },1000);
    
    timer::promise<int,int>([=]( auto res, auto rej ){

        timer::timeout([=](){
            if( rand() % 2 == 0 )
                 res(100);
            else rej(100);
        },10000);

    },
    [=]( int res ){
        console::log("resolved: ${0}",res);
        timer::clear_interval(t);
    },
    [=]( int rej ){
        console::log("rejected: ${0}",rej);
        timer::clear_interval(t);
    });

    return 0; 
}