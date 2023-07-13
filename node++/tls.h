#ifndef NODEPP_TLS
#define NODEPP_TLS

#include "algorithm.h"
#include "ssocket.h"

class tls_t { 

    protected:
    function_t<void,ssocket_t> func;
    ptr_t<int> state = new int(0);

    /*────────────────────────────────────────────────────────────────────────────*/

    public:
    event_t<>          onClose;
    event_t<err_t>     onError;
    event_t<>          onLoad;
    event_t<ssocket_t> onOpen;
    
    /*────────────────────────────────────────────────────────────────────────────*/
    tls_t( function_t<void,ssocket_t> _func ){ func = _func; }

    void listen( string_t host, int port, ssl_t* ctx, agent_t* opt=nullptr ){
        ssocket_t *sk = new ssocket_t; 
                   sk->socket(host,port); 
                   sk->set_sockopt( opt );
                   sk->set_blocking_mode(1);

        if( ctx == nullptr )           onError.emit(err_t("Error Initializing SSL context"));
        if( ctx->create_server() < 0 ) onError.emit(err_t("Error Initializing SSL Client"));

        process::loop::add([=]( tls_t inp ){
            int _bind, _listen, _accept; $Start inp.onLoad.emit();

            $Yield(1); _bind = sk->bind();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _bind == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(1); }
            else if( _bind == -1 ){ inp.onError.emit(err_t("Error while binding TLS")); $Goto(4); }

            $Yield(2); _listen = sk->listen();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _listen == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(2); }
            else if( _listen == -1 ){ inp.onError.emit(err_t("Error while listening TLS")); $Goto(4); }

            $Yield(3); _accept = sk->accept();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _accept == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(3); }
            else if( _accept == -1 ){ inp.onError.emit(err_t("Error while accepting TLS")); $Goto(4); }
            else { 
                process::loop::add([=]( tls_t inp, ssocket_t cli ){
                    int ready = cli.r_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    cli.ssl->accept(); 
                    inp.onOpen.emit(cli); inp.func(cli); 
                    inp.onClose.emit(); return -1;
                },  inp, ssocket_t(_accept) ); $Goto(3);
            }

            $Yield(4); delete sk; inp.onClose.emit(); $Yield(5); $Set(0);
            $End
        }, *this );
    }

    void close(){ if( state != nullptr )(*state) = -1; }

    void connect( string_t host, int port, ssl_t* ctx, agent_t* opt=nullptr ){
        ssocket_t *sk = new ssocket_t; 
                   sk->socket(host,port); 
                   sk->set_sockopt( opt );
                   sk->set_blocking_mode(1);

        if( ctx == nullptr )           onError.emit(err_t("Error Initializing SSL context"));
        if( ctx->create_server() < 0 ) onError.emit(err_t("Error Initializing SSL Client"));

        process::loop::add([=]( tls_t inp ){
            int _accept; $Start inp.onLoad.emit();

            $Yield(1); _accept = sk->connect();
                 if( sk->is_closed() || *state == -1 ){ $Goto(3); }
            else if( _accept == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(1); }
            else if( _accept == -1 ){ inp.onError.emit(err_t("can't connect to server")); $Goto(2); }
            else { sk->ssl = new ssl_t( ctx, sk->get_fd() );
                process::loop::add([=]( tls_t inp ){
                    int ready = sk->r_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    sk->ssl->accept(); 
                    inp.onOpen.emit(*sk); inp.func(*sk); 
                    inp.onClose.emit(); delete sk; return -1;
                },  inp );
            }

            $Yield(3); $Set(0);
            $End
        }, *this );
    }

};

namespace tls {
    tls_t server( function_t<void,ssocket_t> func ){ return tls_t(func); }
    tls_t client( function_t<void,ssocket_t> func ){ return tls_t(func); }
}

#endif