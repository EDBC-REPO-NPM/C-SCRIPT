#ifndef NODEPP_CRYPTO
#define NODEPP_CRYPTO

#include "algorithm.h"
#include "stream.h"

namespace crypto {

    string_t MD5( string_t message ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t SHA256( string_t message ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t SHA1( string_t message ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t sha512( string_t message ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t rmd160( string_t message, int output_size  ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t HMAC( string_t message, int output_size ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t PBKDF2( string_t message, stirng_t salt, string_t password ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t base64_enc( string_t message ){}

    /*────────────────────────────────────────────────────────────────────────────*/

    string_t base64_dec( string_t message ){}

}
#endif