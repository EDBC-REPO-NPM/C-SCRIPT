#ifndef NODEPP_VARIYING
#define NODEPP_VARIYING

#include "string.h"
#include "ptr.h"

class var_t {
public:

   ~var_t() {} 
   
    var_t() noexcept = default;
    
    var_t( const char* f ) : var_ptr( new var_impl<string_t>(string::to_string(f)) ) {}

    template< class T >
    T get() const noexcept { T var; var_ptr->get((void*)&var); return var; }

    void operator=( const char* f ){ set( string::to_string(f) ); }

    template< class T >
    explicit operator T(void) const { return get<T>(); }
    
    template< class T >
    var_t( T f ) : var_ptr( new var_impl<T>(f) ) { }

    template< class T >
    void set( T f ){ var_ptr = new var_impl<T>(f); }

    template< class T >
    void operator=( const T& f ){ set( f ); }
    
private:
    class var_base {
    public:
        virtual ~var_base() {}
        virtual void get( void* argc ){}
        virtual void set( void* argc ){}
    };
    
    template< class T >
    class var_impl : public var_base {
    public:
        var_impl( T f ) : var( f ) {}
        virtual void get( void* argc ){ *((T*)argc) = var; }
        virtual void set( void* argc ){ var = *((T*)argc); }
    private:
        T var;
    };
    
    ptr_t<var_base> var_ptr;
};

#endif