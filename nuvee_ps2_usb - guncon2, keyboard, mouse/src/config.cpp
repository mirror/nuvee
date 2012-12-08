#include <stdio.h>


#include "config-guncon2.cpp"
#include "config-mouse.cpp"


void LoadConfig_Main()
{
	FILE *fp;


	fp = fopen( GetIniPath( "nuvee_ps2_usb_main.ini" ), "r" );
	if( fp ) {
		fscanf( fp, "PORT1 = %d\n", &port_type[0] );
		fscanf( fp, "PORT2 = %d\n", &port_type[1] );

		fscanf( fp, "HIDMOUSE_API = %d\n", &hidmouse_api );
		fscanf( fp, "Debug = %d\n", &usb_logging );
		fscanf( fp, "Swap lightguns = %d\n", &lightgun_swap );
		fscanf( fp, "Snap mouse = %d\n", &snap_mouse_cursor );


		fclose( fp );
	}
}


void SaveConfig_Main()
{
	FILE *fp;

	
	fp = fopen( GetIniPath( "nuvee_ps2_usb_main.ini" ), "w" );
	if( fp ) {
		fprintf( fp, "PORT1 = %d\n", port_type[0] );
		fprintf( fp, "PORT2 = %d\n", port_type[1] );

		fprintf( fp, "HIDMOUSE_API = %d\n", hidmouse_api );
		fprintf( fp, "Debug = %d\n", usb_logging );
		fprintf( fp, "Swap lightguns = %d\n", lightgun_swap );
		fprintf( fp, "Snap mouse = %d\n", snap_mouse_cursor );

		fclose( fp );
	}
}


void GetConfig_Main( HWND hWnd )
{
	HWND hWC;


	hWC = GetDlgItem( hWnd,IDC_MAIN_PORT1_CONTROLLER );
	port_type[0] = ComboBox_GetCurSel( hWC );


	hWC = GetDlgItem( hWnd,IDC_MAIN_PORT2_CONTROLLER );
	port_type[1] = ComboBox_GetCurSel( hWC );


	usb_logging = 0;
	if( IsDlgButtonChecked( hWnd,IDC_MAIN_USB_LOGGING ) )
		usb_logging = 1;


	lightgun_swap = 0;
	if( IsDlgButtonChecked( hWnd,IDC_MAIN_SWAP_GUNS ) )
		lightgun_swap = 1;



	hWC = GetDlgItem( hWnd,IDC_MAIN_HIDMOUSE_API );
	hidmouse_api = ComboBox_GetCurSel( hWC );



	snap_mouse_cursor = 1;
	if( IsDlgButtonChecked( hWnd,IDC_MAIN_SNAPMOUSE ) == 0 )
		snap_mouse_cursor = 0;
}


BOOL CALLBACK MainDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	HWND hWC;

	switch (msg)
	{
	case WM_INITDIALOG:
		LoadConfig_Main();


		hWC = GetDlgItem( hWnd, IDC_MAIN_PORT1_CONTROLLER );
		ComboBox_AddString( hWC, "None" );
		ComboBox_AddString( hWC, "Guncon 2" );
		ComboBox_AddString( hWC, "Keyboard" );
		ComboBox_AddString( hWC, "Mouse" );
		ComboBox_SetCurSel( hWC, port_type[0] );



		hWC = GetDlgItem( hWnd, IDC_MAIN_PORT2_CONTROLLER );
		ComboBox_AddString( hWC, "None" );
		ComboBox_AddString( hWC, "Guncon 2" );
		ComboBox_AddString( hWC, "Keyboard" );
		ComboBox_AddString( hWC, "Mouse" );
		ComboBox_SetCurSel( hWC, port_type[1] );


		if( usb_logging )
			CheckDlgButton( hWnd, IDC_MAIN_USB_LOGGING, TRUE );

		if( lightgun_swap )
			CheckDlgButton( hWnd, IDC_MAIN_SWAP_GUNS, TRUE );



		hWC = GetDlgItem( hWnd, IDC_MAIN_HIDMOUSE_API );
		ComboBox_AddString( hWC, "RawInput" );
		ComboBox_AddString( hWC, "DirectInput" );
		ComboBox_SetCurSel( hWC, hidmouse_api );



		if( snap_mouse_cursor )
			CheckDlgButton( hWnd, IDC_MAIN_SNAPMOUSE, TRUE );

		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_MAIN_OK:
			GetConfig_Main( hWnd );
			SaveConfig_Main();

			EndDialog( hWnd,TRUE );
			return TRUE;


		case ID_MAIN_CANCEL:
			EndDialog( hWnd,FALSE );
			return TRUE;


		case IDC_MAIN_PORT1_CONFIG:
			pad_active = 1;


			GetConfig_Main( hWnd );


			switch( port_type[0] ) {
				case PAD_TYPE_GUNCON:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_GUNCON),
						GetActiveWindow(),(DLGPROC) GunconDlgProc );
					break;


				case PAD_TYPE_KEYBOARD:
					MessageBox( hWnd, "Nothing to configure for now", "Keyboard 1", MB_OK );
					break;


				case PAD_TYPE_MOUSE:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_MOUSE),
						GetActiveWindow(),(DLGPROC) MouseDlgProc );
					break;

			}
			
			return TRUE;


		case IDC_MAIN_PORT2_CONFIG:
			pad_active = 2;


			GetConfig_Main( hWnd );


			switch( port_type[1] ) {
				case PAD_TYPE_GUNCON:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_GUNCON),
						GetActiveWindow(),(DLGPROC) GunconDlgProc );
					break;


				case PAD_TYPE_KEYBOARD:
					MessageBox( hWnd, "Nothing to configure for now", "Keyboard 1", MB_OK );
					break;


				case PAD_TYPE_MOUSE:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_MOUSE),
						GetActiveWindow(),(DLGPROC) MouseDlgProc );
					break;

			}
			
			return TRUE;


		case IDC_MAIN_SWAP_GUNS:
			// flag freeze load to reattach devices
			USB_ResetRoothub(1);
			return TRUE;


		case IDC_MAIN_RECONNECT:
			// flag freeze load to reattach devices
			USB_ResetRoothub(1);
			return TRUE;


		case IDC_MAIN_HELP_HIDMOUSE_API:
			MessageBox( NULL,
				"nuvee requires a Win32 HID or serial mouse device\r\r\rRawInput:\rPreferred mode for real PC mice + lightguns\r- (XP) Needed for dual mouse - lightgun emulation\r\rex. USB - PS/2 mouse, ActLabs - EMS TopGun - Smog GunCon2PC lightguns\r\r\r\rDirectInput:\rCompatible mode for practically any mouse device\r- (XP) Only 1 mouse device allowed\r\rex. WiiMouse",
				"HID Mouse API", MB_OK );
			return TRUE;
		} // end switch
	}

	return FALSE;
}
