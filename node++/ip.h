#ifndef NODEPP_IP
#define NODEPP_IP

#include "algorithm.h"
#include "stream.h"
#include "url.h"

namespace ip { 

    string_t ipv6 = "([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4}"; 
    string_t ipv4 = "([0-9]{1,3}[.]){3}[0-9]{1,3}";

    string_t lookup( string_t host ){ 
             if( host == "255.255.255.255" || host == "broadcast" ){ return string_t("255.255.255.255"); }
        else if( host == "127.0.0.1" || host == "localhost" )      { return string_t("127.0.0.1"); }
        else if( host == "0.0.0.0" || host == "globalhost" )       { return string_t("0.0.0.0"); }

        if( url::is_valid(host) ){ host = url::hostname(host); }
        string_t data; exec_t p("ping -c 1 " + host);
        while( !p.is_closed() ){ data+=p.read(); }
        
        return regex::match( data, ipv4+"|"+ipv6 );
    }

    int is_ipv4( string_t URL ){ return regex::test( URL, ipv4 ) ? 1 : -1; }

    int is_ipv6( string_t URL ){ return regex::test( URL, ipv6 ) ? 1 : -1; }

    int is_ip( string_t URL ){ 
        if( is_ipv4(URL) > 0 ){ return  1; }
        if( is_ipv6(URL) > 0 ){ return  1; } return -1;
    }

}

#endif