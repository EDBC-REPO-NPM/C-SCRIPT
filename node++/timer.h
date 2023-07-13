#ifndef NODEPP_TIMER
#define NODEPP_TIMER

namespace timer {

    template< class V, class... T >
    ptr_t<int> timeout( V func, uint time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time;
            $Start
            
            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
               if( process::now() >= stamp ) break;   $Yield(1); 
            }      func(args...);         $Goto(2);

            $Yield(4); stamp = process::now() + time; $Goto(0);
            $Yield(2); out.reset();       $Yield(3);
            $End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> timeout( V func, uint* time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time;
            $Start
            
            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
               if( process::now() >= stamp ) break;   $Yield(1); 
            }      func(args...);         $Goto(2);

            $Yield(4); stamp = process::now() + *time; $Goto(0);
            $Yield(2); out.reset();       $Yield(3);
            $End
        }, out ); return out;
    };

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    ptr_t<int> counter( V func, uint time, uint count, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time;
            static uint iters = count;
            $Start

            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
                if( process::now() >= stamp ) break; $Yield(1);
            }   if( iters --<= 0 ) $Goto(2);
            
            stamp = process::now() + time; func(args...); $Goto(1); 
            $Yield(2); out.reset();       $Yield(3);
            $End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> counter( V func, uint* time, uint count, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time;
            static uint iters = count;
            $Start

            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
                if( process::now() >= stamp ) break; $Yield(1);
            }   if( iters --<= 0 ) $Goto(2);
            
            stamp = process::now() + *time; func(args...); $Goto(1); 
            $Yield(2); out.reset(); $Yield(3);
            $End
        }, out ); return out;
    };

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    ptr_t<int> interval( V func, uint time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time;
            $Start

            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
                if( process::now() >= stamp ) break; $Yield(1);
            }   
            
            stamp = process::now() + time; func(args...);   $Goto(1); 
            $Yield(2); out.reset();                         $Yield(3);

            $End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> interval( V func, uint* time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time;
            $Start

            while( true ){
                     if( out == nullptr ) $Goto(3);
                else if(*out <= 0 )       $Goto(2);
                if( process::now() >= stamp ) break; $Yield(1);
            }   stamp = process::now() + *time; func(args...);   $Goto(1); 
            $Yield(2); out.reset();       $Yield(3);

            $End
        }, out ); return out;
    };

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class T, class V >
    ptr_t<int> promise( 
        function_t<void,function_t<void,T>,function_t<void,V>> func,
        function_t<void,T> res, function_t<void,V> rej
    ){  ptr_t<int> out = new int(1);
        process::loop::add([=]( ptr_t<int> out ){
            static T   resolved;
            static V   rejected;
            $Start
            
            $Yield(1); func(
                [&]( T data ){ if( $Get > 2 ){ return; } resolved = data; $Set(3); },
                [&]( V data ){ if( $Get > 2 ){ return; } rejected = data; $Set(4); }
            );  if( $Get < 2 ){ $Goto(2); }

            while( true ){
                     if( out == nullptr )     $Goto(6);
                else if(*out <= 0 )           $Goto(5);
                if( $Get != 2 ) break;        $Yield(2);
            }                                 $Goto($Get);

            $Yield(3); res( resolved );       $Goto(5);
            $Yield(4); rej( rejected );       $Goto(5);
            $Yield(5); out.reset();           $Yield(6);
            $End
        }, out ); return out;

    }

    template< class T >
    ptr_t<int> promise( 
        function_t<void,function_t<void,T>> func, function_t<void,T> res
    ){  ptr_t<int> out = new int(1);
        process::loop::add([=]( ptr_t<int> out ){
            static T   resolved;
            $Start
            
            $Yield(1); func(
                [&]( T data ){ if( $Get > 2 ){ return; } resolved = data; $Set(3); }
            );  if( $Get < 2 ){ $Goto(2); }

            while( true ){
                     if( out == nullptr )     $Goto(6);
                else if(*out <= 0 )           $Goto(5);
                if( $Get != 2 ) break;        $Yield(2);
            }                                 $Goto($Get);

            $Yield(3); res( resolved );       $Goto(5);
            $Yield(5); out.reset();           $Yield(6);
            $End
        }, out); return out;

    }
    /*────────────────────────────────────────────────────────────────────────────*/

    void clear( ptr_t<int> address ){ if( !address ) *address = 0; }

}

#endif