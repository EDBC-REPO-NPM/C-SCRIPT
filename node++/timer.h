#ifndef NODEPP_TIMER
#define NODEPP_TIMER

namespace timer {

    template< class V, class... T >
    int* timeout( V func, uint time, T... args ){ int* out = new int(1); 
        process::loop::add([=](){ 
            static uint stamp = process::now() + time;
            $Start
            
            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out == 0 )       $Goto(2);
               if( process::now() >= stamp ) break;   $Yield(1); 
            }      func(args...); $Goto(2);

            $Yield(4); stamp = process::now() + time; $Goto(0);
            $Yield(2); delete out; $Yield(3); $Set(4); 
            $End
        }); return out;
    };

    void clear_timeout( int* address ){ if( address != nullptr ) *address = 0; }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    int* interval( V func, uint time, T... args ){ int* out = new int(1); 
        process::loop::add([=]( int* out ){ 
            static uint stamp = process::now() + time;
            $Start

            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out == 0 )       $Goto(2);
                if( process::now() >= stamp ) break; $Yield(1);
            }       stamp = process::now() + time; func(args...); $Goto(1); 

            $Yield(2); delete out; $Yield(3); $Set(0);
            $End
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
            $Start
            
            $Yield(1); func(
                [&]( T data ){ if( $Get > 2 ){ return; } resolved = data; $Set(3); },
                [&]( V data ){ if( $Get > 2 ){ return; } rejected = data; $Set(4); }
            );  if( $Get < 2 ){ $Goto(2); }

            while( true ){
                     if( out == nullptr )     $Goto(6);
                else if(*out == 0 )           $Goto(5);
                if( $Get != 2 ) break;        $Yield(2);
            }                                 $Goto($Get);
            $Yield(3); res( resolved );       $Goto(5);
            $Yield(4); rej( rejected );       $Goto(5);
            $Yield(5); delete out; $Yield(6); $Set(0);
            $End
        }, out ); return out;

    }

    template< class T >
    int* promise( 
        function_t<void,function_t<void,T>> func, function_t<void,T> res
    ){  int* out = new int(1);
        process::loop::add([=]( int* out ){
            static T   resolved;
            $Start
            
            $Yield(1); func(
                [&]( T data ){ if( $Get > 2 ){ return; } resolved = data; $Set(3); }
            );  if( $Get < 2 ){ $Goto(2); }

            while( true ){
                     if( out == nullptr )     $Goto(6);
                else if(*out == 0 )           $Goto(5);
                if( $Get != 2 ) break;        $Yield(2);
            }                                 $Goto($Get);
            $Yield(3); res( resolved );       $Goto(5);
            $Yield(5); delete out; $Yield(6); $Set(0);
            $End
        }, out); return out;

    }

    void clear_promise( int* address ){ if( address != nullptr ) *address = 0; }

}

#endif