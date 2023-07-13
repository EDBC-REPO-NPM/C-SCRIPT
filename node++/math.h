#ifndef NODEPP_MATH
#define NODEPP_MATH

#include <cmath>

namespace math { uint rand(){ return ::rand(); }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class T > int floor( T args ){ return ::floor( args ); }
    template< class T > int ceil( T args ){ return  ::ceil( args );  }
    template< class T > T abs( T args ){ return     ::abs( args );   }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class T > T min( T min, T max ){ return min<max ? min : max; }
    template< class T > T max( T min, T max ){ return max>min ? max : min; }

    /*────────────────────────────────────────────────────────────────────────────*/
    
    template< class T > T clamp( T _val, T _min, T _max ){ return max( _min, min( _val, _max ) ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class... T > double sqrt( T... args ){ return  ::sqrt( args... );  }
    template< class... T > double pow( T... args ){ return   ::pow( args... );   }
    
    template< class... T > double fmod( T... args ){ return  ::fmod( args... );  }
    template< class... T > double modf( T... args ){ return  ::modf( args... );  }
    template< class... T > double fabs( T... args ){ return  ::fabs( args... );  }

    template< class... T > double log10( T... args ){ return ::log10( args... ); }
    template< class... T > double exp( T... args ){ return   ::exp( args... );   }
    template< class... T > double log( T... args ){ return   ::log( args... );   }

    template< class... T > double cos( T... args ){ return   ::cos( args... );   }
    template< class... T > double sin( T... args ){ return   ::sin( args... );   }
    template< class... T > double tan( T... args ){ return   ::tan( args... );   }

    template< class... T > double atan2( T... args ){ return ::atan2( args... ); }
    template< class... T > double acos( T... args ){ return  ::acos( args... );  }
    template< class... T > double asin( T... args ){ return  ::asin( args... );  }
    template< class... T > double atan( T... args ){ return  ::atan( args... );  }
 
    template< class... T > double cosh( T... args ){ return  ::cosh( args... );  }
    template< class... T > double sinh( T... args ){ return  ::sinh( args... );  }
    template< class... T > double tanh( T... args ){ return  ::tanh( args... );  }

}

#endif