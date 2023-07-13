#ifndef NODEPP_ALGORITHM
#define NODEPP_ALGORITHM
#define CHUNK_SIZE 65536

/*────────────────────────────────────────────────────────────────────────────*/

using uint   = unsigned int;
using uchar  = unsigned char;
using ulong  = unsigned long int;
using ullong = unsigned long long int;

/*────────────────────────────────────────────────────────────────────────────*/

#include <stdexcept>
using err_t  = std::runtime_error;

/*────────────────────────────────────────────────────────────────────────────*/

template< class U, class T >
void map( U func, T argc ){ func( argc ); }

template< class U, class T, class... V > 
void map( U func, T argc, V... args ){ func( argc ); map( func, args... ); }

/*────────────────────────────────────────────────────────────────────────────*/

#include "coroutine.h"
#include "function.h"
#include "console.h"

/*────────────────────────────────────────────────────────────────────────────*/

#endif