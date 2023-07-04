#ifndef NODEPP_HTTPS
#define NODEPP_HTTPS

#include "http.h"
#include "tls.h"

class https_t : public HTTP_NODEPP::http_base {

    public: ssocket_t body;
            int s = 0;

    https_t( ssocket_t& inp, uint _size=CHUNK_SIZE ){ 
        body = inp; sid = inp.get_fd();
        set_buffer_size( _size );
    }

};

namespace https {
    tls_t server( function_t<void,https_t> func ){ 
        return tls_t([=]( ssocket_t cli ){
               https_t req = cli; if( req.read_header() ) func( req );
        }); 
    }

    tls_t client( function_t<void,https_t> func ){ 
        return tls_t([=]( ssocket_t cli ){ 
               https_t req = cli; func( req ); 
        }); 
    }
}

#endif