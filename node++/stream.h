#ifndef NODEPP_STREAM
#define NODEPP_STREAM

#include "algorithm.h"
#include "event.h"

class _core_t { 
    
    protected: 
    ptr_t<string_t>    borrow;
    ptr_t<uint>        chunk;
    ptr_t<int>         state;
    ptr_t<char>        buffer; 
    int d = 0;     int sid = -1;

    /*────────────────────────────────────────────────────────────────────────────*/

    public:
    event_t<>          onUnpipe;
    event_t<>          onDrain;
    event_t<>          onClose;
    event_t<>          onPipe;
    event_t<string_t>  onData;

    /*────────────────────────────────────────────────────────────────────────────*/

    _core_t() : borrow(new string_t()), chunk(new uint(0)), state(new int(0)), buffer(nullptr) {} 
    virtual ~_core_t(){ }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t get_borrow(){ return *borrow; }
        void del_borrow(){ borrow->clear(); }
        void set_borrow( string_t brr ){ *borrow = brr; }
    
    /*────────────────────────────────────────────────────────────────────────────*/

    bool is_closed(){ return ( ( is_feof() && borrow->empty() ) || is_disable() ); }

    virtual int get_fd(){ return sid; }

    virtual bool is_feof(){ return 0; }

    int get_state(){ return (*state); }

    bool is_disable(){
        bool act = ((*state) < 0 );
        if((*state) == -2 )(*state) = 0;
        return act; 
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    virtual int w_ready(){ return 1; }
    virtual int r_ready(){ return 1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    virtual uint set_buffer_size(uint size ){ *chunk = size; buffer = new char[size]; return size; }
    virtual uint get_buffer_size(            ){ return (*chunk); }
    virtual int  set_blocking_mode( int mode ){ return mode; }
    
    /*────────────────────────────────────────────────────────────────────────────*/

            void end( string_t msg="" ){ write( msg ); close(); }
    virtual int write( string_t chunk ){ return chunk.size(); }
            void close(){ (*state) = -1; borrow->clear(); }
    virtual string_t read(){ return std::to_string('\0'); }

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t readline(){ string_t s; (*state) = 1;

        if( borrow->empty() ){ (*borrow) += read(); }
        if( borrow->empty() ){ (*state) = -1; } else {
            int c=1; for( auto x:(*borrow) ) if( x!=10 ) c++; 
                else { break; } s = borrow->splice( 0, c );
        }

        if( (*state) > 0 ){ (*state) = 0; } return s;
    }

    /*────────────────────────────────────────────────────────────────────────────*/

    void unpipe(){ (*state) = -2; onUnpipe.emit(); }

};

class stream_t : public _core_t {

    protected:
    FILE* v = nullptr;

    virtual void  free( FILE* v ){ ::fclose(v); } 
    virtual FILE* open( string_t path, const char* mode ){ return ::fopen( path.c_str(), mode ); }

    public: 
    uint range[2] = { 0, 0 };

    stream_t( uint _size=CHUNK_SIZE ){
        v = tmpfile(); set_buffer_size( _size ); if( v == nullptr ) 
        throw err_t("such file or directory does not exist");
    }

    stream_t( string_t path, const char* mode="r", uint _size=CHUNK_SIZE ){ 
        v = open( path, mode ); set_buffer_size( _size ); if( v == nullptr ) 
        throw err_t("such file or directory does not exist");
    }

   ~stream_t(){ if( d ) return; if( state.count() == 1 ){
        onClose.emit(); free(v); (*state) = -1;
    }   d = 1; }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint size() { uint size=0; if( v == nullptr ) 
        { return size; } fpos_t pos; fgetpos( v, &pos );
        if( fseek( v, 0, SEEK_END )!=-1 )
          { size = ftell(v); } fsetpos( v, &pos );
    return size; }

    /*────────────────────────────────────────────────────────────────────────────*/

    uint pos(){ return ftell(v); }
    uint pos( uint _pos ){ return fseek( v, _pos, SEEK_SET ); }

    /*────────────────────────────────────────────────────────────────────────────*/

    int get_fd(){
    #ifdef __WIN32
        return _fileno( v );
    #else 
        return  fileno( v );
    #endif
    }
    
    FILE* get_file(){ return v; }

    /*────────────────────────────────────────────────────────────────────────────*/

    bool is_feof(){ return feof(v) > 0; }
    void reset( uint _pos=0 ){ rewind(v); pos(_pos); (*state) = 0; }
 
    /*────────────────────────────────────────────────────────────────────────────*/

    string_t read(){ string_t s; if( is_closed() )
        { (*state)=-1; return s; } (*state) = 1; int c=0; fflush( v );

        if(!borrow->empty() ){ s = (*borrow); borrow->clear(); return s; }
        if( pos() >=range[1] && range[1] != 0 ){(*state) = -1; return s; }
        if( pos() < range[0] && range[1] != 0 ){ pos( range[0] ); }

        uint act = *chunk; if( range[1] != 0 ) 
             act = std::min( pos()-range[1], act );

        if( (c=fread( &buffer, sizeof(char), act, v ))<=0 )
            (*state) = -1; else s = string_t( &buffer, c );

        if( (*state) > 0 ){ (*state)=0; } return s;
    }
    /*────────────────────────────────────────────────────────────────────────────*/

    int write( string_t msg ){ if( is_closed() || msg.empty() ) 
        { (*state)=-1; return -1; } (*state) = 1; int c=0; fflush( v );

        if( (c=fwrite( msg.c_str(), sizeof(char), msg.size(), v ))<=0 ) 
          { (*state) = -1; } if( (*state)>0 ) { (*state)=0; } return c;
    }

};

class exec_t : public stream_t { 
    
    protected: 

#ifdef __WIN32
    void  free( FILE* v ){ ::_pclose(v); } 
    FILE* open( string_t path, const char* mode ){ return ::_popen( path.c_str(), mode ); }
#else 
    void  free( FILE* v ){ ::pclose(v); } 
    FILE* open( string_t path, const char* mode ){ 
        return ::popen( path.c_str(), mode ); }
#endif
    
    public:

    exec_t( string_t path, const char* mode="r", uint _size=CHUNK_SIZE ) {
        v = open( path, mode ); set_buffer_size( _size ); if( v == nullptr ) 
        throw err_t("such file or directory does not exist");
    }

    ~exec_t(){ if( d ) return; if( state.count() == 1 ){
        onClose.emit(); free(v); (*state) = -1;
    }   d = 1; }

};

class system_t { public: int status = 0;
    system_t( string_t cmd ){ status = system( cmd.c_str() ); }
    operator int (void){ return status; }
};

template< class... T > 
string_t exec_sync( T... args ){ 
    string_t output; exec_t p( args... );
    while( !p.is_closed() ){ output += p.read(); }    
    return output;
}

/*───────────────────────────────────────────────────────────────────────────────────────────────────*/

namespace stream {

    stream_t* readable_new( string_t path, uint _size=CHUNK_SIZE ){ return new stream_t(path, "r", _size); }
    stream_t  readable( string_t path, uint _size=CHUNK_SIZE ){ return stream_t(path, "r", _size); }

    stream_t* writable_new( string_t path, uint _size=CHUNK_SIZE ){ return new stream_t(path, "w", _size); }
    stream_t  writable( string_t path, uint _size=CHUNK_SIZE ){ return stream_t(path, "w", _size); }

    stream_t* duplex_new( uint _size=CHUNK_SIZE ){ return new stream_t( _size ); }
    stream_t  duplex( uint _size=CHUNK_SIZE ){ return stream_t( _size ); }

    template< class T >
    void unpipe( T input ){ input.unpipe(); }

    template< class T, class V >
    void pipe( T input, V output ){
        process::poll::add([=]( T inp, V out ){ static string_t data;
        $Start
            while( inp.get_state() == 1 || out.get_state() == 1 )
                 { $Goto(0); } inp.onPipe.emit();
            while( !inp.is_closed() && !out.is_closed() ){
                data = inp.read(); out.write(data); inp.onData.emit(data); $Yield(1);
            }   out.onDrain.emit(); $Set(0);
        $End
        }, input, output );
    }

    template< class T >
    void pipe( T input ){
        process::poll::add([=]( T inp ){ static string_t data;
        $Start
            while( inp.get_state() == 1 ){ $Goto(0); } inp.onPipe.emit();
            while(!inp.is_closed() ){
                data = inp.read(); inp.onData.emit(data); $Yield(1);
            }   inp.onDrain.emit(); $Set(0);
        $End
        }, input );
    }

    template< class T, class V >
    void pipeline( T input, V output ){
        process::poll::add([=]( T inp, V out ){ static string_t data;
        $Start
            while( inp.get_state() == 1 || out.get_state() == 1 )
                 { $Goto(0); } inp.onPipe.emit();
            while(!inp.is_closed() && !out.is_closed() ){
                data = inp.readline(); out.write(data); 
                inp.onData.emit(data); $Yield(1);
            }   out.onDrain.emit();    $Set(0);
        $End
        }, input, output );
    }

    template< class T >
    void pipeline( T input ){
        process::poll::add([=]( T inp ){ static string_t data;
        $Start
            while( inp.get_state() == 1 ){ $Goto(0); } 
                   inp.onPipe.emit();
            while(!inp.is_closed() ){
                data = inp.readline(); 
                inp.onData.emit(data); $Yield(1);
            }   inp.onDrain.emit();    $Set(0);
        $End
        }, input );
    }

}

/*───────────────────────────────────────────────────────────────────────────────────────────────────*/

namespace process {
    template< class... T > exec_t exec_sync( T... args ){ return ::exec_sync(args...); }
    template< class... T > system_t spawn( T... args ){ return system_t(args...); }
    template< class... T > exec_t exec( T... args ){ return exec_t(args...); }
}

#endif
