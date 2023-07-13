#ifndef NODEPP_STRING
#define NODEPP_STRING

#include <initializer_list>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <string>

#include "math.h"
#include "ptr.h"

namespace std { string to_string( string input ){ return input; } }

namespace string {

    int   is_hex( char c ){ return ( ( c>='0' && c<='9' ) || ( c>='A' && c<='F' ) || ( c>='a' && c<='f' ) ); }
    int is_space( char c ){ return ( c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\f'||c=='\v'); }
    int is_alpha( char c ){ return ( ( c>='A' && c<='Z' ) || ( c>='a' && c<='z' ) ); }
    int is_graph( char c ){ return ( c>=33 && c<=126 && c!=' ' ); }
    int is_lower( char c ){ return ( c>='a' && c<='z' ); }
    int is_upper( char c ){ return ( c>='A' && c<='Z' ); }
    int is_digit( char c ){ return ( c>='0' && c<='9' ); }
    int is_print( char c ){ return ( c>=32 && c<=126 ); }
    int is_ascii( char c ){ return ( c>=0 && c<=127 ); }
    int is_contr( char c ){ return ( c<32 || c==127 ); }
    int  is_null( char c ){ return ( c==0 ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t<char> buffer( uint n ){ n++; auto b = ptr_t<char>(n); while(n-->0) b[n]=0; return b; }

    /*────────────────────────────────────────────────────────────────────────────*/

    int is_alnum( char c ){ return ( is_alpha(c) || is_digit(c) ); }
    int is_punct( char c ){ return ( is_print(c) && !is_space(c) && !is_alnum(c) ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    char to_upper( char c ){ if( is_lower(c) ){ return c - 'a' + 'A'; } else { return c; } }
    char to_lower( char c ){ if( is_upper(c) ){ return c + 'a' + 'A'; } else { return c; } }
    int char_code( char c ){ return (int) c; }

}

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

    string_t to_lower_case() noexcept { 
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = tolower( (int)(*x) );
        }   return (*this);
    }

    string_t to_upper_case() noexcept {
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = toupper( (int)(*x) );
        }   return (*this);
    }

    string_t to_capital_case() noexcept {
        bool b=1; for( auto x = this->begin(); x!=this->end(); x++ ){
            if( string::is_lower(*x) && b==1 ){ (*x) = string::to_upper(*x); b=0; continue; }
            if(!string::is_alpha(*x) ){ b=1; }{ (*x) = string::to_lower(*x); }
        }   return (*this);
    }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t replace( function_t<bool,char&> func, char t ) noexcept {
        uint n=0; for( auto x : *this ){ if(func(x)) (*this)[n]=t; n++; } return (*this);
    }

    string_t remove( function_t<bool,char&> func ) noexcept {
        uint n=size(); while( n-->0 ){ if( func((*this)[n]) ) erase(n); } return (*this);
    }

    string_t reverse() const noexcept { string_t n_buffer(this->begin(),this->end());
        uint n=size(); for( auto x : *this ){ n--; n_buffer[n]=x; } return n_buffer;
    }

    string_t copy() noexcept { string_t n_buffer(this->begin(),this->end());
        uint n=size(); for( auto x : *this ){ n--; n_buffer[n]=x; } return n_buffer;
    }

    char reduce( function_t<char,char,char> func ){ char act = *(this->begin());
        for( auto x=this->begin()+1; x != this->end(); x++ )
           { act = func( act, (*x) ); } return act;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t sort( function_t<bool,char&,char&> func ) noexcept {
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

    int index_of( function_t<bool,char&> func ) const noexcept {
        for( auto x : *this ){ if( func(x) ) return x; } return -1;
    }

    int count( function_t<bool,char&> func ) const noexcept { 
        int n=size(); for( auto x : *this ){ if( func(x) ) n++; } return n;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    void unshift( char value ){ self::insert( this->begin(), value ); }
    void push( char value ){ self::insert( this->end(), value ); }
    void shift(){ self::erase( this->begin() ); }
    void pop(){ self::erase( this->end() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool some( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 1; } return 0;
    }

    bool none( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if( func(x) ) return 0; } return 1;
    }

    bool every( function_t<bool,char&> func ) const noexcept { 
        for( auto x : *this ){ if(!func(x) ) return 0; } return 1;
    }

    void map( function_t<void,char&> func ) const noexcept { 
        for( auto x : *this ) func(x);
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t slice( int start=0, int end=0 ){

        if( start < 0 ){ start = size() + start; }    if( end <= 0 ){ end = size() + end; }
        if( (uint)start > size() ){ start = size(); } if( (uint)end > size() ){ end = size(); }
        if( end < start ){ end = size(); }   

        auto a = math::min( this->begin() + end, this->end() );
        auto b = this->begin() + start;

        string_t B( b, a ); uint n=B.size(); 
        while( a-->b && n>0 ){ n--; B[n] = *a; }
        
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t splice( int start=0, uint del=0 ){
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();

        auto a = math::min( this->begin() + del, this->end() );
        auto b = this->begin() + start;
            
        string_t B( b, a ); uint n=B.size(); if( del != 0 )
        while( a-->b && n>0 ){ n--; B[n] = *a; erase(a); }
        
        return B;
    }

    string_t splice( int start, uint del, string_t argc ){
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();
            
        auto a = math::min( this->begin() + del, this->end() );
        auto b = this->begin() + start;

        string_t B( b, a ); uint n=B.size(); if( del != 0 )
        while( a-->b && n>0 ){ n--; B[n] = *a; erase(a); }
        argc.map([&]( char x ){ insert( this->begin() + start, x ); start++; });
        return B;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    explicit operator char*(void) const { return (char*)c_str(); }
    explicit operator bool(void) const { return empty(); }

};

/*────────────────────────────────────────────────────────────────────────────*/

namespace string {

    int to_int( string_t buffer ){ 
        int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%d", &out ); return out;
    }

    bool to_bool( string_t buffer ){ 
        int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%d", &out ); return out;
    }

    double to_double( string_t buffer ){
        double out=0.0f; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%lf", &out ); return out;
    }

    float to_float( string_t buffer ){
        float out=0.0f; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%f", &out ); return out;
    }

    char to_char( string_t buffer ){ 
        char out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%c", &out ); return out;
    }

    uint to_uint( string_t buffer ){
        uint out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%u", &out ); return out;
    }

    void* to_addr( string_t buffer ){
        void* out=nullptr; if( buffer.empty() ) return out;
        sprintf( (char*) buffer, "%p", out ); return out;
    }

    long int to_lint( string_t buffer ){
        long int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%ld", &out ); return out;
    }

    long long int to_llint( string_t buffer ){
        long long int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%lld", &out ); return out;
    }


    unsigned long int to_ulint( string_t buffer ){
        unsigned long int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%lu", &out ); return out;
    }

    unsigned long long int to_ullint( string_t buffer ){
        unsigned long long int out=0; if( buffer.empty() ) return out;
        sscanf( (char*) buffer, "%llu", &out ); return out;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    template< class... T >
    string_t format( string_t str, T... args ){
        char buffer[256] = {0}; sprintf( buffer, (char*)str, args... ); 
        return buffer;
    }

    template< class... T >
    int scan( string_t data, string_t str, T... args ){
        return sprintf( (char*)data, (char*)str, args... ); 
    }

    /*────────────────────────────────────────────────────────────────────────────*/
    
    string_t to_hex( uint num ){
        char buffer[32] = {0}; sprintf( buffer, "%x", num ); return buffer;
    }

    string_t to_bin( uint num ){
        char buffer[32] = {0}; uint n=0; do {
            buffer[n] = num & 1 ? '1' : '0'; num >>= 1; n++;
        }   while (num); return buffer;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t to_string( string_t num ){ return num; }

    string_t to_string( const char* num ){ return num; }

    string_t to_string( char num ){
        char buffer[32] = {0}; sprintf( buffer, "%c", num ); return buffer;
    }

    string_t to_string( uint num ){
        char buffer[32] = {0}; sprintf( buffer, "%u", num ); return buffer;
    }

    string_t to_string( int num ){
        char buffer[32] = {0}; sprintf( buffer, "%d", num ); return buffer;
    }

    string_t to_string( long int num ){
        char buffer[32] = {0}; sprintf( buffer, "%ld", num ); return buffer;
    }

    string_t to_string( unsigned long int num ){
        char buffer[32] = {0}; sprintf( buffer, "%lu", num ); return buffer;
    }

    string_t to_string( long long int num ){
        char buffer[32] = {0}; sprintf( buffer, "%lld", num ); return buffer;
    }

    string_t to_string( unsigned long long int num ){
        char buffer[32] = {0}; sprintf( buffer, "%llu", num ); return buffer;
    }

    string_t to_string( double num ){
        char buffer[32] = {0}; sprintf( buffer, "%lf", num ); return buffer;
    }

    string_t to_string( float num ){
        char buffer[32] = {0}; sprintf( buffer, "%f", num ); return buffer;
    }

    template< class T > string_t to_string( T* num ){
        char buffer[32] = {0}; sprintf( buffer, "%p", (void*)num ); return buffer;
    }

    template< class U, class T >
    void map( U func, T argc ){ func( to_string(argc) ); }

    template< class U, class T, class... V > 
    void map( U func, T argc, V... args ){ func( to_string(argc) ); string::map( func, args... ); }

}

#endif