#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

#ifndef ARDUINO
#define $Ready _init_(); int main( int argc, char** args ){ nodepp _v_( argc, args ); return _init_(); } int _init_
#else
#define $Ready _init_(); void setup(){ nodepp _v_(); _init_(); } void loop(){} int _init_
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