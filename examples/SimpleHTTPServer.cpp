#include "node++/node++.h"
#include "node++/http.h"
#include "node++/date.h"

void server( int process ){

    auto server = http::server([=]( http_t cli ){ 

        if( cli.path.size() <= 1 ){
            cli.write_header(200,{{ "content-type", "text/html" }});
            cli.write( date::fulltime() ); return;
        }

        else if( regex::split( cli.path.slice(1), "/" )[0] == "output" ){
            cli.write_header(200,{{ "content-type", "text/html" }});
            cli.write("hola mundo"); return;
        }

        else if( regex::split( cli.path.slice(1), "/" )[0] == "input" ){
            cli.write_header(200,{{ "content-type", "text/html" }});
            cli.write("adios mundo"); return;
        }

        else {
            cli.write_header(404,{{ "content-type", "text/html" }});
            cli.write("path not found"); return;
        }

    });

    server.onLoad.on([](){
        console::log("server started at localhost:8000");
    });

    server.listen( "localhost", 8000 );

}

void $Ready() { server(0); }