#ifndef NODEPP_EVENT
#define NODEPP_EVENT
 
namespace EVENT_NODEPP {

    uint HASH(  ) {
        int x = 32; uint data = 0; while( x --> 0 ){
            data = ( data | rand() % 2 ) << 1;
        }   return data;
    }

    template< class... T >
    struct ID {
        function_t<void,T...> cb;
        uint id;
    };

}

template< class... A > 
class event_t { 

    using ev = queue_t<EVENT_NODEPP::ID<A...>>;

    protected:
    ev        once_queue;
    ev        every_queue;

    public:
    void emit( A... args ){
        every_queue.map([]( EVENT_NODEPP::ID<A...> arg ){ arg.cb(); });
        once_queue.map([]( EVENT_NODEPP::ID<A...> arg ){ arg.cb(); });
        if( !once_queue.empty() ) once_queue.clear();
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    void off( uint _hash ){
        uint index_A = every_queue.index_of([=]( EVENT_NODEPP::ID<A...> data ){ return data.id == _hash; });
        uint index_B = once_queue.index_of([=]( EVENT_NODEPP::ID<A...> data ){ return data.id == _hash; });
        every_queue.erase( every_queue.get( index_A ) );
        once_queue.erase( once_queue.get( index_B ) );
    }

    uint on( function_t<void,A...> func ){
        uint _hash = EVENT_NODEPP::HASH();
        every_queue.push({ func, _hash });
        return _hash;
    }

    uint once( function_t<void,A...> func ){
        uint _hash = EVENT_NODEPP::HASH();
        once_queue.push({ func, _hash }); 
        return _hash;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    event_t(){  }
   ~event_t(){  }

};

#endif