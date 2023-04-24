#ifndef ITERATOR_CSCRIPT
#define ITERATOR_CSCRIPT
 
using namespace std;
namespace std {

    template< class U, class T >
    void pack( U func, T arg ){ func( arg ); }

    template< class U, class T, class... V > 
    void pack( U func, T arg, V... args ){ func( arg ); pack( func, args... ); }

}

#endif 