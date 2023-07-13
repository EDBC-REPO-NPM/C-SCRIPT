#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

#include "algorithm.h"
#include "string.h"
#include "array.h"

namespace console {

#ifndef ARDUINO

    template< class... T >
    void start( T... args ){  }

    template< class... T >
    int scan( T... args ){ return scanf( args... ); }

    template< class V, class... T >
    int print( V argc, T... args ){ return printf( argc, args... ); }

#else

    template< class... T >
    void start( T... args ){ Serial.begin(args...); }

    template< class... T >
    int print( const char* format, T... args ){
      return Serial.write( (char*) string::format( format, args... ) );
    }

    template< class... T >
    string_t scan( const char* format, T... args ) { while( !Serial.available() ){}
      return string::scan( Serial.read(), format, args... );
    }

#endif

    void clear(){ print("\033c"); }

    template< class... T >
    int done( T... input ){ print("\033[0;32m\033[1mDONE: \033[0m\033[0m"); return log(input...); }

    template< class... T >
    int info( T... input ){ print("\033[0;36m\033[1mINFO: \033[0m\033[0m"); return log(input...); }

    template< class... T >
    int error( T... input ){ print("\033[0;31m\033[1mERROR: \033[0m\033[0m"); return log(input...); }

    template< class... T >
    int warning( T... input ){ print("\033[0;33m\033[1mWARNING: \033[0m\033[0m"); return log(input...); }

    template< class... T >
    int success( T... input ){ print("\033[0;32m\033[1mSUCCESS: \033[0m\033[0m"); return log(input...); }

    template< class... T >
    int log( T... args ){ string::map([=]( string_t arg ){ print("%s ",(char*)arg); }, args... ); return print("\n"); }

}

class debug_t { 
    
    protected: string_t message; public:
   ~debug_t(){ console::log( "%s closed", message.c_str() ); }

    debug_t( string_t msg ){ console::log( "%s open", msg.c_str() ); message = msg; }
    void log( string_t msg ){ console::log( "--  %s", msg.c_str() ); }

    debug_t( const debug_t& o) = delete;
    debug_t& operator=( const debug_t& o ) = delete;

    debug_t( const debug_t&& o) = delete;
    debug_t& operator=( const debug_t&& o ) = delete;

};

#endif