#ifndef NODEPP_UDP
#define NODEPP_UDP

#include "algorithm.h"
#include "socket.h"

class udp_t { 
    
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
    udp_t( function_t<void,socket_t> _func ){ func = _func; }

    void listen( std::string host, int port, agent_t* opt ){
        socket_t *sk = new socket_t; 
                  sk->SOCK = SOCK_DGRAM;
                  sk->socket(host,port); 
                  sk->set_sockopt( opt );
                  sk->set_blocking_mode(1);
                  
        process::loop::add([=]( udp_t inp ){
            int _bind; $Start inp.onLoad.emit();

            $Yield(1); _bind = sk->bind();
                 if( sk->is_closed() || *state == -1 ){ $Goto(2); }
            else if( _bind == -1 && SOCKET_NODEPP::_blocked_() ){ $Goto(1); }
            else if( _bind == -1 ){ inp.onError.emit(err_t("Error while binding UDP")); }
            else { 
                process::loop::add([=]( udp_t inp ){
                    int ready = sk->r_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    inp.onOpen.emit(*sk); inp.func(*sk); 
                    inp.onClose.emit(); delete sk; return -1;
                },  inp );
            }

            $Yield(2); $Set(0); $End
        }, *this );
    }

    void close(){ if( state != nullptr )(*state) = -1; }

    void connect( std::string host, int port, agent_t* opt ){
        socket_t *sk = new socket_t; 
                  sk->SOCK = SOCK_DGRAM;
                  sk->socket(host,port); 
                  sk->set_sockopt( opt );
                  sk->set_blocking_mode(1);

        process::loop::add([=]( udp_t inp ){
            int ready = sk->w_ready();
            if( ready == -2 ){ return  0; }
            if( ready == -1 ){ return -1; }
            inp.onOpen.emit(*sk); inp.func(*sk);
            inp.onClose.emit(); delete sk; return -1;
        }, *this );
    }

};

namespace udp {
    udp_t server( function_t<void,socket_t> func ){ return udp_t(func); }
    udp_t client( function_t<void,socket_t> func ){ return udp_t(func); }
}

#endif