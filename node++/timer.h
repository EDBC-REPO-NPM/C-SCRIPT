#ifndef NODEPP_TIMER
#define NODEPP_TIMER

namespace timer {

    template< class V, class... T >
    ptr_t<int> timeout( V func, uint time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time; if( stamp > 65500 ) stamp += 36; 
            _Start
            
            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
               if( process::now() >= stamp ) break;   _Yield(1); 
            }      func(args...);         _Goto(2);

            _Yield(4); stamp = process::now() + time; _Goto(0);
            _Yield(2); out.reset();       _Yield(3);
            _End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> timeout( V func, uint* time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time; if( stamp > 65500 ) stamp += 36;
            _Start
            
            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
               if( process::now() >= stamp ) break;   _Yield(1); 
            }      func(args...);         _Goto(2);

            _Yield(4); stamp = process::now() + *time; _Goto(0);
            _Yield(2); out.reset();       _Yield(3);
            _End
        }, out ); return out;
    };

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    ptr_t<int> counter( V func, uint time, uint count, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time; if( stamp > 65500 ) stamp += 36;
            static uint iters = count;
            _Start

            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
                if( process::now() >= stamp ) break; _Yield(1);
            }   if( iters --<= 0 ) _Goto(2);
            
            stamp = process::now() + time; func(args...); _Goto(1); 
            _Yield(2); out.reset();       _Yield(3);
            _End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> counter( V func, uint* time, uint count, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time; if( stamp > 65500 ) stamp += 36;
            static uint iters = count;
            _Start

            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
                if( process::now() >= stamp ) break; _Yield(1);
            }   if( iters --<= 0 ) _Goto(2);
            
            stamp = process::now() + *time; func(args...); _Goto(1); 
            _Yield(2); out.reset(); _Yield(3);
            _End
        }, out ); return out;
    };

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class V, class... T >
    ptr_t<int> interval( V func, uint time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + time; if( stamp > 65500 ) stamp += 36;
            _Start

            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
                if( process::now() >= stamp ) break; _Yield(1);
            }   stamp = process::now() + time; func(args...);   _Goto(1); 
            _Yield(2); out.reset();       _Yield(3);

            _End
        }, out ); return out;
    };

    template< class V, class... T >
    ptr_t<int> interval( V func, uint* time, T... args ){ ptr_t<int> out = new int(1); 
        process::loop::add([=]( ptr_t<int> out ){ 
            static uint stamp = process::now() + *time; if( stamp > 65500 ) stamp += 36;
            _Start

            while( true ){
                     if( out == nullptr ) _Goto(3);
                else if(*out <= 0 )       _Goto(2);
                if( process::now() >= stamp ) break; _Yield(1);
            }   stamp = process::now() + *time; func(args...);   _Goto(1); 
            _Yield(2); out.reset();       _Yield(3);

            _End
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
            _Start
            
            _Yield(1); func(
                [&]( T data ){ if( _Get > 2 ){ return; } resolved = data; _Set(3); },
                [&]( V data ){ if( _Get > 2 ){ return; } rejected = data; _Set(4); }
            );  if( _Get < 2 ){ _Goto(2); }

            while( true ){
                     if( out == nullptr )     _Goto(6);
                else if(*out <= 0 )           _Goto(5);
                if( _Get != 2 ) break;        _Yield(2);
            }                                 _Goto(_Get);

            _Yield(3); res( resolved );       _Goto(5);
            _Yield(4); rej( rejected );       _Goto(5);
            _Yield(5); out.reset();           _Yield(6);
            _End
        }, out ); return out;

    }

    template< class T >
    ptr_t<int> promise( 
        function_t<void,function_t<void,T>> func, function_t<void,T> res
    ){  ptr_t<int> out = new int(1);
        process::loop::add([=]( ptr_t<int> out ){
            static T   resolved;
            _Start
            
            _Yield(1); func(
                [&]( T data ){ if( _Get > 2 ){ return; } resolved = data; _Set(3); }
            );  if( _Get < 2 ){ _Goto(2); }

            while( true ){
                     if( out == nullptr )     _Goto(6);
                else if(*out <= 0 )           _Goto(5);
                if( _Get != 2 ) break;        _Yield(2);
            }                                 _Goto(_Get);

            _Yield(3); res( resolved );       _Goto(5);
            _Yield(5); out.reset();           _Yield(6);
            _End
        }, out); return out;

    }
    /*────────────────────────────────────────────────────────────────────────────*/

    void clear( ptr_t<int> address ){ if( !address.empty() ) *address = 0; }

}

#endif