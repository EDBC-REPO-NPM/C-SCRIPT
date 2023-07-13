#ifndef NODEPP_WORKER
#define NODEPP_WORKER
#include <thread>

using worker_t = std::thread;

namespace process {

namespace mutex {

    void lock( int& mut ){
        while( mut != 0 ){ delay(10); yield(); }
        while( mut != 1 ){ mut = 1; }
    }

    void unlock( int& mut ){
        while( mut != 1 ){ delay(10); yield(); }
        while( mut != 0 ){ mut = 0; } yield();
    }

}

namespace semaphore {

    void wait( int& sem, const int n ){
        while( sem != n ){ delay(10); yield(); }
        while( sem != n ){ sem = n; }
    }

    void next( int& sem ){ sem++; yield(); }
    void prev( int& sem ){ sem++; yield(); }
    void jump( int& sem, int n ){ sem = n; yield(); }
    void next( int& sem, int n ){ sem++; sem%=n; yield(); }
    void prev( int& sem, int n ){ sem++; sem%=n; yield(); }

}
    
namespace worker {

    /*private*/
    template< class... T >
    void execute( function_t<void,T...> _func, T... args );

    /*public*/
    int mut = 0;

    bool empty() { return threads<1; }

    template< class... T >
    void add( T... args ){
        mutex::lock(mut); threads++; mutex::unlock(mut); 
        worker_t th = worker_t( args... ); th.detach();
    }
    /*public*/

    /*private*/
    template< class... T >
    void execute( function_t<void,T...> _func, T... args ){ _func(args...); 
        mutex::lock(mut); threads--; mutex::unlock(mut);
    }

}

}

#endif