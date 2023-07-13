#ifndef NODEPP_PROCESS
#define NODEPP_PROCESS

#include "string.h"
#include "array.h"

#ifdef __WIN32
#include <windows>
#endif

#ifndef ARDUINO

#include "signal.h"
#include <chrono>
#include <thread>

namespace ENV { 
#ifdef __WIN32

    int SET( string_t name, string_t value ){ return SetEnvironmentVariable( name.c_str(), value.c_str() ); }

    int DEL( string_t name ){ return SetEnvironmentVariable( neme.c_str(), NULL ); }

    string_t GET( string_t name ){ char buffer[MAX_PATH] = {0};
        DWORD result = GetEnvironmentVariable( name.c_str(), buffer, MAX_PATH );
        return buffer; 
    }

    int CLEAR(){ _environ = nullptr; }

#else

    int SET( string_t name, string_t value ){ return setenv( name.c_str(), value.c_str(), 1 ); }

    string_t GET( string_t name ){ return getenv( name.c_str() ); } 

    int DEL( string_t name ){ return unsetenv( name.c_str() ); }

    int CLEAR(){ return clearenv(); }

#endif
}

namespace ENV_NODEPP {

    int set( string_t name, string_t value ){ return ENV::SET( name, value ); }

    string_t get( string_t name ){ return ENV::GET( name ); }

    int remove( string_t name ){ return ENV::DEL( name );  } 

    int clear(){ return ENV::CLEAR(); }

    int init( string_t path ){ try {
            
        FILE* v = fopen( path.c_str(), "r" ); string_t s;
        array_t<string_t> env(2); bool nr = 0; bool pr = 0;
        function_t<void> lb([&](){ ENV::SET( env[0], env[1] ); });

        while( !feof(v) ){ int c = fgetc( v );

            if( c == 39 || c == 34 ){ pr=!pr; continue; }

            if( c == 32 && !pr ){ continue; }
            if( c == 35 ){ nr = true; continue; }
            if( c == 61 ){ env[0]=s; s.clear(); continue; } 

            else if( c == -1 ){ env[1]=s; s.clear(); lb(); nr=0; pr=0; break; }
            else if((c == 10 || c == 59 ) && !pr ){ env[1]=s; s.clear(); lb(); nr=0; continue; }

            if( !nr ) s.push(c);
        }   fclose(v); return  1;
        } catch(...) { return -1; }
    };

}

namespace process {

    namespace env = ENV_NODEPP;
    array_t<string_t> args;
    int threads = 0;

    FILE* stdin  = ::stdin;
    FILE* stderr = ::stderr;
    FILE* stdout = ::stdout;

    uint now(){ auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return (uint) ms.count(); 
    }

    void delay( uint time ){ std::this_thread::sleep_for( std::chrono::milliseconds(time) ); }
    uint pid(){ return std::hash<std::thread::id>{}( std::this_thread::get_id() ); }
    uint cpus(){ return std::thread::hardware_concurrency(); }
    void yield(){ return std::this_thread::yield(); }

    string_t shell(){ return ENV_NODEPP::get("SHELL"); }
    string_t home(){ return ENV_NODEPP::get("HOME"); }
    string_t user(){ return ENV_NODEPP::get("USER"); }
    string_t cwd(){ return ENV_NODEPP::get("PWD"); } 

    void kill( int i=0 ){ exit(i); }

}

#else 

namespace process {
    void delay( uint time ){ ::delay( time ); }
    uint now(){ return ::millis(); }
    array_t<string_t> args;
    int threads = 0;
}

#endif
#endif