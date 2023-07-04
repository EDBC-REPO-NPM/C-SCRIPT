#ifndef NODEPP_ALGORITHM
#define NODEPP_ALGORITHM
#define CHUNK_SIZE 65536

#include <unordered_map>
#include <stdexcept>
#include <map>

/*────────────────────────────────────────────────────────────────────────────*/

template< class T, class U > using umap_t = std::unordered_map< T, U >;
                             using err_t  = std::runtime_error;
template< class T, class U > using map_t  = std::map< T, U >;

/*────────────────────────────────────────────────────────────────────────────*/

template< class U, class T >
void iterate( U func, T argc ){ func( argc ); }

template< class U, class T, class... V > 
void iterate( U func, T argc, V... args ){ func( argc ); iterate( func, args... ); }

/*────────────────────────────────────────────────────────────────────────────*/

#include "coroutine.h"
#include "function.h"
#include "string.h"
#include "array.h"
#include "regex.h"
#include "ptr.h"

/*────────────────────────────────────────────────────────────────────────────*/

using header_t = map_t< string_t, string_t >;
using query_t  = map_t< string_t, string_t >;

/*────────────────────────────────────────────────────────────────────────────*/

template< class... T >
string_t format( T... args ){ return regex::format( args... ); }

/*────────────────────────────────────────────────────────────────────────────*/

#endif