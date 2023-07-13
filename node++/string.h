#ifndef NODEPP_STRING
#define NODEPP_STRING

#include "ptr.h"

namespace string {

    int   is_hex( char c ){ return ( ( c>='0' && c<='9' ) || ( c>='A' && c<='F' ) || ( c>='a' && c<='f' ) ); }
    int is_space( char c ){ return ( c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\f'||c=='\v'); }
    int is_alpha( char c ){ return ( ( c>='A' && c<='Z' ) || ( c>='a' && c<='z' ) ); }
    int is_graph( char c ){ return ( c>=33 && c<=126 && c!=' ' ); }
    int is_lower( char c ){ return ( c>='a' && c<='z' ); }
    int is_upper( char c ){ return ( c>='A' && c<='Z' ); }
    int is_digit( char c ){ return ( c>='0' && c<='9' ); }
    int is_print( char c ){ return ( c>=32 && c<=126 ); }
    int is_ascii( char c ){ return ( c>=0 && c<=126 ); }
    int is_contr( char c ){ return ( c<32 || c==127 ); }
    int  is_null( char c ){ return ( c==0 ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    ptr_t<char> buffer( uint n ){ n++; auto b = ptr_t<char>(n); while(n-->0) b[n]='\0'; return b; }
    ptr_t<char> null(){ auto b = ptr_t<char>((uint)0); b[0]='\0'; return b; }

    /*────────────────────────────────────────────────────────────────────────────*/
    
    int is_alnum( char c ){ return ( is_alpha(c) || is_digit(c) ); }
    int is_punct( char c ){ return ( is_print(c) && !is_space(c) && !is_alnum(c) ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    char to_upper( char c ){ if( is_lower(c) ){ return c - 'a' + 'A'; } else { return c; } }
    char to_lower( char c ){ if( is_upper(c) ){ return c + 'a' + 'A'; } else { return c; } }
    int char_code( char c ){ return (int) c; }

}

/*────────────────────────────────────────────────────────────────────────────*/

class string_t {
    
    protected: ptr_t<char> buffer;

    /*────────────────────────────────────────────────────────────────────────────*/
    public:
    virtual ~string_t(){  }

    string_t( const uint& n ) noexcept {
        buffer = string::buffer(n);
    }

    string_t( const char& argc ) noexcept {
        buffer = string::buffer(1); buffer[0] = argc;
    }

    string_t( const char& val, uint n ) noexcept {
        if( n==0 ){ 
            buffer = string::null(); return;
        }   buffer = string::buffer(n);
        while( n-->0 ){ (*this)[n] = val; }
    }

    string_t() noexcept { buffer = string::null(); }

    string_t( const char* argc ) noexcept {  uint n=0;
        if( argc == nullptr || (n=strlen(argc))==0 ){ 
            buffer = string::null(); return;
        }   buffer = string::buffer(n); memcpy( &buffer, argc, n );
    }

    string_t( const char* argc, uint n ) noexcept {
        if( argc == nullptr || n == 0 ){ 
            buffer = string::null(); return;
        }   buffer = string::buffer(n);
        memcpy( &buffer, argc, n );
    }

    string_t( const ptr_t<char>& argc ) noexcept { buffer = argc; }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    char* end() const noexcept { return &buffer + size(); }

    char* begin() const noexcept { return &buffer; }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint first() const noexcept { return 0; }
    bool empty() const noexcept { return buffer.empty(); }
    uint  size() const noexcept { return empty() ? 0 : buffer.size() - 1; }
    uint  last() const noexcept { return empty() ? 0 : buffer.size() - 2; }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t operator+=( const string_t& oth ){ 
        if( oth.empty() ){ return *this; } string_t ths = copy(); uint n = 0;
        buffer = string::buffer( this->size() + oth.size() );
        for( auto x : ths ){ (*this)[n] = x; n++; }
        for( auto x : oth ){ (*this)[n] = x; n++; } return *this;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator> ( const string_t& oth ) const noexcept { return compare( oth ) == 1; }
    bool operator>=( const string_t& oth ) const noexcept { return compare( oth ) >= 0; }
    bool operator<=( const string_t& oth ) const noexcept { return compare( oth ) <= 0; }
    bool operator< ( const string_t& oth ) const noexcept { return compare( oth ) ==-1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool operator==( const string_t& oth ) const noexcept { return compare( oth ) == 0; }
    bool operator!=( const string_t& oth ) const noexcept { return compare( oth ) != 0; }
    
    char& operator[]( uint n ) const noexcept { return buffer[n]; }

    /*────────────────────────────────────────────────────────────────────────────*/

    int index_of( function_t<bool,char&> func ) const noexcept {
        for( auto x : *this ){ if( func(x) ) return x; } return -1;
    }

    int count( function_t<bool,char&> func ) const noexcept { 
        int n=0; for( auto x : *this ){ if( func(x) ) n++; } return n;
    }

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

    int compare( const string_t& oth ) const noexcept {
        if( size() < oth.size() )     return -1; 
        if( size() > oth.size() )     return  1; 
        uint n = size(); while( n-->0 ){
            if( (*this)[n] < oth[n] ) return -1;
            if( (*this)[n] > oth[n] ) return  1;
        }   return 0;
    }
        
    /*────────────────────────────────────────────────────────────────────────────*/

    char reduce( function_t<char,char&,char&> func ) noexcept { char act = (*this->begin());
        for( auto x=this->begin() + 1; x != this->end(); x++ )
           { act = func( act, *x ); } return act;
    }

    string_t replace( function_t<bool,char&> func, char t ) noexcept {
        uint n=0; for( auto x : *this ){ if(func(x)) (*this)[n]=t; n++; } return (*this);
    }

    string_t remove( function_t<bool,char&> func ) noexcept {
        uint n=size(); while( n-->0 ){ if( func((*this)[n]) ) erase(n); } return (*this);
    }

    string_t reverse() noexcept { auto n_buffer = copy();
        uint n=size(); for( auto x : *this ){ n--; n_buffer[n]=x; } return n_buffer;
    }

    string_t copy() const noexcept { auto n_buffer = string::buffer(size()); 
        memcpy( &n_buffer, &buffer, size() ); return n_buffer;
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

    void unshift( char value ) noexcept { insert( first(), value ); }
    void push( char value ) noexcept { insert( size(), value ); }
    void shift() noexcept { erase( first() ); }
    void clear() noexcept { buffer.reset(); }
    void pop() noexcept { erase( last() ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    void insert( uint index, const char& value ) noexcept {
        if( empty() ){ buffer = string::buffer(1); buffer[0] = value; } 
        else { 
            if( index > size() ) return; uint i=size(), n=size()+1;
                                 auto n_buffer = string::buffer(n);
            while( n-->0 ){ 
                if( n == index ){ n_buffer[n] = value; }
                else { i--; n_buffer[n] = buffer[i]; }
            }   buffer = n_buffer;
        }
    }

    void erase( uint index ) noexcept {
        if( empty() ) return; else {
            if( index >= size() ) return; auto n_buffer = string::buffer(last());
            for( uint i=0, n=0; i<size(); i++ )
                if( i != index ){ n_buffer[n] = buffer[i]; n++; }
                buffer = n_buffer;
        }
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t slice( int start=0, int end=0 ) noexcept { if(empty()) return *this; 

        if( start < 0 ){ start = size() + start; }    if( end <= 0 ){ end = size() + end; }
        if( (uint)start > size() ){ start = size(); } if( (uint)end > size() ){ end = size(); }
        if( end < start ){ end = size(); }   

        uint a = min( first() + end, size() );
        uint b = first() + start, _sz = a - b;

        auto n_buffer = string::buffer(_sz); uint n = _sz;
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; }
        
        return n_buffer;
    }

    /*────────────────────────────────────────────────────────────────────────────*/
 
    string_t splice( int start=0, uint del=0 ) noexcept { if(empty()) return *this;
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();

        uint a = min( first() + del, size() );
        uint b = first() + start, _sz = a - b;

        auto n_buffer = string::buffer(_sz); uint n = _sz; if( del != 0 )
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; erase(a); }
        
        return n_buffer;
    }

    string_t splice( int start, uint del, const string_t& argc ) noexcept { if(empty()) return *this;
        
        if( start < 0 ){ start = size() + start; } if( (uint)start > size() ){ start = size(); }
            del += start; if( del > size() ) del = del - size();
    
        uint a = min( first() + del, size() );
        uint b = first() + start, _sz = a - b;

        auto n_buffer = string::buffer(_sz); uint n = _sz; if( del != 0 )
        while( a-->b && n > 0 ){ n--; n_buffer[n] = buffer[a]; erase(a); }
        
        argc.map([&]( char x ){ insert( start, x ); start++; });
        return n_buffer;
    }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t to_lower_case() noexcept { if(empty()) return *this;
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = tolower( (int)(*x) );
        }   return (*this);
    }

    string_t to_upper_case() noexcept { if(empty()) return *this;
        for( auto x = this->begin(); x!=this->end(); x++ ){
            (*x) = toupper( (int)(*x) );
        }   return (*this);
    }

    string_t to_capital_case() noexcept { if(empty()) return *this;
        bool b=1; for( auto x = this->begin(); x!=this->end(); x++ ){
            if( string::is_lower(*x) && b==1 ){ (*x) = string::to_upper(*x); b=0; continue; }
            if(!string::is_alpha(*x) ){ b=1; }{ (*x) = string::to_lower(*x); }
        }   return (*this);
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    explicit operator char* (void) const {  
        if( !empty() ) return &buffer; return (char*) "\0";
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    const char* c_str() const noexcept { return (char*)(*this); }
    explicit operator bool(void) const { return empty(); }
    
};

/*────────────────────────────────────────────────────────────────────────────*/

string_t operator+( const string_t& A, const string_t& B ){
    string_t C = string::buffer( A.size() + B.size() ); uint n = 0;
    for( auto x : A ){ C[n] = x; n++; }
    for( auto x : B ){ C[n] = x; n++; } return C;
}

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