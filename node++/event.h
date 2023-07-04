#ifndef NODEPP_EVENT
#define NODEPP_EVENT

template< class... A > 
class event_t { 

    protected:
    using _ev = function_t<void,A...>;
    ptr_t<_ev> every_ev, once_ev;

    public:
    void emit( A... args ){
        if( every_ev != nullptr )(*every_ev)(args...); 
        if( once_ev != nullptr )(*once_ev)(args...);
            once_ev.reset();
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    void once( _ev func ){ once_ev = new _ev(func); }
    void on( _ev func ){ every_ev = new _ev(func); }
    void off(){ every_ev.reset(); once_ev.reset(); }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    event_t( _ev func ){ once_ev = new _ev(func); }
    event_t(){  }
   ~event_t(){  }

};

#endif