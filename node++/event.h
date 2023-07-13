#ifndef NODEPP_EVENT
#define NODEPP_EVENT
#include "queue.h"
 
namespace EVENT_NODEPP {

    uint HASH() {
        int x = 32; uint data = 0; while( x --> 0 ){
            data = ( data | rand() % 2 ) << 1;
        }   return data;
    }

    template< class... T >
    struct ID {
        uint             id;
        function_t<T...> cb;
    };

}

template< class... A > 
class event_t { 

    protected:
    using ev = queue_t<EVENT_NODEPP::ID<void,A...>>;
          ev   every_queue, once_queue;

    public:
    void emit( A... args ) noexcept {
        every_queue.map([&]( EVENT_NODEPP::ID<void,A...> arg ){ arg.cb(args...); });
        once_queue.map([&]( EVENT_NODEPP::ID<void,A...> arg ){ arg.cb(args...); });
        if( !once_queue.empty() ) once_queue.clear();
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    void off( uint _hash ) noexcept {
        uint index_A = every_queue.index_of([=]( EVENT_NODEPP::ID<void,A...> data ){ return data.id == _hash; });
        uint index_B = once_queue.index_of([=]( EVENT_NODEPP::ID<void,A...> data ){ return data.id == _hash; });
        every_queue.erase( every_queue.get( index_A ) ); once_queue.erase( once_queue.get( index_B ) );
    }

    uint on( function_t<void,A...> func ) noexcept {
        uint _hash = EVENT_NODEPP::HASH();
        every_queue.push({ _hash, func });
        return _hash;
    }

    uint once( function_t<void,A...> func ) noexcept {
        uint _hash = EVENT_NODEPP::HASH();
        once_queue.push({ _hash, func }); 
        return _hash;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    event_t(){  }
   ~event_t(){  }

};

#endif