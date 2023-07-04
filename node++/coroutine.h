#ifndef NODEPP_COROUTINE
#define NODEPP_COROUTINE

/*────────────────────────────────────────────────────────────────────────────*/

#define _Ready loop(){ process::next(); } void setup

/*────────────────────────────────────────────────────────────────────────────*/

#define _Yield(VALUE) do { _state_ = VALUE; return 1; case VALUE:; } while (0)
#define _Start static int _state_ = 0; { switch(_state_) { case 0:;
#define _Goto(VALUE) do { _state_ = VALUE; return 1; } while (0)
#define _End return -1; } _state_ = 0; return -1; }
#define _Return(VALUE) return VALUE
#define _Set(VALUE) _state_ = VALUE
#define _Get _state_

/*────────────────────────────────────────────────────────────────────────────*/

#endif