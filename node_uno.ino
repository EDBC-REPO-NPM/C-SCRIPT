#include "node++/node++.h"
#include "node++/timer.h"

void _Ready() {

  int pin[4] = { 13, 12, 11, 10 };
  int i=sizeof(pin); while( i-- )
    pinMode( pin[i], OUTPUT );

  timer::interval([](){
    static bool b = 1;
    digitalWrite(13,b); b = !b;
  },200);

  timer::interval([](){
    static bool b = 1;
    digitalWrite(12,b); b = !b;
  },150);

}