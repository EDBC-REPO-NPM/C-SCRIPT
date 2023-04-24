#ifndef PATH_CSCRIPT
#define PATH_CSCRIPT

using namespace std;

struct path_t {
    string dir;
    string root; 
    string file;
    string name;
    string ext;
};

class Path { public:

    bool isAbsolute( string path ){
        regex reg("^[/\\]|^c:"); return test( path, reg );
    }

    string extname( string path ){ string m;
        regex reg("[.]\\w*$"); if( !test( path, reg ) ) return m;
        m = match( path, reg ); return m;
    }

    template< class U, class... T >
    string join( U argc, T... args ){ vector<string> B; string C;
        pack( [&]( U x ){ push(B,x); }, argc, args... );
        for( auto i=B.begin(); i!=B.end(); i++ ){
            if( !is_same<string,decltype(B[0])>::value ) C += to_string(*i); 
            else                                         C += (*i);
            if( i+1 != B.end() )                         C += "/";
        }   return C;
    }


            

    string dirname( string path ){
        auto vec = split( path, regex("/") );
        vec.pop_back(); return std::join(vec,"/");
    }

    string basename( string path ){ regex reg("[^/]+"); 
        auto vec = matchAll( path, reg );
        return vec[ vec.size()-1 ];
    }

    string basename( string path, string del ){ regex reg("[^/]+"); 
        auto vec = matchAll( path, reg ); regex d(del);
        return replace( vec[ vec.size()-1 ], d, "" );
    }

    string format( path_t obj ) { string result;
        if( !(obj.ext.empty()) && !(obj.name.empty()) ) 
            obj.file = obj.name + obj.ext;
        if( !(obj.root.empty()) ) result += obj.root;
        if( !(obj.dir.empty())  ) result += obj.dir;
        if( !(obj.file.empty()) ) result += obj.file;
        return result;
    }

    path_t parse( string path ) { path_t result;

        if( test( path, regex("[/]") ) ) result.root = "/";
        else                             result.root = "";

        result.ext = extname( path ); 
        result.dir = dirname( path );
        result.file = basename( path );
        result.name = basename( path, result.ext );

        return result;
    };

    string replative( string path_a, string path_b ){ string result;
        auto vec_a = split( path_a, regex("/") );
        auto vec_b = split( path_b, regex("/") );
        int i; bool j; do {
            if( vec_a[i] == vec_b[i] ) j = true;
            if( !j ) result += "../";
            else result += vec_b[i];
            i++;
        } while( i!=vec_b.size() );
        return result;
    }  

    string normalize( string path ){ 
        string null("."); if( same( path, "" ) ) return null;
        path = replaceAll( path, regex("[/]+"), "/" );
        if( test( path, regex("[.]{2}[/]") ) ){
            regex reg("[.]{2}[/][^/]+/|[.]{2}[/]");
            path = replaceAll( path, reg, "" );
        }   return path;
    }

    string format( string path ){ return ""; }

}   path;

#endif 