#define NODEPP_SERIAL
#define NODEPP_SERIAL

#include "algorithm.h"
#include "stream.h"

class serial_t : public stream_t { 
	
	protected:

#ifdef __WIN32
	int _set_baud_rate( string_t path, int baud ){
		return system(format("MODE ${0}:${1}",path.c_str(),baud).c_str());
	}
#else
	int _set_baud_rate( string_t path, int baud ){
		return system(format("stty -F ${0} ${1}",path.c_str(),baud).c_str());
	}	
#endif

	public:

	serial_t( string_t path, int baud=-1, int _size=CHUNK_SIZE ){ 
		if( baud != -1 ) _set_baud_rate( path, baud );
		set_buffer_size( _size ); v = fopen( path.c_str(), "r" );
		if( v == nullptr ) throw err_t("such device does not exist");
	}

};

#ifndef