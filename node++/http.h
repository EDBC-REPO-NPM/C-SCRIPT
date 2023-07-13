#ifndef NODEPP_HTTP
#define NODEPP_HTTP

#include "algorithm.h"
#include "stream.h"
#include "url.h"
#include "tcp.h"
#include "map.h"

using header_t = map_t< string_t, string_t >;

namespace HTTP_NODEPP {

    string_t _get_http_status( int status ){
        switch( status ){
            case 100:  return "Continue";                                                break;
            case 101:  return "Switching Protocols";                                     break;
            case 102:  return "Processing";                                              break;
            case 103:  return "Early Hints";                                             break;
            case 200:  return "OK";                                                      break;
            case 201:  return "Created";                                                 break;
            case 202:  return "Accepted";                                                break;
            case 203:  return "Non-Authoritative Information";                           break;
            case 204:  return "No Content";                                              break;
            case 205:  return "Reset Content";                                           break;
            case 206:  return "Partial Content";                                         break;
            case 207:  return "Multi-Status";                                            break;
            case 208:  return "Already Reported";                                        break;
            case 226:  return "IM Used";                                                 break;
            case 300:  return "Multiple Choices";                                        break;
            case 301:  return "Moved Permanently";                                       break;
            case 302:  return "Found";                                                   break;
            case 303:  return "See Other";                                               break;
            case 304:  return "Not Modified";                                            break;
            case 305:  return "Use Proxy";                                               break;
            case 307:  return "Temporary Redirect";                                      break;
            case 308:  return "Permanent Redirect";                                      break;
            case 400:  return "Bad Request";                                             break;
            case 401:  return "Unauthorized";                                            break;
            case 402:  return "Payment Required";                                        break;
            case 403:  return "Forbidden";                                               break;
            case 404:  return "Not Found";                                               break;
            case 405:  return "Method Not Allowed";                                      break;
            case 406:  return "Not Acceptable";                                          break;
            case 407:  return "Proxy Authentication Required";                           break;
            case 408:  return "Request Timeout";                                         break;
            case 409:  return "Conflict";                                                break;
            case 410:  return "Gone";                                                    break;
            case 411:  return "Length Required";                                         break;
            case 412:  return "Precondition Failed";                                     break;
            case 413:  return "Payload Too Large";                                       break;
            case 414:  return "URI Too Long";                                            break;
            case 415:  return "Unsupported Media Type";                                  break;
            case 416:  return "Range Not Satisfiable";                                   break;
            case 417:  return "Expectation Failed";                                      break;
            case 418:  return "I'm a Teapot";                                            break;
            case 421:  return "Misdirected Request";                                     break;
            case 422:  return "Unprocessable Entity";                                    break;
            case 423:  return "Locked";                                                  break;
            case 424:  return "Failed Dependency";                                       break;
            case 425:  return "Too Early";                                               break;
            case 426:  return "Upgrade Required";                                        break;
            case 428:  return "Precondition Required";                                   break;
            case 429:  return "Too Many Requests";                                       break;
            case 431:  return "Request Header Fields Too Large";                         break;
            case 451:  return "Unavailable For Legal Reasons";                           break;
            case 500:  return "Internal Server Error";                                   break;
            case 501:  return "Not Implemented";                                         break;
            case 502:  return "Bad Gateway";                                             break;
            case 503:  return "Service Unavailable";                                     break;
            case 504:  return "Gateway Timeout";                                         break;
            case 505:  return "HTTP Version Not Supported";                              break;
            case 506:  return "Variant Also Negotiates";                                 break;
            case 507:  return "Insufficient Storage";                                    break;
            case 508:  return "Loop Detected";                                           break;
            case 509:  return "Bandwidth Limit Exceeded";                                break;
            case 510:  return "Not Extended";                                            break;
            case 511:  return "Network Authentication Required";                         break;
            default: throw err_t(regex::format("Status ${0} Not Found",status).c_str()); break;
        }
    }

    class http_base : public socket_t {
    protected:

        bool     has_header=0;
        string_t version;
    
    public:
    
        header_t headers;
        int      status;
        query_t  query;

        string_t protocol;
        string_t search;
        string_t method;
        string_t path;
        string_t url;

        int read_header(){
            try {
                string_t base = readline();
                if( !regex::test( base,"HTTP/1.+" ) ) return 0;

                regex_t _a(":"), _b("\\?.+"), _c("^[^?]+");
                array_t<string_t> init = regex::match_all(base,"[^\\s\t\r\n ]+");

                do { string_t line = readline();
                    auto index =_a.search( line );
                    if( index == nullptr ) break;
                    string_t a = line.slice( 0, index[0] );
                    string_t b = line.slice( index[1] +1 );
                    headers[a] = b;
                } while (1); protocol = "HTTP";

                if( !regex::test( init[1], "^\\d+" ) ) {
                    search = _b.match( init[1] );
                    path   = _c.match( init[1] );
                    version= init[2]; method = init[0];
                    query  = search_params::parse( search );
                    url    = regex::format( "http://${0}${1}${2}", headers["Host"], path, search );
                } else {
                    version= init[0]; status = string::to_int(init[1]);
                }          return 1;
            } catch(...) { return 0; }
        }

        void write_header( string_t method, string_t path, string_t version, header_t headers ){ 
            string_t res; res += regex::format("${0} ${1} ${2}\r\n",method,path,version);
            for( auto x:headers ){ res +=regex::format("${0}: ${1}\r\n",x.first,x.second); }
                                   res += "\r\n"; write( res );
        }

        void write_header( int status, header_t headers ){
            string_t res; res += regex::format("${0} ${1} ${2}\r\n",version,status,_get_http_status(status));
            for( auto x:headers ){ res += regex::format("${0}: ${1}\r\n",x.first,x.second); }
                                   res += "\r\n"; has_header = 1; write( res );
            if( method == "HEAD" ){ close(); }
        }

    };

}

class http_t : public HTTP_NODEPP::http_base {

    public: socket_t body;
            int s = 0;

    http_t( socket_t& inp, uint _size=CHUNK_SIZE ) { 
        body = inp; sid = inp.get_fd();
        set_buffer_size( _size );
    }

};

namespace http {
    tcp_t server( function_t<void,http_t> func ){ 
        return tcp_t([=]( socket_t cli ){ 
            http_t req = cli; if( req.read_header() ) func( req );
        }); 
    }

    tcp_t client( function_t<void,http_t> func ){ 
        return tcp_t([=]( socket_t cli ){ 
            http_t req = cli; func( req ); 
        }); 
    }
}

#endif