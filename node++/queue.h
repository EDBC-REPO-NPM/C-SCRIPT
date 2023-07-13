#ifndef NODEPP_QUEUE
#define NODEPP_QUEUE

#include "ptr.h"

namespace QUEUE_NODEPP {
    template< class T >
    class NODE { public: T data; 
        NODE( T value ){ data = value; }
        NODE* nxt = nullptr; NODE* prv = nullptr;
    };
}

template< class V >
class queue_t {

    using self = QUEUE_NODEPP::NODE<V>;

    protected: 
        self* act = nullptr;
        ptr_t<self> queue;

    /*────────────────────────────────────────────────────────────────────────────*/
    public: queue_t(){}

    template< class... T > 
    queue_t( V argc, T... args ){ 
        self* n = nullptr; iterate([&]( V arg ){
        if( n == nullptr ){ queue = new self( arg ); n = &queue; } 
        else {
            n->nxt = new self( arg );
            n->nxt->prv = n;
            n = n->nxt;
        }
    }, argc, args... ); }

   ~queue_t(){ if( queue.count() <= 1 ) clear(); }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool empty() const noexcept { return queue == nullptr; }

    uint size() const noexcept { 
        if( empty() ) return 0; auto n = first(); uint i=1;
        while( n->nxt != nullptr ){ n = n->nxt; i++; } return i;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    V* operator&( void ) const noexcept { return act==nullptr ? first() : act; }

    V operator[]( uint i ) const noexcept { auto n = first();
        while( n->nxt != nullptr && i-->1 ){ n = n->nxt; } 
        return n->data;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    int index_of( function_t<bool,V&> func ) const noexcept {
        int i=0; self* n = first();
        if( empty() ) return -1; while( n!=nullptr ) { 
            if( func(n->data)== 1 ) return i; 
            if( n->nxt == nullptr ) break;
            i++; n = n->nxt;
        }   return -1;
    }

    int count( function_t<bool,V&> func ) const noexcept { 
        int i=0; self* n = first();
        if( empty() ) return 0; while( n!=nullptr ) { 
            if( func(n->data)== 1 ) i++; 
            if( n->nxt == nullptr ) break;
                n = n->nxt;
        }   return i;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool some( function_t<bool,V&> func ) const noexcept {
        if( empty() ) return false;
        self* n = first(); while( n!=nullptr ) { 
            if( func(n->data)== 1 ) return 1; 
            if( n->nxt == nullptr ) break;
                n = n->nxt;
        }   return 0;
    }

    bool none( function_t<bool,V&> func ) const noexcept {
        if( empty() ) return false;
        self* n = first(); while( n!=nullptr ) { 
            if( func(n->data)== 1 ) return 0; 
            if( n->nxt == nullptr ) break;
                n = n->nxt;
        }   return 1;
    }

    bool every( function_t<bool,V&> func ) const noexcept {
        if( empty() ) return false;
        self* n = first(); while( n!=nullptr ) { 
            if( func(n->data)== 0 ) return 0; 
            if( n->nxt == nullptr ) break;
                n = n->nxt;
        }   return 1;
    }

    void map( function_t<void,V&> func ) const noexcept {
        if( empty() ) return; 
        self* n = first(); while( n!=nullptr ) { 
            func( n->data ); n = n->nxt;
        }
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void unshift( V value ){ insert( first(), value ); }
    void push( V value ){ insert( last(), value ); }
    void clear(){ while(!empty() ){ pop(); } }
    void shift(){ erase( first() ); }
    void pop(){ erase( last() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void insert( uint index, V value ){ insert( get(index), value ); }

    void insert( self* index, V value ){
        if( empty() ){ queue = new self( value ); } 
        else {
            if( index == last() ){
                index->nxt = new self( value );
                index->nxt->prv = index;
            } else if( index == first() ){
                auto prev = *queue; queue = new self( value );
                queue->nxt= new self( prev ); 
                queue->nxt->prv = first();
            } else {
                index->nxt = new self( value ); 
                index->nxt->prv = index;
            }
        }
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void erase( uint index ){ erase( get(index) ); }

    void erase( self* x ){
        if( x == nullptr ) return; act = nullptr;
        if( x == last() ){
              if ( x == first() ){ queue.reset(); }
            else { x->prv->nxt = x->nxt; delete x; }
        } else if( x == first() ){
            queue = queue->nxt;
        } else {
            x->prv->nxt = x->nxt;
            x->nxt->prv = x->prv; delete x;
        }
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    self* get( int i=-2 ) const noexcept { if( empty() ) return (self*) nullptr;
        if( i==-2 ) return act==nullptr ? first() : act; auto n=first(); 
        while( n->nxt != nullptr && i-->0 ){ n = n->nxt; } return n;
    }

    self* first() const noexcept { return &queue; }

    self* last() const noexcept {
        if( empty() ) return (self*) nullptr; auto n=first(); 
        while( n->nxt != nullptr ){ n = n->nxt; } return n;
    }
    
    self* prev(){ if( empty() ) return (self*) nullptr;
        if( act == nullptr ){ act = last(); return act; }
        act = act->prv == nullptr ? last() : act->prv; return act;
    }
    
    self* next(){ if( empty() ) return (self*) nullptr;
        if( act == nullptr ){ act = first(); return act; }
        act = act->nxt == nullptr ? first() : act->nxt; return act;
    }

};

#endif