#ifndef EVENT_CSCRIPT
#define EVENT_CSCRIPT
 
using namespace std;

template< class... A > class Event { public: function<void( A... args )> emit;
    template< class T > Event( T func ){ emit = [&]( A... args ){ 
        try { func(args...); } catch( system_error &e ) { Error(e); }
    }; }
};

#endif 