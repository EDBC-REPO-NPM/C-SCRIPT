#include "node++/node++.h"
#include "node++/fetch.h"

int $Ready(){

    fetch_t("http://ip-api.com/csv/",[]( auto cli ){
        console::log("${0}",cli.headers["Host"]);
        cli.onData.on([]( string_t chunk ){
            console::log(chunk);
        });
        stream::pipe( cli );
    });

    return 0;
}