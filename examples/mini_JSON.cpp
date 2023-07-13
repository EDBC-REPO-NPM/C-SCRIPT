
#include "node++/node++.h"
#include "node++/var.h"
#include "node++/map.h"

void func( map_t<string_t,var_t> obj ){ 
    console::log( "int: ", (int)obj["var"] );
    console::log( "dou: ", (float)obj["par"] );
    console::log( "str: ", (string_t)obj["mar"] );
}

void $Ready() { var_t ppt;
    
    func({
        { "var", 10 },
        { "par", 10.50f },
        { "mar", "hola mundo" },
    });

}