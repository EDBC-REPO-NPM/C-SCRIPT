#ifndef NODEPP_STRING
#define NODEPP_STRING

#include <initializer_list>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <string>

/*────────────────────────────────────────────────────────────────────────────*/

namespace std { string to_string( string input ){ return input; } }

/*────────────────────────────────────────────────────────────────────────────*/

class string_t : public std::string { public:

    using self = std::string;

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t() : self() {}

    string_t( const char* s ) : self( s ) {}
    string_t( const self& str ) : self( str ) {}
    string_t( const string_t& str ) : self( str ) {}
    string_t( std::size_t n, char c ) : self( n, c ) {}
    string_t( const char* s, std::size_t n ) : self( s, n ) {}

    template< class type >  
    string_t( type first, type last ) : self ( first, last ) {}
    string_t( const self& str, std::size_t pos, std::size_t len=npos ) : self( str, pos, len ) {}
    string_t( const string_t& str, std::size_t pos, std::size_t len=npos ) : self( str, pos, len ) {}
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t& operator=( const string_t& x ) { self::operator=(x); return (*this); }
    string_t& operator=( const self& x ) { self::operator=(x); return (*this); }
    string_t& operator=( string_t&& x ) { self::operator=(x); return (*this); }
    string_t& operator=( self&& x ) { self::operator=(x); return (*this); }

    template< class... T >
    string_t& operator=( T... x ){ self::operator=(x...); return (*this); }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t to_lower_case(){ 
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = tolower( (int)(*x) );
        }   return (*this);
    }

    string_t to_upper_case(){ 
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = toupper( (int)(*x) );
        }   return (*this);
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace( function_t<bool,char&> func, char targ ){ 
        std::replace_if( this->begin(), this->end(), func, targ );
        return (*this); 
    }

    string_t remove( function_t<bool,char&> func ){ 
        std::remove_if( this->begin(), this->end(), func ); 
        return (*this); 
    }

    string_t reverse(){ string_t B( this->begin(), this->end() ); 
        std::reverse_copy( this->begin(), this->end(), B.begin() ); 
        return B; 
    }

    char reduce( function_t<char,char&,char&> func ){ char act = {0};
        for( auto x=this->begin(); x != this->end(); x++ )
           { act = func( act, (*x) ); } return act;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t sort( function_t<bool,char&,char&> func ){
        string_t B( this->begin(), this->end() ); 
        std::sort( B.begin(), B.end(), func ); return B;
    } 
    
    /*────────────────────────────────────────────────────────────────────────────*/

    int index_of( function_t<bool,char&> func ){ int i=0; 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) return i; i++; } return -1;
    }

    char& value_of( function_t<bool,char&> func ){ 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) return (*x); } 
    }

    int count( function_t<bool,char&> func ){ int i=0; 
        for( auto x=this->begin(); x != this->end(); x++ )
           { if( func(*x)==true ) i++; } return i; 
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    void unshift( char value ){ self::insert( this->begin(), value ); }
    void push( char value ){ self::insert( this->end(), value ); }
    void shift(){ self::erase( this->begin() ); }
    void pop(){ self::erase( this->end() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool some( function_t<bool,char&> func ){ 
        return std::any_of( this->begin(), this->end(), func ); 
    }

    bool none( function_t<bool,char&> func ){ 
        return std::none_of( this->begin(), this->end(), func ); 
    }

    bool every( function_t<bool,char&> func ){ 
        return std::all_of( this->begin(), this->end(), func ); 
    }

    void map( function_t<void,char&> func ){ 
         for( auto x=this->begin(); x != this->end(); x++ ) func(*x); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t slice( int start=0, int end=0 ){

        if( start < 0 ){ start = this->size() + start; }    if( end < 0 ){ end = this->size() + end; }
        if((uint)start > this->size() ){ start = this->size(); } if((uint)end > this->size() ){ end = this->size(); }
        if( end < start ){ end = this->size(); }

        auto max = std::min( this->begin()+end, this->end() );
        auto min = this->begin()+start;

        string_t B( min, max );
         std::copy( min, max, B.begin() ); 
        
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t splice( int start=0, int del=0 ){
        
        if( start < 0 ){ start = this->size() + start; } if((uint)start > this->size() ){ start = this->size(); }
            del += start; if( del < 0 ) del = 0; 

        auto max = std::min( this->begin()+del, this->end() );
        auto min = this->begin()+start;
        auto fuc = []( char x ){ return true; };
            
        string_t B( min, max ); if( del != 0 ) 
        self::erase( std::remove_if( min, max, fuc ), max );
        
        return B;
    }

    template< class... U >
    string_t splice( int start, int del, string_t argc, U... args ){
        
        if( start < 0 ){ start = this->size() + start; if((uint)start > this->size() ) start = this->size(); }
            del += start; if( del < 0 ) del = 0; 
            
        auto max = std::min( this->begin()+del, this->end() );
        auto min = this->begin()+start;
        auto fuc = []( char x ){ return true; };
            
        string_t B( min, max ); if( del != 0 ) 
        self::erase( std::remove_if( min, max, fuc ), max );
        
        iterate( [&]( string_t x ){ 
            for( auto y:x ){ this->insert( this->begin()+start, y ); } start++; 
        }, argc, args... ); 
        
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t join( string_t c="," ){ string_t B; 
        for( auto i=this->begin(); i!=this->end(); i++ ){
           { B+=(*i); } if( i+1 != this->end() ) { B+=c; }
        }   return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

};

#endif