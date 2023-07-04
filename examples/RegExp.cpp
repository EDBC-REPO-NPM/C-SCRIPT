#include "node++/node++.h"

int $Ready() {

    regex_t reg("[0-9]+","i");
    console::log( "hola 123: ", reg.test("hola 123") ? "true" : "false" );
    console::log( "hola 123: ", reg.match("hola 123") );

    console::log( " -- 123 456 hola 789 -- " );
    for( auto x:reg.match_all("123 456 hola 789") )
        console::log(" - ${0}", x);

    console::log(" -- hola 123 -- ");
    string_t nst = reg.replace_all("hola 123","mundo");
    console::log(nst);

    return 0; 
}