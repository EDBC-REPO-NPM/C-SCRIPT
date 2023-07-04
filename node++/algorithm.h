#ifndef NODEPP_ALGORITHM
#define NODEPP_ALGORITHM

using uint = unsigned int;

/*────────────────────────────────────────────────────────────────────────────*/

#include "coroutine.h"
#include "function.h"
#include "console.h"
#include "ptr.h"

/*────────────────────────────────────────────────────────────────────────────*/

template< class U, class T >
void iterate( U func, T argc ){ func( argc ); }

template< class U, class T, class... V > 
void iterate( U func, T argc, V... args ){ func( argc ); iterate( func, args... ); }

/*────────────────────────────────────────────────────────────────────────────*/

String fotmat( const char* format, ... ){
  char buffer[256];

  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  return String(buffer);
}

/*────────────────────────────────────────────────────────────────────────────*/

#endif