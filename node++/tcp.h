#ifndef NODEPP_TCP
#define NODEPP_TCP

#include "algorithm.h"
#include "socket.h"

class tcp_t {

    protected:
    function_t<void,socket_t> func;
    ptr_t<int> state = new int(0);

    /*────────────────────────────────────────────────────────────────────────────*/

    public:
    event_t<>         onClose;
    event_t<err_t>    onError;
    event_t<>         onLoad;
    event_t<socket_t> onOpen;

    /*────────────────────────────────────────────────────────────────────────────*/

    tcp_t( function_t<void,socket_t> _func ){ func = _func; }

    void listen( string_t host, int port, agent_t* opt=nullptr ){
        socket_t *sk = new socket_t; 
                  sk->socket(host,port); 
                  sk->set_sockopt( opt );
                  sk->set_blocking_mode(1);

        process::loop::add([=]( tcp_t inp ){
            int _bind, _listen, _accept; $Start inp.onLoad.emit();

            $Yield(1); _bind = sk->bind();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _bind == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(1); }
            else if( _bind == -1 ){ inp.onError.emit(err_t("Error while binding TCP")); $Goto(4); }

            $Yield(2); _listen = sk->listen();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _listen == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(2); }
            else if( _listen == -1 ){ inp.onError.emit(err_t("Error while listening TCP")); $Goto(4); }

            $Yield(3); _accept = sk->accept();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _accept == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(3); }
            else if( _accept == -1 ){ inp.onError.emit(err_t("Error while accepting TCP")); $Goto(4); }
            else {
                process::poll::add([=]( tcp_t inp, socket_t cli ){
                    int ready = cli.r_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    inp.onOpen.emit(cli); inp.func(cli); return -1;
                },  inp, socket_t(_accept) ); $Goto(3);
            }

            $Yield(4); inp.onClose.emit(); delete sk; $Set(0); $End
        }, *this );
    }

    void close(){ if( state != nullptr )(*state) = -1; }

    void connect( string_t host, int port, agent_t* opt=nullptr ){
        socket_t *sk = new socket_t; 
                  sk->socket(host,port); 
                  sk->set_sockopt( opt );
                  sk->set_blocking_mode(1);

        process::loop::add([=]( tcp_t inp ){
            int _connect; $Start inp.onLoad.emit();

            $Yield(1); _connect = sk->connect();
                 if( sk->is_closed() || *state == -1 ){ $Goto(2); }
            else if( _connect == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(1); }
            else if( _connect == -1 ){ inp.onError.emit(err_t("Error while connecting TCP")); $Goto(2); }
            else {
                process::poll::add([=]( tcp_t inp ){
                    int ready = sk->w_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    inp.onOpen.emit(*sk); inp.func(*sk); 
                    inp.onClose.emit(); delete sk; return -1;
                },  inp );
            }

            $Yield(2); $Set(0); $End
        }, *this );
    }

};

namespace tcp {
    tcp_t server( function_t<void,socket_t> func ){ return tcp_t(func); }
    tcp_t client( function_t<void,socket_t> func ){ return tcp_t(func); }
}

#endif