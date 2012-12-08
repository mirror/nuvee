/*
Special thanks to:
- Gigaherz + NHerve (Luigi___) - USB Keyboard / Buzzer
- QEmu + PS2Dev - (OHCI information)

- Tony Shadwick (numbski) - Guncon2 USB probe + x/y confirmation (Guncon1 behavior)
- smog - Guncon2 button bits

- USBNull (pcsx2.net)
- Lilypad team (pcsx2.net)
- ePSXe team (ePSXe.com)
- PCSX-reloaded team (codeplex.pcsxr.com)

- Jake Stookey (jstookey)
  rawinput api
  www.jstookey.com/arcade/rawmouse/
*/



#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <dinput.h>
#include <stdio.h>


//#define DEBUG_API
#define DEBUG_USB


// include the DirectX Library files
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")


#define _WIN32_WINNT 0x501
#define WM_INPUT 0x00FF



#include "globals.h"
#include "resource.h"



char *GetIniPath( char *str )
{
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
			// - mult-monitor = left may be non-zeo
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


void ShowCursor( int player, int x, int y )
{
	if( gpuCursor )
		gpuCursor( player, x, y );
}



#include "raw_mouse.h"
#include "rawinput.cpp"
#include "dinput.cpp"

#include "usb.cpp"
#include "config.cpp"



void Reset_Pad()
{
	int swap_ports;


	pad_active = 0;
	swap_ports = 0;


	// override
	if( cmdline_port_type[0] != -1 ) port_type[0] = cmdline_port_type[0];
	if( cmdline_port_type[1] != -1 ) port_type[1] = cmdline_port_type[1];


	// (1) main config + (2) guncon config
	for( int reload = 0; reload < 2; reload++ ) {
		/*
		some games swap usb ports (no control over which usb port gets chosen)
		- allow easy fast switching (dual lightguns)
		
		ps2 port0 <--> nuvee port2
		ps2 port1 <--> nuvee port1
		*/

		if( lightgun_swap ) {
			int temp;
			temp = port_type[0];


			port_type[0] = port_type[1];
			port_type[1] = temp;

			lightgun_swap = 0;
			swap_ports = ( swap_ports == 0 ) ? 1 : 0;
		}


		pad_active = 1;
		switch( port_type[ THIS_PAD ] )
		{
			case PAD_TYPE_NONE:
				break;

			case PAD_TYPE_GUNCON:
				LoadConfig_Guncon_Presets( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ) );


				if( swap_ports == 0 )
					LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );
				else
					LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );
				break;

			case PAD_TYPE_MOUSE:
				if( swap_ports == 0 )
					LoadConfig_Mouse( GetIniPath( "nuvee_ps2_usb_mouse1.ini" ) );
				else
					LoadConfig_Mouse( GetIniPath( "nuvee_ps2_usb_mouse2.ini" ) );
				break;
		}


		pad_active = 2;
		switch( port_type[ THIS_PAD ] )
		{
			case PAD_TYPE_NONE:
				break;

			case PAD_TYPE_GUNCON:
				LoadConfig_Guncon_Presets( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ) );


				if( swap_ports == 0 )
					LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );
				else
					LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );
				break;

			case PAD_TYPE_MOUSE:
				if( swap_ports == 0 )
					LoadConfig_Mouse( GetIniPath( "nuvee_ps2_usb_mouse2.ini" ) );
				else
					LoadConfig_Mouse( GetIniPath( "nuvee_ps2_usb_mouse1.ini" ) );
				break;
		}
	}


	lightgun_swap = 0;
}



// Blade_Arma - pcsxr interface
void CALLBACK USBregisterCursor(void (CALLBACK *callback)(int, int, int))
{
	// player=0-7, x=0-511, y=0-255
	gpuCursor = callback;
}


LRESULT WINAPI nuvee_key_winproc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
  {
		case WM_INPUT:
			USBhandleHID( lParam );
			break;
	}


	return CallWindowProc( pOldWinProc, hwnd, message, wParam, lParam );
}


void CALLBACK USBabout (void)
{
	MessageBox( NULL, "nuvee ps2 usb", "About", NULL );
}


int CALLBACK USBtest (void)
{
	return 0;
}

	
// custom HID - dual lightguns
void USBhandleHID( LPARAM in_device_handle )
{
	if( rawinput_active )
		add_to_raw_mouse_x_and_y( (HRAWINPUT) in_device_handle );
}


// command-line support
char name1[512], name2[512];
void CALLBACK USBcmdline( char *cmdline )
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


		if( strcmp( name1, "-nuvee_guncon1_profile" ) == 0 ) {
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

			else if( strcmp( name2, "guncon2" ) == 0 ) {
				cmdline_port_type[0] = PAD_TYPE_GUNCON;
			}
		}


		else if( strcmp( name1, "-nuvee_port2" ) == 0 ) {
			if( strcmp( name2, "none" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_NONE;
			}

			else if( strcmp( name2, "mouse" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_MOUSE;
			}

			else if( strcmp( name2, "guncon2" ) == 0 ) {
				cmdline_port_type[1] = PAD_TYPE_GUNCON;
			}
		}
	}
}


int CALLBACK USBfreeze(int mode, freezeData *data) {
	USBfreezeData *usbd;


	switch( mode ) {
		case FREEZE_LOAD:
			if (data->size != sizeof(USBfreezeData)) {
				USB_LOG( "Freeze load failure!\n" );

				return -1;
			}


			usbd = (USBfreezeData*) (data->data);

			if( usbd->version != 2 ) {
				USB_LOG( "Freeze load failure = incorrect version (%d)\n", usbd->version );

				return -1;
			}

			memcpy( usbregs, usbd->usbregs, sizeof(usbregs) );
			memcpy( usbdevice, usbd->usbdevice, sizeof(usbdevice) );

			USB_LOG( "Freeze load\n" );


			USB_ResetRoothub(0);
			break;


		case FREEZE_SAVE:
			usbd = (USBfreezeData*) (data->data);

			usbd->version = 2;
			memcpy( usbd->usbregs, usbregs, sizeof(usbregs) );
			memcpy( usbd->usbdevice, usbdevice, sizeof(usbdevice) );

			USB_LOG( "Freeze save\n" );
			break;

	
		case FREEZE_SIZE:
			data->size = sizeof( USBfreezeData );
			break;
	}

	return 0;
}


void CALLBACK USBsetSettingsDir( const char *dir ) {
	return;
	
	strcpy( ini_path, "inis" );

	if( dir != NULL )
		strcpy( ini_path, dir );
}


USBhandler CALLBACK USBirqHandler(void)
{
	// Pass our handler to pcsx2.
	return (USBhandler) _USBirqHandler;
}


void CALLBACK USBsetRAM( void *mem )
{
	//USB_LOG( "RAM = %X\n", ram );

	ram = (u8*) mem;
}


u32 CALLBACK PS2EgetLibType( void )
{
	return PS2E_LT_USB;
}


const char* CALLBACK PS2EgetLibName( void )
{
	return PS2_LibraryName;
}


u32 CALLBACK PS2EgetLibVersion2( int type )
{
	if (type == PS2E_LT_USB )
		return (PS2E_USB_VERSION<<16) | PS2_VERSION;


	return 0;
}


int CALLBACK USBinit()
{
	USBReset();	
	return 0;
}


void CALLBACK USBshutdown( void )
{
}


void CALLBACK USBconfigure (void)
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "logs\\nuvee-log.txt", "w" );
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "USBconfigure\n" );
#endif


	// HID acquire
	hWnd_app = GetActiveWindow();


	DialogBox( hInstance_app, MAKEINTRESOURCE (IDD_MAIN), GetActiveWindow(), (DLGPROC) MainDlgProc );
}


int CALLBACK USBopen( HWND hWnd )
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "logs\\nuvee-log.txt", "w" );
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "USBopen\n" );
#endif


	init_done++;
	if( init_done > 1 ) return 0;


	// SSSPSX
	if (!IsWindow (hWnd) && !IsBadReadPtr ((u32*)hWnd, 4))
		hWnd = *(HWND*)hWnd;
	if (!IsWindow (hWnd))
		hWnd = NULL;
	else
	{
		while( GetWindowLong( hWnd, GWL_HWNDPARENT ) )
			hWnd = GetParent( hWnd );
	}

	hWnd_app = hWnd;



	// load config files
	LoadConfig_Main();


	// PCSX2 procedure - this hWnd works (GS window)
	Init_RawInput();
	Add_RawInput( hWnd_app );

	Init_DInput( hInstance_app, hWnd_app );



	// must do now - GUID detection
	Reset_Pad();


	if( port_type[0] == PAD_TYPE_MOUSE || port_type[1] == PAD_TYPE_MOUSE ||
			port_type[0] == PAD_TYPE_GUNCON || port_type[1] == PAD_TYPE_GUNCON )
		SnapCursor();

	return 0;
}


void CALLBACK USBclose (void)
{
#ifdef DEBUG_API
	if( fp_debug == 0 ) fp_debug = fopen( "logs\\nuvee-log.txt", "w" );
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );
	fprintf( fp_debug, "USBclose\n" );
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


void CALLBACK USBsetLogDir( const char* dir )
{
}


BOOL APIENTRY DllMain( HMODULE hInst, DWORD dwReason, LPVOID lpReserved )
{
	hInstance_app = hInst;
	return TRUE;
}
