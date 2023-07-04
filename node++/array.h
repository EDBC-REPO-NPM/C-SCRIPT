#ifndef NODEPP_ARRAY
#define NODEPP_ARRAY

#include <initializer_list>
#include <algorithm>
#include <memory>
#include <vector>

template< class T >
class array_t : public std::vector<T> { public:

    using Allocator = std::allocator<T>;
    using self = std::vector<T>;

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t() : self() {} 
    array_t( const Allocator& alloc ) : self(alloc) {}

    array_t( self&& x ) : self(x) {}
    array_t( array_t&& x ) : self(x) {}
    array_t( self&& x, const Allocator& alloc ) : self( x, alloc ) {}
    array_t( array_t&& x, const Allocator& alloc ) : self( x, alloc ) {}

    array_t( const self& x ) : self(x) {}
    array_t( const array_t& x ) : self( x ) {}    
    array_t( const self& x, const Allocator& alloc ) : self( x, alloc ) {}
    array_t( const array_t& x, const Allocator& alloc ) : self( x, alloc ) {}

    array_t( uint n, const Allocator& alloc = Allocator()) : self( n, alloc ) {}
    array_t( uint n, const T& val, const Allocator& alloc = Allocator() ) : self( n, val, alloc ) {}
    array_t( std::initializer_list<T> il, const Allocator& alloc = Allocator() ) : self( il, alloc ) {}

    template< class type >
    array_t( type first, type last, const Allocator& alloc = Allocator() ) : self( first, last, alloc ) {}

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t& operator=( const array_t& x ) { self::operator=(x); return (*this); }
    array_t& operator=( const self& x ) { self::operator=(x); return (*this); }
    array_t& operator=( array_t&& x ) { self::operator=(x); return (*this); }
    array_t& operator=( self&& x ) { self::operator=(x); return (*this); }

    template< class... V >
    array_t& operator=( V... x ){ self::operator=(x...); return (*this); }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    array_t replace( function_t<bool,T&> func, T targ ){ 
        std::replace_if( this->begin(), this->end(), func, targ );
        return (*this); 
    }

    array_t remove( function_t<bool,T&> func ){ 
        std::remove_if( this->begin(), this->end(), func ); 
        return (*this); 
    }

    array_t reverse(){ array_t B( this->begin(), this->end() ); 
        std::reverse_copy( this->begin(), this->end(), B.begin() ); 
        return B; 
    }

    T reduce( function_t<T,T&,T&> func ){ T act = {0};
        for( auto x=this->begin(); x != this->end(); x++ )
           { act = func( act, (*x) ); } return act;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    T sort( function_t<bool,T&,T&> func ){
        array_t B( this->begin(), this->end() ); 
        std::sort( B.begin(), B.end(), func ); return B;
    } 

    /*────────────────────────────────────────────────────────────────────────────*/

    int index_of( function_t<bool,T&> func ){ int i=0; 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) return i; i++; } return -1;
    }

    T& value_of( function_t<bool,T&> func ){ 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) return (*x); } 
    }

    int count( function_t<bool,T&> func ){ int i=0; 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) i++; } return i; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void unshift( T value ){ self::emplace( this->begin(), value ); }
    void push( T value ){ self::emplace( this->end(), value ); }
    void shift(){ self::erase( this->begin() ); }
    void pop(){ self::erase( this->end() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool some( function_t<bool,T&> func ){ 
        return std::any_of( this->begin(), this->end(), func ); 
    }

    bool none( function_t<bool,T&> func ){ 
        return std::none_of( this->begin(), this->end(), func ); 
    }

    bool every( function_t<bool,T&> func ){ 
        return std::all_of( this->begin(), this->end(), func ); 
    }

    void map( function_t<void,T&> func ){ 
        for( auto x=this->begin(); x != this->end(); x++ ) func(*x); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t slice( int start=0, int end=0 ){

        if( start < 0 ){ start = this->size() + start; }    if( end < 0 ){ end = this->size() + end; }
        if( (uint)start > this->size() ){ start = this->size(); } if( (uint)end > this->size() ){ end = this->size(); }
        if( end < start ){ end = this->size(); }   

        auto max = std::min( this->begin()+end, this->end() );
        auto min = this->begin()+start;
        
        array_t B( min, max );
        std::copy( min, max, B.begin() ); 
        
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    array_t splice( int start=0, int del=0 ){
        
        if( start < 0 ){ start = this->size() + start; } if( (uint)start > this->size() ){ start = this->size(); }
            del += start; if( del < 0 ) del = 0;

        auto max = std::min( this->begin()+del, this->end() );
        auto min = this->begin()+start;
        auto fuc = []( T x ){ return true; };

        array_t B( min, max ); if( del != 0 ) 
        self::erase( std::remove_if( min, max, fuc ), max );
        
        return B;
    }

    template< class... U >
    array_t splice( int start, uint del, T argc, U... args ){
        
        if( start < 0 ){ start = this->size() + start; if( (uint)start > this->size() ) start = this->size(); }
            del += start; if( del < 0 ) del = 0;

        auto max = std::min( this->begin()+del, this->end() );
        auto min = this->begin()+start;
        auto fuc = []( T x ){ return true; };

        array_t B( min, max ); if( del != 0 ) 
        self::erase( std::remove_if( min, max, fuc ), max );
        
        iterate( [&]( T x ){ this->emplace( this->begin()+start, x ); start++; }, argc, args... );
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t join( string_t c="," ){
        string_t B; for( auto i=this->begin(); i!=this->end(); i++ ){
            if( !std::is_same<string_t,T>::value && !std::is_same<std::string,T>::value && !std::is_same<char*,T>::value )
            { B += std::to_string(*i); } else { B += (*i); } if( i+1 != this->end() ) { B += c; }
        }   return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

};

#endif