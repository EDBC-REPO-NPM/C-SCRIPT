#ifndef PROMISE_CSCRIPT
#define PROMISE_CSCRIPT

#include <thread>
#include <future>
#include <chrono>

using namespace std;

namespace std {

    template< class V, class... T >
    auto resolve( V func, T... args ){
        return async( launch::deferred, [=](){
            return func( args... );
        }, args... );
    }

    template< class T > auto resolve( T func ){
        return async( launch::deferred, func );
    }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class... T >
    int* setTimeout( V func, int time=1, T... args ){ 
        int* out = new int; *out = 1; thread th([=]() { 
            this_thread::sleep_for( chrono::milliseconds(time) );
            if( !*out ) return; func( args... ); 
        }); th.detach(); return out;
    };

    template< class T >
    int* setTimeout( T func, int time=1 ){ 
        int* out = new int; *out = 1; thread th([=]() { 
            this_thread::sleep_for( chrono::milliseconds(time) );
            if( !*out ) return; func(); 
        }); th.detach(); return out;
    };

    void clearTimeout( int* address ){ *address = 0; }

    //───────────────────────────────────────────────────────────────────────//

    template< class V, class... T >
    int* setInterval( V func, int time=1, T... args ){ 
        int* out = new int; *out = 1; thread th([=]() { while( *out ){ 
            this_thread::sleep_for( chrono::milliseconds(time) ); 
            if( !*out ) return; func( args... ); 
        }});th.detach(); return out;
    };

    template< class V >
    int* setInterval( V func, int time=1 ){ 
        int* out = new int; *out = 1; thread th([=]() { while( *out ){ 
            this_thread::sleep_for( chrono::milliseconds(time) ); 
            if( !*out ) return; func(); 
        }});th.detach(); return out;
    };

    void clearInterval( int* address ){ *address = 0; }

}

//────────────────────────────────────────────────────────────────────────────────────────────────//

/*
template< class T  >
struct Promise_Resolver {
    void then( T data )
    void reject( Err err )
};

struct Promise {
    Promise( void* resove, void* reject ) {
    }
};
*/

#endif 