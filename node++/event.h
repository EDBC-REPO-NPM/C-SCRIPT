#ifndef NODEPP_EVENT
#define NODEPP_EVENT

#include <mutex>
 
namespace EVENT_NODEPP {
    uint HASH(  ) {
        int x = 32; uint data = 0; while( x --> 0 ){
            data = ( data | rand() % 2 ) << 1;
        }   return data;
    }
}

template< class... A > 
class event_t { 

    using ev_queue = umap_t<uint,function_t<void,A...>>;

    protected:
    ptr_t<ev_queue>   every_queue;
    ptr_t<ev_queue>   once_queue;
    ptr_t<std::mutex> mut;

    public:
    void emit( A... args ){
        std::lock_guard<std::mutex> lock(*mut);
        for( auto x:*every_queue ) x.second(args...);
        for( auto x:*once_queue )  x.second(args...);
        if( !once_queue->empty() ) once_queue->clear();
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    uint on( function_t<void,A...> func ){
        std::lock_guard<std::mutex> lock(*mut);
        uint _hash = EVENT_NODEPP::HASH();
        (*every_queue)[_hash] = func; return _hash;
    }

    uint once( function_t<void,A...> func ){
        std::lock_guard<std::mutex> lock(*mut);
        uint _hash = EVENT_NODEPP::HASH();
        (*once_queue)[_hash] = func; 
        return _hash;
    }

    void off( uint _hash ){
        std::lock_guard<std::mutex> lock(*mut);
        every_queue->erase( _hash );
        once_queue->erase( _hash );
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    event_t() : every_queue(new ev_queue), once_queue(new ev_queue), mut(new std::mutex) {}
   ~event_t(){  }

};

#endif