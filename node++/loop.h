#ifndef NODEPP_LOOP
#define NODEPP_LOOP
#include "queue.h"

namespace process {

    namespace loop {

        queue_t<function_t<int>> queue;

        bool empty(){ return queue.empty(); }

        template< class T, class... V >
        void add( T _func, V... arg ){ 
            queue.push([=](){ return _func( arg... ); });
        }

        void next(){
            if( queue.empty() ){ return; }
            int result = queue.get()->data();
                 if( result >= 0 ) queue.next();
            else if( result <  0 ) queue.erase( queue.get() );
        }

    }

    namespace poll {

        queue_t<function_t<int>> queue;

        bool empty(){ return queue.empty(); }

        template< class T, class... V >
        void add( T _func, V... arg ){ 
            queue.push([=](){ return _func( arg... ); });
        }

        void next(){ 
            if( queue.empty() ){ return; }
            int result = queue.get()->data();
                 if( result == 0 ) queue.next();
            else if( result <  0 ) queue.erase( queue.get() );
        }

    }

}
#endif