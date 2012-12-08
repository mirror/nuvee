/*
Special thanks to Lilypad team
- pcsx2.net

Special thanks to ePSXe team
- ePSXe.com

Special thanks to PCSX-reloaded team
- codeplex.pcsxr.com

Special thanks to jstookey (Jake Stookey)
- rawinput api
- www.jstookey.com/arcade/rawmouse/
*/


#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <dinput.h>
#include <stdio.h>


//#define DEBUG_API


// include the DirectX Library files
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


#define _WIN32_WINNT 0x501
#define WM_INPUT 0x00FF


#include "globals.h"
#include "resource.h"



char *GetIniPath( char *str )
{
	if( emu_type == 1 )
		strcpy( ini_path, "plugins" );


	strcpy( temp_str, ini_path );
	strcat( temp_str, "\\" );
	strcat( temp_str, str );

	return temp_str;
}


void SnapCursor()
{
	static int snap_count = 0;


	if( snap_mouse_cursor == 0 || GetForegroundWindow() != hWnd_app ) {
		ClipCursor( NULL );

		snap_count = 0;
	}



	// restrain windows cursor (+ multi-monitor fix)
	else if(
		port_type[0] == PAD_TYPE_MOUSE || port_type[1] == PAD_TYPE_MOUSE ||
		port_type[0] == PAD_TYPE_GUNCON || port_type[1] == PAD_TYPE_GUNCON )
	{
		RECT rect_self;


		// alt-tab focus check
		if( snap_count < 8 )
			snap_count++;
		else
		{
			GetWindowRect( hWnd_app, &rect_self );

			
			// chop window title bar
			// - mult-monitor = left may be non-zero
			if( rect_self.top != 0 ) {
				rect_self.top += 32;

				rect_self.top += 0;
				rect_self.left += 4;
				rect_self.right -= 4;
				rect_self.bottom -= 4;
			}


			ClipCursor( &rect_self );
		}
	}
}



#include "raw_mouse.h"
#include "rawinput.cpp"
#include "dinput.cpp"

#include "config.cpp"

#include "mouse.cpp"
#include "guncon.cpp"
#include "justifier.cpp"


void ShowCursor( int player, int x, int y )
{
	if( gpuCursor )
		gpuCursor( player, x, y );
}


void Reset_Pad()
{
	pad_active = 0;
	pad_count = 0;
	
	pad_config_mode[0] = 0;
	pad_config_mode[1] = 0;



	// override
	if( cmdline_port_type[0] != -1 )
		port_type[0] = cmdline_port_type[0];

	if( cmdline_port_type[1] != -1 )
		port_type[1] = cmdline_port_type[1];



	pad_active = 1;
	switch( port_type[ THIS_PAD ] )
	{
		case PAD_TYPE_NONE:
			break;

		case PAD_TYPE_MOUSE:
			LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );
			break;

		case PAD_TYPE_GUNCON:
			LoadConfig_Guncon_Presets( GetIniPath( "nuvee_psx_guncon_profiles.ini" ) );
			LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon1.ini" ) );
			break;

		case PAD_TYPE_JUSTIFIER:
			LoadConfig_Justifier_Presets( GetIniPath( "nuvee_psx_justifier_profiles.ini" ) );
			LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier1.ini" ) );
			break;
	}


	pad_active = 2;
	switch( port_type[ THIS_PAD ] )
	{
		case PAD_TYPE_NONE:
			break;

		case PAD_TYPE_MOUSE:
			LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse2.ini" ) );
			break;

		case PAD_TYPE_GUNCON:
			LoadConfig_Guncon_Presets( GetIniPath( "nuvee_psx_guncon_profiles.ini" ) );
			LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon2.ini" ) );
			break;

		case PAD_TYPE_JUSTIFIER:
			LoadConfig_Justifier_Presets( GetIniPath( "nuvee_psx_justifier_profiles.ini" ) );
			LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier2.ini" ) );
			break;
	}
}




int CALLBACK PSEgetLibType( void )
{
	emu_type = 1;

	return 8;
}


const char* CALLBACK PSEgetLibName( void )
{
	emu_type = 1;

	return PS1_LibraryName;
}


int CALLBACK PSEgetLibVersion( void )
{
	emu_type = 1;

	return (version << 16) | (revision << 8) | build;
}



int CALLBACK PS2EgetLibType( void )
{
	// PCSX2 - first-time emu detection
	fp_emu = fopen( GetIniPath( "nuvee_emu.txt" ), "w" );
	if( fp_emu ) {
		fprintf( fp_emu, "ps2\n" );
		fclose( fp_emu );
	}


	emu_type = 2;

	return PS2E_LT_PAD;
}


const char* CALLBACK PS2EgetLibName( void )
{
	// PCSX2 - first-time emu detection
	fp_emu = fopen( GetIniPath( "nuvee_emu.txt" ), "w" );
	if( fp_emu ) {
		fprintf( fp_emu, "ps2\n" );
		fclose( fp_emu );
	}


	emu_type = 2;

	return PS2_LibraryName;
}


int CALLBACK PS2EgetLibVersion2( int type )
{
	// PCSX2 - first-time emu detection
	fp_emu = fopen( GetIniPath( "nuvee_emu.txt" ), "w" );
	if( fp_emu ) {
		fprintf( fp_emu, "ps2\n" );
		fclose( fp_emu );
	}


	emu_type = 2;

	if (type == PS2E_LT_PAD)
		return (PS2E_PAD_VERSION<<16) | PS2_VERSION;
}


// Blade_Arma - pcsxr interface
void CALLBACK PADregisterCursor(void (CALLBACK *callback)(int, int, int))
{
	// player=0-7, x=0-511, y=0-255
	gpuCursor = callback;
}


LRESULT WINAPI nuvee_key_winproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
  {
		case WM_INPUT:
			PADhandleHID( lParam );
			break;
	}


	return CallWindowProc( pOldWinProc, hwnd, message, wParam, lParam );
}


int CALLBACK PADinit( int flags )
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "PADinit\n" );
#endif

	return 0;
}


void CALLBACK PADshutdown( void )
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "PADshutdown\n" );
#endif

}


void CALLBACK PADconfigure (void)
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "PADconfigure\n" );
#endif


	// PCSX2 - first-time emu detection
	fp_emu = fopen( GetIniPath( "nuvee_emu.txt" ), "r" );
	if( fp_emu ) {
		char emu_name[256];

		fscanf( fp_emu, "%s", emu_name );


		if( strcmp( emu_name, "ps2" ) == 0 ) {
			emu_type = 2;
			strcpy( ini_path, "inis" );
		}
		
		else if( strcmp( emu_name, "ps1" ) == 0 ) {
			emu_type = 1;
			strcpy( ini_path, "plugins" );
		}


		fclose( fp_emu );
	}




	// HID acquire
	hWnd_app = GetActiveWindow();


	DialogBox( hInstance_app, MAKEINTRESOURCE (IDD_MAIN), GetActiveWindow(), (DLGPROC) MainDlgProc );
}


int CALLBACK PADopen( HWND hWnd )
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "PADopen\n" );
#endif


	init_done++;
	if( init_done > 1 ) return 0;


	hWnd_app_key = hWnd;


	// SSSPSX
	if (!IsWindow (hWnd) && !IsBadReadPtr ((u32*)hWnd, 4))
		hWnd = *(HWND*)hWnd;
	if (!IsWindow (hWnd))
		hWnd = NULL;
	else
	{
		while( GetWindowLong( hWnd, GWL_HWNDPARENT ) )
			hWnd = GetParent (hWnd);
	}

	hWnd_app = hWnd;


	
	// load config files
	LoadConfig_Main();



	// PCSX2 procedure - this hWnd works (GS window)
	Init_RawInput();
	Add_RawInput( hWnd_app );

	Init_DInput( hInstance_app, hWnd_app );


	Reset_Pad();


	if( port_type[0] == PAD_TYPE_MOUSE || port_type[1] == PAD_TYPE_MOUSE ||
			port_type[0] == PAD_TYPE_GUNCON || port_type[1] == PAD_TYPE_GUNCON )
		SnapCursor();

	return 0;
}


void CALLBACK PADclose (void)
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "PADclose\n" );
#endif


	init_done--;
	if( init_done == 0 )
	{
		Done_DInput();

		Remove_RawInput( hWnd_app );
		Done_RawInput();


		ClipCursor( NULL );
		hWnd_app = 0;
	}
}


// returns: 1 if supports pad1
//			2 if supports pad2
//			3 if both are supported
int CALLBACK PADquery( void )
{
	return 3;
}


// depracated?
typedef struct
{
	unsigned char controllerType;
	unsigned short buttonStatus;
	unsigned char rightJoyX, rightJoyY, leftJoyX, leftJoyY;
	unsigned char moveX, moveY;
	unsigned char reserved[91];
} PadDataS;


long PADreadPort1( PadDataS* pads )
{
	memset( pads,0,sizeof(pads) );

	pad_active = 1;
	return 0;
}


long PADreadPort2( PadDataS* pads )
{
	memset( pads,0,sizeof(pads) );

	pad_active = 2;
	return 0;
}


u8 CALLBACK PADstartPoll( int pad )
{
	if( port_type[0] == PAD_TYPE_MOUSE || port_type[1] == PAD_TYPE_MOUSE ||
			port_type[0] == PAD_TYPE_GUNCON || port_type[1] == PAD_TYPE_GUNCON ||
			port_type[0] == PAD_TYPE_JUSTIFIER || port_type[1] == PAD_TYPE_JUSTIFIER )
		SnapCursor();



	if( emu_type == 1 ) {
		if( hWnd_app == GetActiveWindow() ) {
			Remove_RawInput( hWnd_app );

			// rehook WM_INPUT after fullscreen switch
			Add_RawInput( hWnd_app );
		}
	}



	mouse_active = 0;

	if( port_type[0] == PAD_TYPE_MOUSE ||
			port_type[0] == PAD_TYPE_GUNCON ||
			port_type[0] == PAD_TYPE_JUSTIFIER ) {
		mouse_active++;
	}

	if( port_type[1] == PAD_TYPE_MOUSE ||
			port_type[1] == PAD_TYPE_GUNCON ||
			port_type[1] == PAD_TYPE_JUSTIFIER ) {
		mouse_active++;
	}


	if( mouse_active == 2 )
	{
		if( emu_type == 1 )
		{
			// PCSX1 - one / two input plugins (1 port each)
			// - polls pad1, then pad2 (two calls)
			if( init_done == 1 ) 
				mouse_active = 1;
			else
				mouse_active = pad;
		}

		else if( emu_type == 2 )
		{
			// PCSX2 - one input plugin (2 ports)
			// - polls pad1, pad2 together
			mouse_active = pad;
		}
	}


	// pad = 1-2
	pad_active = pad;
	pad_count = 0;


	// okay to poll mouse
	if( mouse_active == 1 )
	{
		if( port_type[ THIS_PAD ] == PAD_TYPE_MOUSE ||
				port_type[ THIS_PAD ] == PAD_TYPE_GUNCON ||
				port_type[ THIS_PAD ] == PAD_TYPE_JUSTIFIER )
		{
			if( rawinput_active )
				Poll_RawInput();
			else
				Poll_Mouse_DInput();

		
			Poll_Joystick_DInput();
		}
	}


	/*
	// turn off cursor
	if( port_type[ THIS_PAD ] != PAD_TYPE_GUNCON &&
			port_type[ THIS_PAD ] != PAD_TYPE_JUSTIFIER )
	{
		ShowCursor( pad_active, 0, 0 );
	}
	*/


	return 0xff;
}


u8 CALLBACK PADpoll( char value )
{
	if( pad_count == 0 ) {
		//ShowCursor( -1, 0, 0 );


		// no controller data
		memset( pad_out, 0xff, sizeof(pad_out) );
	}



	switch( port_type[ THIS_PAD ] ) {
	case PAD_TYPE_NONE:
		//ShowCursor( -1, 0, 0 );


		// Frogger 2 - no controller (!)
		pad_out[0] = 0xff;
		pad_out[1] = 0xff;
		break;


	case PAD_TYPE_MOUSE:
		//ShowCursor( -1, 0, 0 );

		PADpoll_mouse( value );
		break;


	case PAD_TYPE_GUNCON:
		PADpoll_guncon( value );
		break;


	case PAD_TYPE_JUSTIFIER:
		PADpoll_justifier( value );
		break;
	}


	return pad_out[ pad_count++ ];
}


void CALLBACK PADabout (void)
{
	if( emu_type == 1 )
		MessageBox( NULL, "nuvee psx controller", "About", NULL );
	
	else if( emu_type == 2 )
		MessageBox( NULL, "nuvee ps2 controller", "About", NULL );
}


int CALLBACK PADtest (void)
{
	return 0;
}

	
// custom HID - dual lightguns
void PADhandleHID( LPARAM in_device_handle )
{
	if( rawinput_active )
		add_to_raw_mouse_x_and_y( (HRAWINPUT) in_device_handle );
}


// command-line support
char name1[512], name2[512];
void CALLBACK PADcmdline( char *cmdline )
{
	char *ptr;


	ptr = cmdline;
	while( ptr[0] ) {
		// check valid cmdline arg
		if( ptr[0] != '-' ) {
			ptr++;
			continue;
		}


		// check keyphrases
		sscanf( ptr, "%s %s", &name1, &name2 );
		ptr++;

		if( strcmp( name1, "-nuvee_guncon1_model" ) == 0 )
		{
			if( strcmp( name2, "homebrew" ) == 0 ) {
				cmdline_guncon_model[0] = GUNCON_MODEL_HOMEBREW;
			}

			else if( strcmp( name2, "custom" ) == 0 ) {
				cmdline_guncon_model[0] = GUNCON_MODEL_CUSTOM;
			}

			else if( strcmp( name2, "unlicensed" ) == 0 ) {
				cmdline_guncon_model[0] = GUNCON_MODEL_UNLICENSED;
			}

			else if( strcmp( name2, "namco" ) == 0 ) {
				cmdline_guncon_model[0] = GUNCON_MODEL_NAMCO;
			}
		}


		else if( strcmp( name1, "-nuvee_guncon2_model" ) == 0 )
		{
			if( strcmp( name2, "homebrew" ) == 0 ) {
				cmdline_guncon_model[1] = GUNCON_MODEL_HOMEBREW;
			}

			else if( strcmp( name2, "custom" ) == 0 ) {
				cmdline_guncon_model[1] = GUNCON_MODEL_CUSTOM;
			}

			else if( strcmp( name2, "unlicensed" ) == 0 ) {
				cmdline_guncon_model[1] = GUNCON_MODEL_UNLICENSED;
			}

			else if( strcmp( name2, "namco" ) == 0 ) {
				cmdline_guncon_model[1] = GUNCON_MODEL_NAMCO;
			}
		}


		else if( strcmp( name1, "-nuvee_guncon1_profile" ) == 0 ) {
			strcpy( cmdline_guncon_id[0], name2 );
		}


		else if( strcmp( name1, "-nuvee_guncon2_profile" ) == 0 ) {
			strcpy( cmdline_guncon_id[1], name2 );
		}


		else if( strcmp( name1, "-nuvee_port1" ) == 0 ) {
			if( strcmp( name2, "none" ) == 0 ) {
				cmdline_port_type[0] = PAD_TYPE_NONE;
			}

			else if( strcmp( name2, "mouse" ) == 0 ) {
				cmdline_port_type[0] = PAD_TYPE_MOUSE;
			}

			else if( strcmp( name2, "guncon" ) == 0 ) {
				cmdline_port_type[0] = PAD_TYPE_GUNCON;
			}

			else if( strcmp( name2, "justifier" ) == 0 ) {
				cmdline_port_type[0] = PAD_TYPE_JUSTIFIER;
			}
		}


		else if( strcmp( name1, "-nuvee_port2" ) == 0 ) {
			if( strcmp( name2, "none" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_NONE;
			}

			else if( strcmp( name2, "mouse" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_MOUSE;
			}

			else if( strcmp( name2, "guncon" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_GUNCON;
			}

			else if( strcmp( name2, "justifier" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_JUSTIFIER;
			}
		}
	}
}


// PCSX2


// PADkeyEvent is called every vsync (return NULL if no event)
// - notifies GUI of keypresses
keyEvent* CALLBACK PADkeyEvent()
{
	static int call_count = 0;
	static int wait_event = 0;


	// safety check
	if( init_done == 0 ) return 0;


	if( init_done == 2 ) {
		call_count++;
		call_count &= 1;

		if( call_count == 1 ) return 0;
	}



	// use for PCSX2-only
	if( emu_type == 2 )
	{
		// don't debounce for some frames
		if( wait_event > 0 ) {
			wait_event--;
			return &save.ev;
		}


		// SSSPSX
		Poll_Keyboard_DInput();


		memcpy( save.state[0], save.state[1], sizeof (save.state[0]) );
		memcpy( save.state[1], di_keystate, sizeof( save.state[1] ) );

		memset( &save.ev, 0, sizeof( save.ev ) );


		for (int cnt = 0; cnt < 256; cnt++)
		{
			if( (~save.state[0][cnt] & save.state[1][cnt] & 0x80) ||
				  (save.state[0][cnt] & ~save.state[1][cnt] & 0x80) )
			{
				save.ev.evt = (save.state[1][cnt] & 0x80) ? 1 : 2;
				save.ev.key = MapVirtualKey (cnt, 1);


				// don't throw too many events at one time
				if( save.ev.evt == 1 )
					wait_event = 0;
				else if( save.ev.evt == 2 )
					wait_event = 60;


				return &save.ev;
			}
		}
	}

	return 0;
}


int CALLBACK PADfreeze(int mode, freezeData *data) {
	return 0;
}


void CALLBACK PADsetSettingsDir( const char *dir ) {
	if( emu_type == 1 ) {
		strcpy( ini_path, "plugins" );
	}
	else {
		strcpy( ini_path, "inis" );

		if( dir != NULL )
			strcpy( ini_path, dir );
	}
}


void CALLBACK PADupdate(int port) {
}


BOOL APIENTRY DllMain( HMODULE hInst, DWORD dwReason, LPVOID lpReserved )
{
	hInstance_app = hInst;
	return TRUE;
}
