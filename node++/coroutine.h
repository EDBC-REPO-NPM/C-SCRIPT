#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
#define $Ready _init_(); int main( int argc, char** args ){ process::start( argc, args ); _init_(); while( !process::empty() ) process::next(); return 0; } void _init_
#else
#define $Ready loop(){ process::next(); } void setup
#endif

/*────────────────────────────────────────────────────────────────────────────*/

#define $Yield(VALUE) do { _state_ = VALUE; return 1; case VALUE:; } while (0)
#define $Start static int _state_ = 0; { switch(_state_) { case 0:;
#define $Goto(VALUE) do { _state_ = VALUE; return 1; } while (0)
#define $End return -1; } _state_ = 0; return -1; }
#define $Return(VALUE) return VALUE
#define $Set(VALUE) _state_ = VALUE
#define $Get _state_

/*────────────────────────────────────────────────────────────────────────────*/

#endif