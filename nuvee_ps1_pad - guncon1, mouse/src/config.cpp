#include <stdio.h>


#include "config-mouse.cpp"
#include "config-guncon.cpp"
#include "config-justifier.cpp"


void LoadConfig_Main()
{
	FILE *fp;


	fp = fopen( GetIniPath( "nuvee_psx_main.ini" ), "r" );
	if( fp ) {
		fscanf( fp, "PORT1 = %d\n", &port_type[0] );
		fscanf( fp, "PORT2 = %d\n", &port_type[1] );
		fscanf( fp, "HIDMOUSE_API = %d\n", &hidmouse_api );
		fscanf( fp, "Snap mouse = %d\n", &snap_mouse_cursor );
		fscanf( fp, "Offscreen shot = %d\n", &offscreen_shot );

		fclose( fp );
	}


	// override
	if( cmdline_port_type[0] != -1 ) port_type[0] = cmdline_port_type[0];
	if( cmdline_port_type[1] != -1 ) port_type[1] = cmdline_port_type[1];
}



void SaveConfig_Main()
{
	FILE *fp;

	
	fp = fopen( GetIniPath( "nuvee_psx_main.ini" ), "w" );
	if( fp ) {
		fprintf( fp, "PORT1 = %d\n", port_type[0] );
		fprintf( fp, "PORT2 = %d\n", port_type[1] );
		fprintf( fp, "HIDMOUSE_API = %d\n", hidmouse_api );
		fprintf( fp, "Snap mouse = %d\n", snap_mouse_cursor );
		fprintf( fp, "Offscreen shot = %d\n", offscreen_shot );

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


	// PCSX1 - None, Mouse, Guncon, Justifier, Multitap
	// PCSX2 adjustment - None, Guncon, Justifier, Mouse
	if( emu_type == 2 ) {
		// Guncon (PS2) -> Guncon (normal)
		if( port_type[0] == 1 ) port_type[0] = PAD_TYPE_GUNCON;
		else if( port_type[0] == 2 ) port_type[0] = PAD_TYPE_MOUSE;
		else if( port_type[0] == 3 ) port_type[0] = PAD_TYPE_JUSTIFIER;


		// Guncon (PS2) -> Guncon (normal)
		if( port_type[1] == 1 ) port_type[1] = PAD_TYPE_GUNCON;
		else if( port_type[1] == 2 ) port_type[1] = PAD_TYPE_MOUSE;
		else if( port_type[1] == 3 ) port_type[1] = PAD_TYPE_JUSTIFIER;
	}



	hWC = GetDlgItem( hWnd,IDC_MAIN_HIDMOUSE_API );
	hidmouse_api = ComboBox_GetCurSel( hWC );



	snap_mouse_cursor = 1;
	if( IsDlgButtonChecked( hWnd,IDC_MAIN_SNAPMOUSE ) == 0 )
		snap_mouse_cursor = 0;


	offscreen_shot = 1;
	if( IsDlgButtonChecked( hWnd,IDC_MAIN_OFFSCREEN ) == 1 )
		offscreen_shot = 0;
}


BOOL CALLBACK MainDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	HWND hWC;


	switch (msg)
	{
	case WM_INITDIALOG:
		LoadConfig_Main();


		if( emu_type == 1 ) {
			hWC = GetDlgItem( hWnd, IDC_MAIN_PORT1_CONTROLLER );
			ComboBox_AddString( hWC, "None" );
			ComboBox_AddString( hWC, "Mouse" );
			ComboBox_AddString( hWC, "Guncon" );
			ComboBox_AddString( hWC, "Justifier (N/A)" );
			ComboBox_AddString( hWC, "Multitap (N/A)" );
			ComboBox_SetCurSel( hWC, port_type[0] );


			hWC = GetDlgItem( hWnd, IDC_MAIN_PORT2_CONTROLLER );
			ComboBox_AddString( hWC, "None" );
			ComboBox_AddString( hWC, "Mouse" );
			ComboBox_AddString( hWC, "Guncon" );
			ComboBox_AddString( hWC, "Justifier (N/A)" );
			ComboBox_AddString( hWC, "Multitap (N/A)" );
			ComboBox_SetCurSel( hWC, port_type[1] );
		}

		else if( emu_type == 2 ) {
			hWC = GetDlgItem( hWnd, IDC_MAIN_PORT1_CONTROLLER );
			ComboBox_AddString( hWC, "None" );
			ComboBox_AddString( hWC, "Guncon" );
			ComboBox_AddString( hWC, "Mouse" );
			ComboBox_AddString( hWC, "Justifier (N/A)" );
			ComboBox_SetCurSel( hWC, port_type[0] );

			if( port_type[0] == PAD_TYPE_GUNCON ) ComboBox_SetCurSel( hWC, 1 );
			if( port_type[0] == PAD_TYPE_MOUSE ) ComboBox_SetCurSel( hWC, 2 );
			if( port_type[0] == PAD_TYPE_JUSTIFIER ) ComboBox_SetCurSel( hWC, 3 );



			hWC = GetDlgItem( hWnd, IDC_MAIN_PORT2_CONTROLLER );
			ComboBox_AddString( hWC, "None" );
			ComboBox_AddString( hWC, "Guncon" );
			ComboBox_AddString( hWC, "Mouse" );
			ComboBox_AddString( hWC, "Justifier (N/A)" );
			ComboBox_SetCurSel( hWC, port_type[1] );

			if( port_type[1] == PAD_TYPE_GUNCON ) ComboBox_SetCurSel( hWC, 1 );
			if( port_type[1] == PAD_TYPE_MOUSE ) ComboBox_SetCurSel( hWC, 2 );
			if( port_type[1] == PAD_TYPE_JUSTIFIER ) ComboBox_SetCurSel( hWC, 3 );
		}


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
				case PAD_TYPE_MOUSE:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_MOUSE),
						GetActiveWindow(),(DLGPROC) MouseDlgProc );
					break;


				case PAD_TYPE_GUNCON:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_GUNCON),
						GetActiveWindow(),(DLGPROC) GunconDlgProc );
					break;


				case PAD_TYPE_JUSTIFIER:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_JUSTIFIER),
						GetActiveWindow(),(DLGPROC) JustifierDlgProc );
					break;
			}

			
			return TRUE;


		case IDC_MAIN_PORT2_CONFIG:
			pad_active = 2;


			GetConfig_Main( hWnd );


			switch( port_type[1] ) {				
				case PAD_TYPE_MOUSE:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_MOUSE),
						GetActiveWindow(),(DLGPROC) MouseDlgProc );
					break;


				case PAD_TYPE_GUNCON:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_GUNCON),
						GetActiveWindow(),(DLGPROC) GunconDlgProc );
					break;


				case PAD_TYPE_JUSTIFIER:
					DialogBox( hInstance_app, MAKEINTRESOURCE(IDD_JUSTIFIER),
						GetActiveWindow(),(DLGPROC) JustifierDlgProc );
					break;
			}

			return TRUE;



		case IDC_MAIN_HELP_HIDMOUSE_API:
			MessageBox( NULL,
				"nuvee requires a Win32 HID or serial mouse device\r\r\rRawInput:\rPreferred mode for real PC mice + lightguns\r- (XP) Needed for dual mouse - lightgun emulation\r\rex. USB - PS/2 mouse, ActLabs - EMS TopGun - Smog GunCon2PC lightguns\r\r\r\rDirectInput:\rCompatible mode for practically any mouse device\r- (XP) Only 1 mouse device allowed\r\rex. WiiMouse",
				"HID Mouse API", MB_OK );
			return TRUE;
		}
	}

	return FALSE;
}
