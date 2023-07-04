#pragma once

#include <initializer_list>
#include "algorithm.h"
#include "ptr.h"

namespace _JSON {

    class NODE { 
        private:

            using object = map_t< const char*, NODE >;
            using array  = std::vector<NODE>;

            enum TYPE { OBJECT, ARRAY, CCHAR, FLOAT, CHAR, BOOL, INT, NULLPTR } type;
            union MEMORY {
                object*   OBJECT;
                array*    ARRAY;
                char*     CCHAR;
                float     FLOAT;
                char      CHAR;
                bool      BOOL;
                int       INT;
            }   memory;

            void set_cchar( const char* arg ){ type = TYPE::CCHAR; memory.CCHAR = (char*)arg; }
            void set_object( object* arg ){ type = TYPE::OBJECT; memory.OBJECT = arg; }
            void set_array( array* arg ){ type = TYPE::ARRAY; memory.ARRAY = arg; }
            void set_float( float arg ){ type = TYPE::FLOAT; memory.FLOAT = arg; }
            void set_char( char arg ){ type = TYPE::CHAR; memory.CHAR = arg; }
            void set_bool( bool arg ){ type = TYPE::BOOL; memory.BOOL = arg; }
            void set_int( int arg ){ type = TYPE::INT; memory.INT = arg; }

            void free(){  }

        public:

            NODE( const char* arg ){ set_cchar( arg ); }
            NODE( object arg ){ set_object( &arg ); }
            NODE( double arg ){ set_float( arg ); }
            NODE( array arg ){ set_array( &arg ); }
            NODE( float arg ){ set_float( arg ); }
            NODE( char arg ){ set_char( arg ); }
            NODE( bool arg ){ set_bool( arg ); }
            NODE( int arg ){ set_int( arg ); }
            NODE(){ type = TYPE::NULLPTR; }

        //  NODE( const NODE&& ) = delete;
        //  NODE( const NODE& ) = delete;

           ~NODE(){ free(); }

        /*────────────────────────────────────────────────────────────────────────────*/

            operator object(void){
                if( type == TYPE::OBJECT ) return (*memory.OBJECT);
                else                     { object obj; return obj; }
            }

            operator array(void){
                if( type == TYPE::ARRAY ) return (*memory.ARRAY);
                else                    { array obj; return obj; }
            }

            operator float(void){
                if( type == TYPE::FLOAT ) return memory.FLOAT;
                else                      return .0;
            }

            operator char*(void){
                if( type == TYPE::CCHAR ) return memory.CCHAR;
                else                      return (char*)"\0";
            }

            operator char(void){
                if( type == TYPE::CHAR ) return memory.CHAR;
                else                     return '\0';
            }

            operator bool(void){
                if( type == TYPE::BOOL ) return memory.BOOL;
                else                     return false;
            } 

            operator int(void){ 
                if( type == TYPE::INT ) return memory.INT;
                else                    return 0; 
            } 

        /*────────────────────────────────────────────────────────────────────────────*/

            int get_type(){ return type; }

        /*────────────────────────────────────────────────────────────────────────────*/

            NODE operator[]( const char* arg ){
                if( memory.OBJECT != nullptr )
                   { return (*memory.OBJECT)[arg]; }
                     throw err_t("Object does not exist");
            }

            NODE operator[]( int arg ){
                if( memory.ARRAY != nullptr )
                   { return (*memory.ARRAY)[arg]; }
                     throw err_t("Array does not exist");
            }

        /*────────────────────────────────────────────────────────────────────────────*/

            void operator=( const char* arg ){ set_cchar( arg ); }
            void operator=( object arg ){ set_object( &arg ); }
            void operator=( double arg ){ set_float( arg ); }
            void operator=( array arg ){ set_array( &arg ); }
            void operator=( float arg ){ set_float( arg ); }
            void operator=( char arg ){ set_char( arg ); }
            void operator=( bool arg ){ set_bool( arg ); }
            void operator=( int arg ){ set_int( arg ); }
    };

/*
    void print_arr( std::ostream& a, NODE& b );
    void print_obj( std::ostream& a, NODE& b );
*/
};

std::ostream& operator<<( std::ostream& a, _JSON::NODE& b ){
    switch( b.get_type() ){
    //  case 0: _JSON::print_arr(a,b);        break;
    //  case 1: _JSON::print_obj(a,b);        break;
        case 2: a << (char*)b;                break;
        case 3: a << (float)b;                break;
        case 4: a << (char)b;                 break;
        case 5: a << (bool)b;                 break;
        case 6: a << (int)b;                  break;
        default: a << "undefined";            break;
    }   return a;
}
/*
void _JSON::print_arr( std::ostream& a, NODE& b ){
    std::vector<ptr_t<NODE>> tmp = b;
    for( auto x:tmp ) a << (*x); 
}

void _JSON::print_obj( std::ostream& a, NODE& b ){
    map_t<char*,ptr_t<NODE>> tmp = b;
    for( auto x:tmp ) a << x.first << ": " <<  (*x.second); 
}
*/

class JSON {} json;

using json_t = _JSON::NODE;

/*
class mem { public:

    template< class T >
    function_t<int(T*)> set( T memory ){
        function_t<int(T*)> _func([=]( T* data ){
            if( !std::is_same<T,decltype(memory)>::value ) 
              { return -1; } (*data) = memory; return 1;
        });     return _func;
    }

};

struct animal { string_t nombre; };

int ready(){

    mem p; auto f1 = p.set(10); int q; f1(&q);
    cout << "entero: " << q << endl;

    auto f2 = p.set(string_t("hola mundo"));
    string_t r; f2(&r); cout << "string: " << r << endl;

    f2 = p.set(string_t("hola mundo de mierda"));
    f2(&r); cout << "string: " << r << endl;

    animal a1 = { .nombre = string_t("perro") };
    animal s; auto f3 = p.set(a1); f3(&s);
    cout << "struct: " << s.nombre << endl;

    return 0;
}
*/