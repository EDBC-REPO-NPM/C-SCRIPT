#ifndef NODEPP_PEROPHERIC
#define NODEPP_PEROPHERIC
#include "algorithm.h"

#ifdef __WIN32
#include <windows.h>
#include <winuser.h>

namespace peripheric {

	INPUT Input = {0};

namespace screen {
	
	int screen[2] = {
		::GetSystemMetrics( SM_CYSCREEN ),
		::GetSystemMetrics( SM_CXSCREEN )
	};

	function_t<float,float> screen_ref[2] = {
		[]( float value ){ return value * screen[0] / 100; },
		[]( float value ){ return value * screen[1] / 100; }
	};

	int get_screens(){ int count
		return EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&Count) ? count : 0;
	}

}

namespace mouse {

	void set_position( float x, float y ){
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

		Input.mi.dx = screen::screen_ref[0](x) * ( 65535.0f / screen::screen[0] );
		Input.mi.dy = screen::screen_ref[1](y) * ( 65535.0f / screen::screen[1] );

		::SendInput( 1, &Input, sizeof(INPUT) );
	}

	void press_button( int button ){
		Input.type = INPUT_MOUSE; switch( button ){
			case 1: Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;   break;
			case 2: Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  break;
			case 3: Input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN; break;
		}	::SendInput( 1, &Input, sizeof(INPUT) );
	}

	void release_button( int button ){
		Input.type = INPUT_MOUSE; switch( button ){
			case 1: Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;	 break;
			case 2: Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;	 break;
			case 3: Input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP; break;
		} 	::SendInput( 1, &Input, sizeof(INPUT) );break;
	}

	void scroll_up(){ mouse_event(0x0800, 0, 0, 120, 0); }

	void scroll_down(){ mouse_event(0x0800, 0, 0,-120, 0); }

	void click_button( int button ){
		press_button( button ); 
		release_button(button);
    }

	int* get_position() { 
		int result[2] = { 0,0 }; POINT MousePoint;
		if( GetCursorPos( &MousePoint ) ) {
			result[0] = screen::screen_ref[0](MousePoint.x);
			result[1] = screen::screen_ref[1](MousePoint.y);
		}	return result;
	}

}

namespace keyboard {

	void press_button( int key ){
		Input.type = INPUT_KEYBOARD;

		Input.ki.dwFlags = 0;
		Input.ki.dwFlags = 0;
		Input.ki.wVk = key;

		::SendInput( 1, &Input, sizeof(INPUT) );
	}

	void release_button( int key ){
		Input.type = INPUT_KEYBOARD;

		Input.ki.dwFlags = KEYEVENTF_KEYUP;
		Input.ki.dwFlags = 0;
		Input.ki.wVk = key;

		::SendInput( 1, &Input, sizeof(INPUT) );
	}

	void push_button( int key ){
		 press_button( key ); 
		 release_button(key);
	}

	int is_pressed(){
		unsigned int i = 0xFE; while( i --> 0 )
			if( GetKeyState(i) & 0x8000 ) 
			return i; return -1;
	}
}

}

#elif __linux__

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

namespace peripheric {

ptr_t<Display> dpy         = XOpenDisplay(NULL);
int            id          = DefaultScreen(&dpy);
ptr_t<Screen>  scr         = DefaultScreenOfDisplay(&dpy);
Window         root_window = XRootWindow(&dpy,id);

namespace screen {
	
	int screen[2] = { scr->width, scr->height };

	function_t<float,float> screen_ref[2] = {
		[]( float value ){ return value * screen[0] / 100; },
		[]( float value ){ return value * screen[1] / 100; }
	};

	int get_screens(){ return XScreenCount( &dpy ); }

}

namespace mouse {

	void set_position( float x, float y ){
		XTestFakeMotionEvent(&dpy,id,screen::screen_ref[0](x),screen::screen_ref[1](y),CurrentTime);
	    XFlush(&dpy);
	}

	void press_button( int button ){
	    XTestFakeButtonEvent(&dpy,button,1,CurrentTime);
	    XFlush(&dpy);
	}

	void release_button( int button ){
	    XTestFakeButtonEvent(&dpy,button,0,CurrentTime);
	    XFlush(&dpy);
	}

	void scroll_up(){
		XTestFakeButtonEvent(&dpy,4,1,CurrentTime);
		XTestFakeButtonEvent(&dpy,4,0,CurrentTime);
	    XFlush(&dpy);
	}

	void scroll_down(){
		XTestFakeButtonEvent(&dpy,5,1,CurrentTime);
		XTestFakeButtonEvent(&dpy,5,0,CurrentTime);
	    XFlush(&dpy);
	}

	void click_button( int button ){
		press_button( button ); 
		release_button(button);
    }

	int* get_position(){ 
		int result[2] = { 0,0 }; 

		Window root_returned, child_returned;
		int win_x, win_y, root_x, root_y;
		unsigned int mask_return;

		bool done = XQueryPointer( &dpy, root_window, &root_returned, &child_returned, 
								   &root_x, &root_y, &win_x, &win_y, 
								   &mask_return );
			
		if( done ){ result[0] = win_x; result[1] = win_y; } return result;
	}

}

namespace keyboard {

	void press_button( int key ){
		XTestFakeKeyEvent(&dpy,XKeysymToKeycode(&dpy,key),1,0);
	    XFlush(&dpy);
	}

	void release_button( int key ){
	    XTestFakeButtonEvent(&dpy,5,0,CurrentTime);
	    XFlush(&dpy);
	}

	void push_button( int key ){
		press_button( key ); 
		release_button(key);
	}

	int is_pressed(){
		XEvent event; XNextEvent( &dpy, &event );
		if( event.type == KeyPress ) 
			return event.xkey.keycode;
		else if( event.type == ButtonPress )
			return event.xbutton.button;
			return -1;
	}

}

}

#elif ARDUINO
#include "Keyboard.h"
#include "Mouse.h"

namespace peripheric {

namespace screen {
	
}

namespace mouse {

}

namespace keyboard {

}

}

#endif
#endif