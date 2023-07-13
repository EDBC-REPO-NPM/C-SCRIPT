#include "node++/node++.h"
#include "node++/timer.h"
#include "node++/http.h"
#include "node++/path.h"
#include "node++/date.h"
#include "node++/fs.h"

void server( int process ){

    auto server = http::server([=]( http_t cli ){ 

        string_t dir = "www/index.html";
        if( cli.path.size() > 1 )
            dir = path::join( "www", cli.path );

        console::log( cli.path, process );

        if( fs::exists_file(dir)<0 ){
            cli.write_header( 404,{{ "content-type", "text/plain" }} );
            cli.end( regex::format("404: Oops time: ${0}",date::fulltime()) ); return;
        }

        auto str = fs::readable( dir );

        if( cli.headers["Range"].empty() ){

            cli.write_header( 200, {
                { "Content-Length", string::to_string(str.size()) },
            //  { "Cache-Control", "public, max-age=3600" },
                { "Content-Type",   path::mimetype(dir) }
            });

            if(!regex::test(path::mimetype(dir),"audio|video","i") ) 
                stream::pipe( str, cli );

        } else if( !cli.headers["Range"].empty() ) {

            array_t<string_t> range = regex::match_all(cli.headers["Range"],"\\d+","i");
            uint rang[2]; rang[0] = string::to_int( range[0] );
                 rang[1] = math::min( rang[0]+(1024*1024*10), str.size()-1 );

            cli.write_header( 206, {
                { "Content-Range", regex::format("bytes ${0}-${1}/${2}",rang[0],rang[1],str.size()) },
                { "Content-Type",  path::mimetype(dir) }, 
                { "Accept-Range", "bytes" }
            });
            
            str.range[0] = rang[0]; str.range[1] = rang[1];
            stream::pipe( str, cli );

        }

    });

    server.onLoad.on([](){
        console::log("server started at localhost:8000");
    });

    server.listen( "localhost", 8000 );

}

void $Ready() { server(0); }
