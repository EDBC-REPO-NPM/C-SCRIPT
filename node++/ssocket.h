#ifndef NODEPP_SSCOKET
#define NODEPP_SSCOKET

#include "assets/openssl/ssl.h"
#include "assets/openssl/err.h"

#include "algorithm.h"
#include "socket.h"
#include "fs.h"

class ssl_t { protected:

    using onSNI = function_t<ssl_t*,string_t>;
    ptr_t<onSNI> func;

    SSL_CTX* create_server_context(){
        const SSL_METHOD *method; method = TLS_server_method();
        SSL_CTX* ctx = SSL_CTX_new(method); return ctx;
    }

    SSL_CTX* create_client_context(){
        const SSL_METHOD *method; method = TLS_client_method();
        SSL_CTX* ctx = SSL_CTX_new(method); return ctx;
    }

    int configure_context( SSL_CTX* ctx, string_t cert, string_t key ){
        return (  
            SSL_CTX_use_certificate_file( ctx, cert.c_str(), SSL_FILETYPE_PEM )<0 ||
            SSL_CTX_use_PrivateKey_file( ctx, key.c_str(), SSL_FILETYPE_PEM )<0 
        ) ? -1 : 1;
    }

    static int servername_handler( SSL* ssl, int* ao, void* arg ){
        const char* servername = SSL_get_servername( ssl, TLSEXT_NAMETYPE_host_name );
        onSNI func = *((onSNI*)arg); if( servername ){ ssl_t* ctx = func(servername); 
            if( ctx != nullptr ){ ctx->create_server();
                SSL_CTX* v = SSL_set_SSL_CTX( ssl, ctx->ctx );
                if( v != ctx->ctx ) return SSL_TLSEXT_ERR_NOACK;
            }
        }   return SSL_TLSEXT_ERR_OK;
    }

    void set_ctx_sni( SSL_CTX* ctx, onSNI* func ){
        SSL_CTX_set_tlsext_servername_callback( ctx, servername_handler );
        SSL_CTX_set_tlsext_servername_arg( ctx, func );
    }

public: 

    int srv = 0;
    SSL* ssl = nullptr;
    SSL_CTX* ctx = nullptr; 
    string_t key; string_t cert;

   ~ssl_t(){
        if( ssl != nullptr ){ 
            if( srv==1 ){ SSL_shutdown(ssl); }
                          SSL_free(ssl); 
        }
        if( ctx != nullptr ){ SSL_CTX_free(ctx); } 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    ssl_t( const ssl_t& o) = delete;
    ssl_t& operator=( const ssl_t& o ) = delete;

    ssl_t( const ssl_t&& o) = delete;
    ssl_t& operator=( const ssl_t&& o ) = delete;

    /*────────────────────────────────────────────────────────────────────────────*/

    ssl_t( string_t _key, string_t _cert, onSNI* _func=nullptr ){
        if( fs::exists_file(_key)<0 || fs::exists_file(_cert)<0 )
            throw err_t("such key or cert does not exist");
        if( _func != nullptr ) func = _func;
            key = _key; cert = _cert; 
    }

    ssl_t( ssl_t* ctx, int fd ){ 
        if( ctx == nullptr ) throw err_t("ctx is a nullptr");
        ssl = SSL_new(ctx->ctx); SSL_set_fd( ssl, fd );
    }

    int create_server(){
        ctx = create_server_context();
        int res = configure_context( ctx, cert, key ); 
        if( func != nullptr ) set_ctx_sni( ctx, &func );
        return res;
    }

    int create_client(){
        ctx = create_client_context();
        return configure_context( ctx, cert, key );
    }

    int accept(){ srv=1; 
        int sid = SSL_accept( ssl );
        int err = SSL_get_error( ssl, sid ); if( sid <= 0 ){
            if( err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE ) return -2;
            else return -1;
        }   return sid;
    }

    int connect(){ srv=0; 
        int sid = SSL_connect( ssl );
        int err = SSL_get_error( ssl, sid ); if( sid <= 0 ){
            if( err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE ) return -2;
            else return -1;
        }   return sid; 
    }

    int read( char* buffer, uint size ){
        if( ssl == nullptr ) throw err_t("ssl is a nullptr");
        return SSL_read( ssl, buffer, size );
    }
    
    int write( char* buffer, uint size ){
        if( ssl == nullptr ) throw err_t("ssl is a nullptr");
        return SSL_write( ssl, buffer, size );
    }

};

class ssocket_t : public socket_t { public:

    ptr_t<ssl_t> ssl;

    ssocket_t( int _sid=0 ) : socket_t( _sid ) { }

    string_t read(){
        string_t s; if( is_closed() ) 
        { (*state)=-1; return s; } (*state) = 1; int c = 0;

        if(!borrow->empty() ){ s = (*borrow); borrow->clear(); return s; }
        
        if( SOCK == SOCK_STREAM ) { 
             if( (c=ssl->read( &buffer, *chunk ))<=0 ) (*state) = -1;
        } else { throw err_t("UDP NOT SUPORTED"); } s = string_t( &buffer, c );

        if( (*state)>0 ){ (*state) = 0; } return s;
    }

    int write( string_t msg ){ if( is_closed() || msg.empty() ) 
        { (*state)=-1; return -1; } (*state) = 1; int c=0;
        
        if( SOCK == SOCK_STREAM ){
             if( (c=ssl->write( (char*)msg.c_str(), msg.size() ))<=0 ) (*state) = -1;
        } else { throw err_t("UDP NOT SUPORTED"); }

        if( (*state)>0 ){ (*state) = 0; } return c;
    }
    
};

#endif