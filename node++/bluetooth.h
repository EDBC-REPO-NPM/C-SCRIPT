#ifndef NODEPP_BLUETOOTH
#define NODEPP_BLUETOOTH

#include "algorithm.h"
#include "promise.h"
#include "bsocket.h"

class bth_t {

    protected:
    function_t<void,bth_t> func;
    ptr_t<int> state = new int(0);

    /*────────────────────────────────────────────────────────────────────────────*/

    public:
    event_t<>         onClose;
    event_t<err_t>    onError;
    event_t<>         onLoad;
    event_t<bth_t> onOpen;

    /*────────────────────────────────────────────────────────────────────────────*/

    bth_t( function_t<void,bth_t> _func ){ func = _func; }

    void listen( string_t host, int port=1, agent_t* opt=nullptr ){
        bth_t *sk = new bth_t; 
               sk->PROT = BTPROTO_RFCOMM;
               sk->AF = AF_BLUETOOTH; 
                     
               sk->socket(host,port); 
               sk->set_sockopt( opt );
               sk->set_blocking_mode(1);

        bth_t self = *this;

        process::loop::add([=](){
            static bth_t*  cli = nullptr;
            
            event_t<>      Close = self.onClose;
            event_t<err_t> Error = self.onError;
            event_t<>      Load  = self.onLoad;
            event_t<bth_t> Open  = self.onOpen;
            ptr_t<int>     state = self.state;

            int _bind, _listen, _accept; $Start Load.emit();

            $Yield(1); _bind = sk->bind();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _bind == -1 && errno == EWOULDBLOCK ){ $Goto(1); }
            else if( _bind == -1 ){ Error.emit(err_t("Error while binding BTH")); $Goto(4); }

            $Yield(2); _listen = sk->listen();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _listen == -1 && errno == EWOULDBLOCK ){ $Goto(2); }
            else if( _listen == -1 ){ Error.emit(err_t("Error while listening BTH")); $Goto(4); }

            $Yield(3); _accept = sk->accept();
                 if( sk->is_closed() || *state == -1 ){ $Goto(4); }
            else if( _accept == -1 && errno == EWOULDBLOCK ){ $Goto(3); }
            else if( _accept == -1 ){ Error.emit(err_t("Error while accepting BTH")); $Goto(4); }
            else {
                process::poll::add([=]( tcp_t inp, socket_t cli ){
                    int ready = cli.r_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    inp.onOpen.emit(cli); inp.func(cli); return -1;
                },  inp, socket_t(_accept) ); $Goto(3);
            }

            $Yield(4); inp.onClose.emit(); delete sk; $Set(0);
            $End
        });
    }

    void connect( string_t host, int port=1, agent_t* opt=nullptr ){ 
        bth_t *sk = new bth_t; 
               sk->PROT = BTPROTO_RFCOMM;
               sk->AF = AF_BLUETOOTH; 
                     
               sk->socket(host,port); 
               sk->set_sockopt( opt );
               sk->set_blocking_mode(1);

        bth_t self = *this;

        process::loop::add([=](){
            event_t<>      Close = self.onClose;
            event_t<err_t> Error = self.onError;
            event_t<>      Load  = self.onLoad;
            event_t<bth_t> Open  = self.onOpen;
            ptr_t<int>     state = self.state;

            int _accept; $Start Load.emit();

            $Yield(1); _accept = sk->connect();
                 if( sk->is_closed() || *state == -1 ){ $Goto(2); }
            else if( _accept == -1 && errno == EWOULDBLOCK ){ $Goto(1); }
            else if( _accept == -1 ){ Error.emit(err_t("Error while connecting TCP")); }
            else {
                process::poll::add([=]( tcp_t inp ){
                    int ready = sk->w_ready();
                    if( ready == -2 ){ return  0; }
                    if( ready == -1 ){ return -1; }
                    inp.onOpen.emit(*sk); inp.func(*sk); 
                    inp.onClose.emit(); delete sk; return -1;
                },  inp );
            }

            $Yield(2); $Set(0);
            $End
        });
    }

};

namespace bluetooth {
    bth_t server( function_t<void,bth_t> func ){ return bth_t(func); }
    bth_t client( function_t<void,bth_t> func ){ return bth_t(func); }
}

#endif