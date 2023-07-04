#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

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

    int scan( const char* format, ... ) {
      char buffer[256]; int count = 0;

      va_list args;
      va_start(args, format);
      vsnprintf(buffer, sizeof(buffer), format, args);
      va_end(args);

      for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == '%') {
          i++;
          if (buffer[i] == 'd' || buffer[i] == 'i') {
            int* arg = va_arg(args, int*);
            while (!Serial.available()) {}
            *arg = Serial.parseInt();
            count++;
          } else if (buffer[i] == 'f') {
            float* arg = va_arg(args, float*);
            while (!Serial.available()) {}
            *arg = Serial.parseFloat();
            count++;
          }
        } else {
          while (!Serial.available()) {}
          char c = Serial.read();
          if (c != buffer[i]) break;
        }
      }
      
      return count;
    }

    int print( const char* format, ... ){
      char buffer[256];

      va_list args;
      va_start(args, format);
      vsnprintf(buffer, sizeof(buffer), format, args);
      va_end(args); int i=0;

      for( i=0; buffer[i]; i++ ) Serial.write( buffer[i] ); 
      return i;
    }

#endif

    void clear(){ print("\033c"); }

    template< class T >
    int log( T args ){ return print("%s\n", std::to_string( args ).c_str() ); }

    template< class V, class... T >
    int log( V argc, T... args ){ 
        if(!regex::test(std::to_string(argc),"\\$\\{\\d+\\}","i") ){
            iterate([&]( auto arg ){ print("%s ",std::to_string(arg).c_str()); }, argc, args... );
        } else {
            print("%s", format( argc, args... ).c_str() );
        }
    print("\n"); return 1; }

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

}

class debug_t { 
    
    protected: string_t message; public:
   ~debug_t(){ console::log( "${0} closed", message ); }

    debug_t( string_t msg ){ console::log( "${0} open", msg ); message = msg; }
    void log( string_t msg ){ console::log( "--  ${0}", msg ); }

    debug_t( const debug_t& o) = delete;
    debug_t& operator=( const debug_t& o ) = delete;

    debug_t( const debug_t&& o) = delete;
    debug_t& operator=( const debug_t&& o ) = delete;

};
#endif