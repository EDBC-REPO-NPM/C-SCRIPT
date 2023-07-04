#ifndef NODEPP_FETCH
#define NODEPP_FETCH

#include "ip.h"
#include "http.h"
#include "https.h"

class fetch_t { 
    
    public:

    header_t headers;
    string_t method = "GET";
    string_t version = "HTTP/1.0";

    ptr_t<ssl_t> ssl;
    ptr_t<agent_t> agent;
    ptr_t<stream_t> body;

    template< class T >
    fetch_t( string_t url, T cb ){ url_t uri = ::url::parse( url );
        string_t dir = uri.pathname + uri.search + uri.hash;
        fetch_t self = *this;

        if( regex::test(url,"^https","i") ){
            auto client = https::client([=]( https_t cli ){
                 
                cli.write_header( self.method, dir, self.version, self.headers );
                if( self.body!=nullptr ) while( !self.body->is_closed() )
                  { cli.write( self.body->read() ); }
                if( cli.read_header() > 0 ) cb ( cli );

            });
            
            client.connect( ip::lookup(uri.hostname), uri.port, &ssl, &agent );
        }

        else if( regex::test(url,"^http","i") ){

            auto client = http::client([=]( http_t cli ){

                cli.write_header( self.method, dir, self.version, self.headers );
                if( self.body!=nullptr ) while( !self.body->is_closed() )
                  { cli.write( self.body->read() ); }
                if( cli.read_header() > 0 ) cb ( cli );

            });
            
            client.connect( ip::lookup(uri.hostname), uri.port, &agent );
        }
        
    }

};

namespace fetch { }

#endif