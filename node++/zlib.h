#ifndef NODEPP_ZLIB
#define NODEPP_ZLIB

#include "assets/brotli/decode.h"
#include "assets/brotli/encode.h"
#include "assets/zconf.h"
#include "assets/zlib.h"

#include "algorithm.h"
#include "stream.h"

namespace zlib {

    BrotliDescompress( string_t buffer ){}
    BrotliCompress( string_t buffer ){}

    Inflate( string_t buffer ){}
    Deflate( string_t buffer ){}

    Guzip( string_t buffer ){}
    Gzip( string_t buffer ){}

}

#endif