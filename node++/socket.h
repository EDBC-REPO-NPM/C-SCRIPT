#ifndef NODEPP_SOCKET
#define NODEPP_SOCKET

#include "algorithm.h"
#include "stream.h"

#ifdef __WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <netdb.h>
#endif

struct agent_t {
    bool broadcast     = 0;
    bool keep_alive    = 0;
    bool reuse_port    = 0;
    uint buffer_size   = CHUNK_SIZE;
    uint send_timeout  = 0;
    uint recv_timeout  = 0;
    bool reuse_address = 0;
};

namespace {
namespace SOCKET_NODEPP {
#ifdef __WIN32

    template< class... T > string_t _inet_ntop_( T... args ){ return inet_ntop( args... ); }

    template< class... T > int _inet_pton_( T... args ){ return inet_pton(args...); }

    void _init_(){ WSADATA wsa_data; WSAStartup( MAKEWORD(2,2), &wsa_data ); }

    void _close_( int fd, int srv ){ closesocket( fd ); WSACleanup(); }

    bool _blocked_(){ 
        return ( 
            WSAGetLastError() == WSAEWOULDBLOCK || 
            WSAGetLastError() == WSAEINPROGRESS ||
            WSAGetLastError() == WSAECONNRESET  ||
            WSAGetLastError() == WSAEALREADY    ||
            WSAGetLastError() == WSAEAGAIN
        ); 
    }

#else

    template< class... T > string_t _inet_ntop_( T... args ){ return inet_ntop( args... ); }

    template< class... T > int _inet_pton_( T... args ){ return inet_pton(args...); }

    void _close_( int fd, int srv ){
        if( srv == 1 ){ ::shutdown( fd, SHUT_RDWR ); }
                        ::close( fd );  
    }

    void _init_(){  }

    bool _blocked_(){ 
        return ( 
            errno == EWOULDBLOCK || 
            errno == EINPROGRESS ||
            errno == ECONNRESET  ||
            errno == EALREADY    ||
            errno == EAGAIN
        ); 
    }

#endif
}
}

class socket_t : public _core_t {

    protected:

    struct sockaddr server_addr, client_addr;
    socklen_t addrlen; bool srv=0; socklen_t len;

    public:

    int SOCK  = SOCK_STREAM;
    int AF    = AF_INET; 
    int PROT  = 0;

    /*────────────────────────────────────────────────────────────────────────────*/

    int set_recv_buff( uint en ){
        return setsockopt( sid, SOL_SOCKET, SO_RCVBUF, (char*)&en, sizeof(en) );
    }

    int set_send_buff( uint en ){
        return setsockopt( sid, SOL_SOCKET, SO_SNDBUF, (char*)&en, sizeof(en) );
    }

    int set_recv_timeout( long int time ){ struct timeval en = {.tv_sec=time}; 
        return setsockopt( sid, SOL_SOCKET, SO_RCVTIMEO, (char*)&en, sizeof(en) );
    }

    int set_send_timeout( long int time ){ struct timeval en = {.tv_sec=time}; 
        return setsockopt( sid, SOL_SOCKET, SO_SNDTIMEO, (char*)&en, sizeof(en) );
    }

    int set_keep_alive( uint en ){ 
        return setsockopt( sid, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, sizeof(en) );
    }

    int set_broadcast( uint en ){ 
        return setsockopt( sid, SOL_SOCKET, SO_BROADCAST, (char*)&en, sizeof(en) );
    }

    int set_reuse_address( uint en ){ 
        return setsockopt( sid, SOL_SOCKET, SO_REUSEADDR, (char*)&en, sizeof(en) );
    }

    int set_reuse_port( uint en ){ 
        return setsockopt( sid, SOL_SOCKET, SO_REUSEPORT, (char*)&en, sizeof(en) );
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    int set_blocking_mode( int mode ){ int fd = get_fd();
    #ifdef __WIN32
        return ioctlsocket( fd, FIONBIO, (u_long)&mode ); 
    #else 
        static int flags = fcntl( fd, F_GETFL, 0 );
        if( mode == 1 )
             return fcntl( fd, F_SETFL, flags | O_NONBLOCK );
        else return fcntl( fd, F_SETFL, flags );
    #endif
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    long int get_recv_timeout(){ struct timeval en; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_RCVTIMEO, (char*)&en, &size)<0) ? -1 : en.tv_sec;
    }

    long int get_send_timeout(){ struct timeval en; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_SNDTIMEO, (char*)&en, &size)<0) ? -1 : en.tv_sec;
    }

    int get_reuse_address(){ int en = 0; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_REUSEADDR , (char*)&en, &size)<0) ? -1 : en;
    }

    int get_reuse_port(){ int en = 0; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_REUSEPORT , (char*)&en, &size)<0) ? -1 : en;
    }

    int get_keep_alive(){ int en = 0; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_KEEPALIVE, (char*)&en, &size)<0) ? -1 : en;
    }

    int get_broadcast(){ int en = 0; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_BROADCAST, (char*)&en, &size)<0) ? -1 : en;
    }

    int get_error(){ int en; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_ERROR, (char*)&en, &size ) < 0) ? -1 : en;
    }

    int get_recv_buff(){ int en; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_RCVBUF, (char*)&en, &size)<0) ? -1 : en;
    }

    int get_send_buff(){ int en; socklen_t size = sizeof(en);
        return (getsockopt(sid, SOL_SOCKET, SO_SNDBUF, (char*)&en, &size)<0) ? -1 : en;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

//  string_t get_ip(){}

    /*────────────────────────────────────────────────────────────────────────────*/

    bool is_feof(){ return get_error() != 0; }

    /*────────────────────────────────────────────────────────────────────────────*/

    int set_sockopt( agent_t* opt ){ if( !opt ) return -1;
        set_reuse_address( opt->reuse_address );
        set_recv_timeout( opt->recv_timeout );
        set_send_timeout( opt->send_timeout );
        set_buffer_size( opt->buffer_size );
        set_reuse_port( opt->reuse_port );
        set_keep_alive( opt->keep_alive );
        set_broadcast( opt->broadcast );
        return 1;
    }

    agent_t* get_sockopt( ){ agent_t* opt;
        opt->reuse_address = get_reuse_address();
        opt->recv_timeout  = get_recv_timeout();
        opt->send_timeout  = get_send_timeout();
        opt->buffer_size   = get_buffer_size();
        opt->reuse_port    = get_reuse_port();
        opt->keep_alive    = get_keep_alive();
        opt->broadcast     = get_broadcast();
        return opt;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    socket_t( int _sid=0, uint _size=CHUNK_SIZE ){
        SOCKET_NODEPP::_init_(); sid = _sid; set_buffer_size( _size );
    }

   ~socket_t(){ if( d ) return; if( state.count() == 1 ){
        SOCKET_NODEPP::_close_( sid, srv ); (*state) =-1;
    }   d = 1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    virtual int socket( string_t host, int port ){ 
        SOCKET_NODEPP::_init_(); addrlen = sizeof( server_addr );

        if((sid=::socket( AF, SOCK, PROT ))<=0) return -1;
                  set_reuse_address(1); set_reuse_port(1);

        struct sockaddr_in server={0}, client={0};
        server.sin_port          = htons(port);
        server.sin_family        = AF;

             if( host == "0.0.0.0" || host == "globalhost" )       { server.sin_addr.s_addr = INADDR_ANY; }
        else if( host == "255.255.255.255" || host == "broadcast" ){ server.sin_addr.s_addr = INADDR_BROADCAST; } 
        else if( host == "127.0.0.1" || host == "localhost" )      { SOCKET_NODEPP::_inet_pton_(AF_INET, "127.0.0.1", &server.sin_addr); }
        else                                                       { SOCKET_NODEPP::_inet_pton_(AF_INET, host.c_str(), &server.sin_addr); }

        server_addr = *((struct sockaddr*) &server);
        client_addr = *((struct sockaddr*) &client);
        len = sizeof( server ); return 1;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    int connect(){ if( srv == 1 ) return -1; return ::connect( sid, &server_addr, addrlen ); }
    int accept(){ if( srv == 0 ) return -1; return ::accept( sid, &server_addr, &addrlen ); }
    int listen( int i=0 ){ if( srv == 0 ) return -1; return ::listen( sid, i ); }
    int bind(){ srv = 1; return ::bind( sid, &server_addr, addrlen ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t read(){ string_t s; if( is_closed() ) 
        { (*state)=-1; return s; } int c=0; (*state)=1;

        if(!borrow->empty() ){ s = (*borrow); borrow->clear(); return s; }

        if( SOCK == SOCK_STREAM ) { 
            if( (c=::recv( sid, &buffer, *chunk, 0 ))<=0 ) 
              (*state)=-1; else s = string_t( &buffer, c );
        } else { struct sockaddr* cli; if( srv == 1 ) cli = &client_addr; else cli = &server_addr;
            if( (c=::recvfrom( sid, &buffer, *chunk, MSG_WAITALL, cli, &len ))<=0 ) 
              (*state)=-1; else s = string_t( &buffer, c );
        }

        if( (*state) > 0 ){ (*state) = 0; } return s;
    }

    int write( string_t msg ){ if( is_closed() || msg.empty() )
        { (*state)=-1; return -1; } int c=0; (*state)=1; 

        if( SOCK == SOCK_STREAM ){
           if( (c=::send( sid, (char*)msg.c_str(), msg.size(), 0 ))<=0 ) (*state)=-1;
        } else { struct sockaddr* cli; if( srv==1 ) cli = &client_addr; else cli = &server_addr;
           if( (c=::sendto( sid, (char*)msg.c_str(), msg.size(), MSG_CONFIRM, cli, len ))<=0 ) (*state)=-1;
        }

        if( (*state) > 0 ){ (*state) = 0; } return c;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    int w_ready(){ int c=0, s=0;
        set_blocking_mode(1); 
        char buffer[1] = {0};

        if( SOCK == SOCK_STREAM ) { 
            c=::send( sid, buffer, 0, 0 );
        } else {
            struct sockaddr* cli; if( srv == 1 ) cli = &client_addr; else cli = &server_addr;
            c=::sendto( sid, buffer, 0, MSG_CONFIRM, cli, len );
        }

        if( c == -1 && SOCKET_NODEPP::_blocked_() ) s = -2; 
        else if( c == -1 ) s = -1; else s = 1;

        set_blocking_mode(0); return s;
    }
    
    int r_ready(){ int c=0, s=0;
        set_blocking_mode(1); 
        char buffer[1] = {0};

        if( SOCK == SOCK_STREAM ) { 
            c=::recv( sid, buffer, 0, 0 );
        } else {
            struct sockaddr* cli; if( srv == 1 ) cli = &client_addr; else cli = &server_addr;
            c=::recvfrom( sid, buffer, 0, MSG_WAITALL, cli, &len );
        }

        if( c == -1 && SOCKET_NODEPP::_blocked_() ) s = -2; 
        else if( c == -1 ) s = -1; else s = 1;

        set_blocking_mode(0); return s;
    }

    /*────────────────────────────────────────────────────────────────────────────*/
    
};

#endif