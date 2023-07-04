#ifndef NODEPP_BSOCKET
#define NODEPP_BSOCKET

#include "algorithm.h"
#include "socket.h"

#ifdef __WIN32
    #include <ws2bth.h>
    #include <BluetoothAPIs.h>
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "Bthprops.lib")
#else 
    #include <bluetooth/rfcomm.h>
    #include <bluetooth/bluetooth.h>
#endif

class bsocket_t : public socket_t { public:

    int socket( string_t host, int port ){ addrlen = sizeof(server_addr);

        if(( sid = ::socket( AF, SOCK, PROT ) ) <= 0 ) 
            return -1; 

        struct sockaddr_rc server={0}, client={0};

        addr.rc_family           = AF;
        addr.rc_channel          = (uint8_t) port;

        str2ba( host.c_str(), &addr.rc_bdaddr );

        len = sizeof( client );
        server_addr = *( (struct sockaddr*) &server );
        client_addr = *( (struct sockaddr*) &client );
        
    }

};
#endif