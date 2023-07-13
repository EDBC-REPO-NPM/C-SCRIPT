#include "node++/node++.h"
#include "node++/fetch.h"
#include "node++/timer.h"

void $Ready(){

    fetch_t("http://ip-api.com/csv/",[]( auto cli ){
        console::log( cli.headers["Host"] );

        cli.onData.on([]( string_t chunk ){
            console::log( chunk );
        });

        stream::pipe( cli );
    });

}