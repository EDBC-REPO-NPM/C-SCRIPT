#ifndef NODEPP_ARRAY
#define NODEPP_ARRAY

#include <cstring>

#include "math.h"
#include "ptr.h"

template< class T >
class array_t {
    
    protected: ptr_t<T> buffer;

    /*────────────────────────────────────────────────────────────────────────────*/
    public:

    array_t() noexcept = default;

    virtual ~array_t(){  }

    array_t( const T* val ) noexcept {
        if( val == nullptr ) return; uint n = 0; 
        while( !val[n] != '\0' && n < sizeof(val) ){ n++; }
        buffer = ptr_t<T>( n ); memcopy( &buffer, val, n );
    }

    array_t( const uint& n ) noexcept { 
        if( n == 0 ) return; buffer = ptr_t<T>(n);
    }

    array_t( const T& val, uint n ) noexcept { 
        if( n == 0 ) return; buffer = ptr_t<T>(n); 
        while( n-->0 ){ (*this)[n] = val; }
    }

    array_t( const T* val, const uint& n ) noexcept { 
        if( val != nullptr || n == 0 ) return; 
        buffer = ptr_t<T>(n); memcopy( &buffer, val, n );
    }

    array_t( const ptr_t<T>& argc ) noexcept { buffer = argc; }

    template< class... V >
    array_t( T argc, V... args ) noexcept { uint n = 0; 
        ::map([&]( T arg ){ n++; }, argc, args... ); buffer = ptr_t<T>(n); n=0;
        ::map([&]( T arg ){ buffer[n] = arg; n++; }, argc, args... ); 
    }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    T* begin() const noexcept { return &buffer; }
    T* end() const noexcept { return &buffer+size(); }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint first() const noexcept { return 0; }
    bool empty() const noexcept { return buffer.empty(); }
    uint  size() const noexcept { return empty() ? 0 : buffer.size() - 0; }
    uint  last() const noexcept { return empty() ? 0 : buffer.size() - 1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t operator+=( const array_t& oth ){ 
        array_t ths = copy(); uint n = 0;
        buffer = ptr_t<T>( this->size() + oth.size() );
        for( auto x : ths ){ (*this)[n] = x; n++; }
        for( auto x : oth ){ (*this)[n] = x; n++; } return *this;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator> ( const array_t& oth ) const noexcept { return compare( oth ) == 1; }
    bool operator>=( const array_t& oth ) const noexcept { return compare( oth ) >= 0; }
    bool operator<=( const array_t& oth ) const noexcept { return compare( oth ) <= 0; }
    bool operator< ( const array_t& oth ) const noexcept { return compare( oth ) ==-1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator==( const array_t& oth ) const noexcept { return compare( oth ) == 0; }
    bool operator!=( const array_t& oth ) const noexcept { return compare( oth ) != 0; }

    T& operator[]( uint n ) const noexcept { return buffer[n]; }

    /*────────────────────────────────────────────────────────────────────────────*/

    int index_of( function_t<bool,T&> func ) const noexcept {
        for( auto x : *this ){ if( func(x) ) return x; } return -1;
    }

    int count( function_t<bool,T&> func ) const noexcept { 
        int n=0; for( auto x : *this ){ if( func(x) ) n++; } return n;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    T reduce( function_t<T,T&,T&> func ) const noexcept { T act = {0};
        for( auto x : *this ){ act = func( act, x ); } return act;
    }

    bool some( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 1; } return 0;
    }

    bool none( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    bool every( function_t<bool,T&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    void map( function_t<void,T&> func ) const noexcept { 
        for( auto x : *this ) func(x);
    }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    int compare( const array_t& oth ) const noexcept {
        if( size() < oth.size() ) return -1; 
        if( size() > oth.size() ) return  1; 
        uint n = size(); while( n-->0 ){
            if( (*this)[n] < oth[n] )  return -1;
            if( (*this)[n] > oth[n] )  return  1;
        }   return 0;
    }

    array_t remove( function_t<bool,T&> func ) noexcept {
        int s = count( func ); if( s<=0 ) return (*this);
        auto n_buffer = ptr_t<T>(size()-s);
        uint n=0; for( auto x : *this )
           { if( !func(x) ){ n_buffer[n]=x; n++; } }
        buffer = n_buffer; return (*this);
    }

    array_t copy() const noexcept { auto n_buffer = ptr_t<T>(size());
        uint n=first(); for( auto x : *this ){ n_buffer[n]=x; n++; } return n_buffer;
    }

    array_t reverse() const noexcept { auto n_buffer = ptr_t<T>(size());
        uint n=last(); for( auto x : *this ){ n_buffer[n]=x; n--; } return n_buffer;
    }

    array_t replace( function_t<bool,char&> func, char targ ) noexcept {
        for( auto x : *this ){ if(func(x)) x=targ; }
        return (*this); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t sort( function_t<bool,T&,T&> func ) noexcept {
        auto n_buffer = copy();

        while(1){ int nn = 0;
            for( uint i=0; i<size(); i++ ){
                int act=i, prv = i-1; if( prv<0 ) continue;
                char _act = n_buffer[act], _prv = n_buffer[prv];
                if( func( _prv, _act ) == 0 ){ continue; } nn++;
                n_buffer[act] = _prv; n_buffer[prv] = _act;
            }   if( nn == 0 ) break;
        }
        
        return n_buffer;
    } 

    /*────────────────────────────────────────────────────────────────────────────*/

    void unshift( T value ) noexcept { insert( first(), value ); }
    void push( T value ) noexcept { insert( size(), value ); }
    void shift() noexcept { erase( first() ); }
    void pop() noexcept { erase( last() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void clear() noexcept { buffer.reset(); }
    void erase() noexcept { buffer.reset(); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void insert( uint index, T& value ) noexcept {
        if( empty() ){ buffer = ptr_t<T>(1); buffer[0] = value; } 
        else { 
            if( index > size() ) return; uint i=size(), n=size()+1;
                                 auto n_buffer = ptr_t<T>(n);
            while( n-->0 ){ 
                if( n == index ){ n_buffer[n] = value; }
                else { i--; n_buffer[n] = buffer[i]; }
            }   buffer = n_buffer;
        }
    }

    void erase( uint index ) noexcept {
        if( empty() ) return; else {
            if( index >= size() ) return; auto n_buffer = ptr_t<T>(last());
            for( uint i=0, j=0; i<size(); i++ )
                if( i != index ){ n_buffer[j] = buffer[i]; j++; }
                buffer = n_buffer;
        }
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t slice( int start=0, int end=0 ) noexcept {

        if( start < 0 ){ start = size() + start; }    if( end <= 0 ){ end = size() + end; }
        if( (uint)start > size() ){ start = size(); } if( (uint)end > size() ){ end = size(); }
        if( end < start ){ end = size(); }   

        uint a = math::min( first() + end, size() );
        uint b = first() + start, _sz = a - b;

        auto n_buffer = ptr_t<T>(_sz); uint n = _sz;
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; }
        
        return n_buffer;
    }

    /*────────────────────────────────────────────────────────────────────────────*/
 
    array_t splice( int start=0, uint del=0 ) noexcept {
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();

        auto a = math::min( first() + del, size() );
        auto b = first() + start, _sz = a - b;

        auto n_buffer = ptr_t<T>(_sz); uint n = _sz; if( del != 0 )
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; erase(a); }
        
        return n_buffer;
    }

    template< class... U >
    array_t splice( int start, uint del, T argc, U... args ) noexcept {
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();
    
        auto a = math::min( first() + del, size() );
        auto b = first() + start, _sz = a - b;

        auto n_buffer = ptr_t<T>(_sz); uint n = _sz; if( del != 0 )
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; erase(a); }
        
        ::map([&]( T x ){ insert( start, x ); start++; }, argc, args... );
        return n_buffer;
    }

    /*────────────────────────────────────────────────────────────────────────────*/
    
    explicit operator bool(void) const { return empty(); }
    explicit operator T*(void) const { return &buffer; }
    const T* c_str() const noexcept { return &buffer; }

};

/*────────────────────────────────────────────────────────────────────────────*/

template< class T >
array_t<T> operator+( const array_t<T>& A, const array_t<T>& B ){
    array_t<T> C = ptr_t<T>( A.size()+B.size() ); uint n = 0;
    for( auto x : A ){ C[n] = x; n++; }
    for( auto x : B ){ C[n] = x; n++; } return C;
}

#endif