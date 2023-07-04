#ifndef NODEPP_CONSOLE
#define NODEPP_CONSOLE

namespace console {

    void clear(){ Serial.write("\033c"); }

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

    template< class... T >
    int log( T... args ){ return print( args... ); }

    template< class... T >
    void start( T... args ){ Serial.begin(args...); }

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
    
    protected: String message; public:
   ~debug_t(){ console::log( "%s closed", message.c_str() ); }

    debug_t( String msg ){ console::log( "%s open", msg.c_str() ); message = msg; }
    void log( String msg ){ console::log( "--  %s", msg.c_str() ); }

    debug_t( const debug_t& o) = delete;
    debug_t& operator=( const debug_t& o ) = delete;

    debug_t( const debug_t&& o) = delete;
    debug_t& operator=( const debug_t&& o ) = delete;

};

#endif