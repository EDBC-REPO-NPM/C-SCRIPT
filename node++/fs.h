#ifndef NODEPP_FS
#define NODEPP_FS

#include "algorithm.h"
#include "process.h"
#include "stream.h"

namespace fs {

    stream_t* readable_new( string_t path, uint _size=CHUNK_SIZE ){ return new stream_t(path, "r", _size); }
    stream_t  readable( string_t path, uint _size=CHUNK_SIZE ){ return stream_t(path, "r", _size); }

    stream_t* writable_new( string_t path, uint _size=CHUNK_SIZE ){ return new stream_t(path, "w", _size); }
    stream_t  writable( string_t path, uint _size=CHUNK_SIZE ){ return stream_t(path, "w", _size); }

    string_t read_file( string_t src ){ string_t s;
        stream_t _file( src, (char*)"r" );
        while( !_file.is_closed() )
             { s +=_file.read(); } return s;
    }

    int copy_file( string_t src, string_t des ){ 
        try {
            stream_t _file_a( src, (char*)"r" );
            stream_t _file_b( des, (char*)"w" );
            stream::pipe( _file_a, _file_b ); return 1;
        } catch(...) { return -1; }
    }

    int rename_file( string_t oname, string_t nname ) {
        return rename( oname.c_str(), nname.c_str() );
    }

    int remove_file( string_t path ){
        return remove( path.c_str() );
    }

    int exists_file( string_t path ){
        FILE* _stream = fopen( path.c_str(), (char*)"r" );
        if( _stream == nullptr ) return -1;
        return fclose( _stream ) ? -1 : 1;
    }

    int create_file( string_t path ){
        FILE* _stream = fopen( path.c_str(), (char*)"w" );
        return fclose( _stream );
    }

    int file_size( string_t path ){
        try {
            stream_t file( path.c_str() );
                       return file.size();
        } catch(...) { return -1; }
    }

    void write_file( string_t path, string_t data ){
        stream_t file( path, (char*)"w" );
        file.write( data );
    }

    void append_file( string_t path, string_t data ){
        stream_t file( path, (char*)"a" );
        file.write( data );
    }

#ifdef __WIN32

    int rename_dir( string_t oname, string_t nname ) { 
        if( oname.empty() || nname.empty() ) return -1;
        return rename( oname.c_str(), nname.c_str() ); 
    }

    int move_dir( string_t opath, string_t npath ){ 
        if( opath.empty() || npath.empty() ) return -1;
        string_t cmd = "move "+opath+" "+npath;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int create_dir( string_t path ){ 
        if( path.empty() ) return -1; string_t cmd = "mkdir "+path;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int remove_dir( string_t path ){ 
        if( path.empty() ) return -1; string_t cmd = "rmdir "+path;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int exists_dir( string_t path ){ if( path.empty() ) return -1;
        string_t cmd = "if !exist "+path+" ( echo -1 )";
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    array_t<string_t> read_dir( string_t path ){ 
        if( exists_dir(path)==-1 ) throw err_t("such file or directory does not excist"); 
        array_t<string_t> list; string_t cmd = "forfile "+path+" echo @file"; 
        string_t str; for( auto x:exec_sync(cmd) ){
            if( x == 10 || x == -1 ){
                push( list, str ); 
                str.clear(); continue;
            }   str->push(x);
        }   return list;             
    }

    int copy_dir( string_t opath, string_t npath ){
        if( opath.empty() || npath.empty() ) return -1;
        string_t cmd = "xcopy "+opath+" "+npath;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

#else

    int rename_dir( string_t oname, string_t nname ) { 
        if( oname.empty() || nname.empty() ){ return -1; }
        return rename( oname.c_str(), nname.c_str() ); 
    }

    int move_dir( string_t opath, string_t npath ){ 
        if( opath.empty() || npath.empty() ){ return -1; }
        string_t cmd = "mv "+opath+" "+npath;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int create_dir( string_t path ){ 
        if( path.empty() ){ return -1; } string_t cmd = "mkdir "+path;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int remove_dir( string_t path ){ 
        if( path.empty() ){ return -1; } string_t cmd = "rmdir "+path;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    int exists_dir( string_t path ){ if( path.empty() ){ return -1; }
        string_t cmd = "[ ! -d "+path+" ] && echo -1";
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

    array_t<string_t> read_dir( string_t path ){ 
        if( exists_dir(path)==-1 ){ throw err_t("such file or directory does not excist"); }
        array_t<string_t> list; string_t cmd = "for f in ` ls "+path+"` ; do echo $f ; done";
        string_t str; for( auto x:exec_sync(cmd) ){
            if( x == 10 || x == -1 ){
                list.push( str ); str.erase();
            }   str.push( x );
        }   return list;             
    }

    int copy_dir( string_t opath, string_t npath ){
        if( opath.empty() || npath.empty() ){ return -1; }
        string_t cmd = "cp "+opath+" "+npath;
        return ( exec_sync(cmd).size()<=0 ) ? 1 : -1;
    }

#endif

}
#endif