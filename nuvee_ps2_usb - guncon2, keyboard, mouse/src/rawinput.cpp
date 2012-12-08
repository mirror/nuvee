#include "raw_mouse.h"
#include <stdio.h>



void Init_RawInput()
{
	if( hidmouse_api != HIDMOUSE_API_RAWINPUT ) return;


	// prevent double hooking
	if( rawinput_active == 1 ) {
	  //MessageBox(NULL, "RawInput already active.  Exiting." , "Error!" ,MB_OK);
		return;
	}


	if (!init_raw_mouse(1, 0, 1)) { // registers for (sysmouse=yes,  terminal services mouse=no, HID_mice=yes)
	  //MessageBox(NULL, "RawInput not supported by Operating System.  Exiting." , "Error!" ,MB_OK);
	  return; 
	}



	rawinput_active = 1;
}


void Add_RawInput( HWND hwnd )
{
	if( rawinput_active == 0 ) return;
	if( hidmouse_api != HIDMOUSE_API_RAWINPUT ) return;


	register_raw_mouse( hwnd );


	// setup WM_INPUT hooks
	rawinput_count++;

	if( rawinput_count == 1 && pOldWinProc == 0 ) {
		pOldWinProc = (WNDPROC)GetWindowLong( hWnd_app, GWL_WNDPROC );
		SetWindowLong( hWnd_app, GWL_WNDPROC, (long) nuvee_key_winproc );
	}
}


void Poll_RawInput()
{
#if 0
	static FILE *fp_debug = 0;


	if( fp_debug == 0 ) {
		fp_debug = fopen( "debug.txt", "w" );
	}

	fprintf( fp_debug, "%d\n", raw_mouse_count() );
#endif

	if( rawinput_active == 0 ) return;
	if( hidmouse_api != HIDMOUSE_API_RAWINPUT ) return;



	for( int lcv = 0; lcv < raw_mouse_count(); lcv++ ) {
		device_x[lcv] = get_raw_mouse_x_delta(lcv);
		device_y[lcv] = get_raw_mouse_y_delta(lcv);
		device_z[lcv] = get_raw_mouse_z_delta(lcv);

		device_buttons[lcv][0] = is_raw_mouse_button_pressed(lcv, 0);
		device_buttons[lcv][1] = is_raw_mouse_button_pressed(lcv, 1);
		device_buttons[lcv][2] = is_raw_mouse_button_pressed(lcv, 2);
		device_buttons[lcv][3] = is_raw_mouse_button_pressed(lcv, 3);
		device_buttons[lcv][4] = is_raw_mouse_button_pressed(lcv, 4);


		// works after WM_INPUT received
		device_absolute[lcv] = is_raw_mouse_absolute(lcv);
	}
}


void Remove_RawInput( HWND hwnd )
{
	if( rawinput_active == 0 ) return;
	if( hidmouse_api != HIDMOUSE_API_RAWINPUT ) return;


	unregister_raw_mouse( hwnd );


	// remove hooks
	rawinput_count--;

	if( rawinput_count == 0 && pOldWinProc ) {
		SetWindowLong( hWnd_app, GWL_WNDPROC, (long)pOldWinProc );
		pOldWinProc = 0;
	}
}


void Done_RawInput()
{
}