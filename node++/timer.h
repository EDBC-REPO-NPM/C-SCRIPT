#ifndef NODEPP_TIMER
#define NODEPP_TIMER

namespace timer {

    template< class V, class... T >
    int* timeout( V func, uint time, T... args ){ int* out = new int(1); 
        process::loop::add([=](){ 
            static uint stamp = process::now() + time;
            _Start
            
            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out == 0 )       _Goto(2);
               if( process::now() >= stamp ) break;   _Yield(1); 
            }      func(args...); _Goto(2);

            _Yield(4); stamp = process::now() + time; _Goto(0);
            _Yield(2); delete out; _Yield(3); _Set(4); 
            _End
        }); return out;
    };

    void clear_timeout( int* address ){ if( address != nullptr ) *address = 0; }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    int* interval( V func, uint time, T... args ){ int* out = new int(1); 
        process::loop::add([=]( int* out ){ 
            static uint stamp = process::now() + time;
            _Start

            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out == 0 )       _Goto(2);
                if( process::now() >= stamp ) break; _Yield(1);
            }       stamp = process::now() + time; func(args...); _Goto(1); 

            _Yield(2); delete out; _Yield(3); _Set(0);
            _End
        }, out ); return out;
    };

    void clear_interval( int* address ){ if( address != nullptr ) *address = 0; }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class T, class V >
    int* promise( 
        function_t<void,function_t<void,T>,function_t<void,V>> func,
        function_t<void,T> res, function_t<void,V> rej
    ){  int* out = new int(1);
        process::loop::add([=]( int* out ){
            static T   resolved;
            static V   rejected;
            _Start
            
            _Yield(1); func(
                [&]( T data ){ if( _Get > 2 ){ return; } resolved = data; _Set(3); },
                [&]( V data ){ if( _Get > 2 ){ return; } rejected = data; _Set(4); }
            );  if( _Get < 2 ){ _Goto(2); }

            while( true ){
                     if( out == nullptr )     _Goto(6);
                else if(*out == 0 )           _Goto(5);
                if( _Get != 2 ) break;        _Yield(2);
            }                                 _Goto(_Get);
            _Yield(3); res( resolved );       _Goto(5);
            _Yield(4); rej( rejected );       _Goto(5);
            _Yield(5); delete out; _Yield(6); _Set(0);
            _End
        }, out ); return out;

    }

    template< class T >
    int* promise( 
        function_t<void,function_t<void,T>> func, function_t<void,T> res
    ){  int* out = new int(1);
        process::loop::add([=]( int* out ){
            static T   resolved;
            _Start
            
            _Yield(1); func(
                [&]( T data ){ if( _Get > 2 ){ return; } resolved = data; _Set(3); }
            );  if( _Get < 2 ){ _Goto(2); }

            while( true ){
                     if( out == nullptr )     _Goto(6);
                else if(*out == 0 )           _Goto(5);
                if( _Get != 2 ) break;        _Yield(2);
            }                                 _Goto(_Get);
            _Yield(3); res( resolved );       _Goto(5);
            _Yield(5); delete out; _Yield(6); _Set(0);
            _End
        }, out); return out;

    }

    void clear_promise( int* address ){ if( address != nullptr ) *address = 0; }

}

#endif