#include <stdio.h>


//"plugins\\nuvee_psx_mouse2.ini"
void LoadConfig_Mouse( char *file_name )
{
	FILE *fp;


	fp = fopen( file_name, "r" );
	if( fp ) {
		fscanf( fp, "Sensitivity = %d\n", &mouse_sensitivity[ THIS_PAD ] );
		fscanf( fp, "Threshold = %d\n", &mouse_threshold[ THIS_PAD ] );
		fscanf( fp, "Deadzone = %d\n", &mouse_deadzone[ THIS_PAD ] );

		fscanf( fp, "GUID = %s\n", &mouse_name[ THIS_PAD ] );
		fscanf( fp, "USB mouse = %d\n", &mouse_usb_converter[ THIS_PAD ] );

		fclose( fp );
	}




	// check for device id
	device_hid[ THIS_PAD ] = 0;


	if( hidmouse_api == HIDMOUSE_API_RAWINPUT )
	{
		for( int lcv = 1; lcv < raw_mouse_count(); lcv++ ) {
			if( strcmp( mouse_name[ THIS_PAD ], get_raw_mouse_name(lcv) ) == 0 ) {
				device_hid[ THIS_PAD ] = lcv;
				break;
			}
		}
	}

	else if( hidmouse_api == HIDMOUSE_API_DIRECTINPUT )
	{
		for( int lcv = 1; lcv < di_mouse_count; lcv++ ) {
			if( strcmp( mouse_name[ THIS_PAD ], di_mouse_guid[lcv] ) == 0 ) {
				device_hid[ THIS_PAD ] = lcv;
				break;
			}
		}
	}



	// check joysticks (either API)
	for( int lcv = 0; lcv < di_joystick_count; lcv++ ) {
		if( strcmp( mouse_name[ THIS_PAD ], di_joystick_guid[lcv] ) == 0 ) {
			device_hid[ THIS_PAD ] = lcv + 20;
			break;
		}
	}
}


void SaveConfig_Mouse( char *file_name )
{
	FILE *fp;

	
	fp = fopen( file_name, "w" );
	if( fp ) {
		fprintf( fp, "Sensitivity = %d\n", mouse_sensitivity[ THIS_PAD ] );
		fprintf( fp, "Threshold = %d\n", mouse_threshold[ THIS_PAD ] );
		fprintf( fp, "Deadzone = %d\n", mouse_deadzone[ THIS_PAD ] );

		if( mouse_name[ THIS_PAD ][0] )
			fprintf( fp, "GUID = %s\n", mouse_name[ THIS_PAD ] );
		else
			fprintf( fp, "GUID = SysMouse\n" );

		fprintf( fp, "USB mouse = %d\n", mouse_usb_converter[ THIS_PAD ] );

		fclose( fp );
	}
}


DWORD WINAPI get_mouse_hid_device()
{
	int lcv;


	if( pad_active == 1 ) {
		LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse on game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Mouse 1 - Acquire", MB_OK );
	}
	else {
		LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse on game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Mouse 2 - Acquire", MB_OK );
	}


	device_hid[ THIS_PAD ] = 0;

		

	// Joystick usage
	if( di_joystick_count > 0 &&
			IDYES == MessageBox( NULL,
				"Will you be using a gamepad as your mouse?",
				"Joystick attached", MB_YESNO ) )
	{
		int done;

	
		// wait for button de-click
		while(1) {
			bool stop;

			Poll_Joystick_DInput();

			stop = 1;
			for( int lcv = 0; lcv < di_joystick_count; lcv++ )
			{
				if( device_buttons[ lcv+20 ][0] ||
						device_buttons[ lcv+20 ][1] ||
						device_buttons[ lcv+20 ][2] || 
						device_buttons[ lcv+20 ][3] ||
						device_buttons[ lcv+20 ][4] )
				{
					stop = 0;

					Sleep(1);
					break;
				}
			}

			if( stop ) break;
		}



		done = 0;
		while( done == 0 ) {
			Poll_Joystick_DInput();

			// check joysticks FIRST! (PPJoy + GlovePIE)
			for( lcv = 0; lcv < di_joystick_count; lcv++ )
			{
				// check for active device
				if( device_buttons[ lcv+20 ][0] ||
						device_buttons[ lcv+20 ][1] ||
						device_buttons[ lcv+20 ][2] ||
						device_buttons[ lcv+20 ][3] ||
						device_buttons[ lcv+20 ][4] )
				{
					done = 1;


					device_hid[ THIS_PAD ] = lcv + 20;


					strcpy( mouse_name[ THIS_PAD ], di_joystick_guid[ lcv ] );
					break;
				}
			}

			Sleep(1);
		}
	}


	// RawInput usage
	else if( rawinput_active && hidmouse_api == HIDMOUSE_API_RAWINPUT )
	{
		int done;


		for( lcv = 0; lcv < raw_mouse_count(); lcv++ )
			reset_raw_mouse_data(lcv);


		done = 0;
		while( done == 0 )
		{
			// include SysMouse (non-raw devices)
			for( lcv = raw_mouse_count()-1; lcv >= 0; lcv-- )
			{
				// check for active device
				if( is_raw_mouse_button_pressed(lcv,0) ||
						is_raw_mouse_button_pressed(lcv,1) ||
						is_raw_mouse_button_pressed(lcv,2) )
				{
					done = 1;

					device_hid[ THIS_PAD ] = lcv;


					if( lcv > 0 )
						strcpy( mouse_name[ THIS_PAD ], get_raw_mouse_name(lcv) );
					else
						strcpy( mouse_name[ THIS_PAD ], "RawInput SysMouse" );
					break;
				}
			}

			Sleep(1);
		}
	}


	// DirectInput
	else if( di_mouse_count > 0 && hidmouse_api == HIDMOUSE_API_DIRECTINPUT )
	{
		int done;

		// wait for button de-click
		while(1) {
			Poll_Mouse_DInput();

			if( device_buttons[0][0] ||
					device_buttons[0][1] ||
					device_buttons[0][2] )
			{
				Sleep(1);
				continue;
			}

			break;
		}


		done = 0;
		while( done == 0 )
		{
			// include SysMouse (non-raw devices)
			for( lcv = di_mouse_count-1; lcv >= 0; lcv-- )
			{
				Poll_Mouse_DInput();


				// check for active device
				if( device_buttons[ lcv ][0] ||
						device_buttons[ lcv ][1] ||
						device_buttons[ lcv ][2] )
				{
					done = 1;

					device_hid[ THIS_PAD ] = lcv;


					if( lcv > 0 )
						strcpy( mouse_name[ THIS_PAD ], di_mouse_guid[ lcv ] );
					else
						strcpy( mouse_name[ THIS_PAD ], "DInput SysMouse" );
					break;
				}


				Sleep(1);
			}
		} // end wait device
	}



	if( pad_active == 1 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Mouse 1 - Acquire", MB_OK );


		SaveConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );
	}
	else {
		MessageBox( NULL,
			"Device found - ready to use",
			"Mouse 2 - Acquire", MB_OK );


		SaveConfig_Mouse( GetIniPath( "nuvee_psx_mouse2.ini" ) );
	}

	return 0;
}


void GetConfig_Mouse( HWND hWnd )
{
	HWND hWC;
	char str[256];


	hWC = GetDlgItem( hWnd,IDC_MOUSE_SENSITIVITY );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &mouse_sensitivity[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_MOUSE_THRESHOLD );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &mouse_threshold[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_MOUSE_DEADZONE );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &mouse_deadzone[ THIS_PAD ] );



	// check special hack
	mouse_usb_converter[ THIS_PAD ] = 0;
	if( IsDlgButtonChecked( hWnd,IDC_MOUSE_USB_CONVERTER_HACK ) )
		mouse_usb_converter[ THIS_PAD ] = 1;
}


BOOL CALLBACK MouseDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	HWND hWC;
	char str[256];


	switch (msg)
	{
	case WM_INITDIALOG:
		Init_RawInput();
		Add_RawInput( hWnd_app );

		Init_DInput( hInstance_app, hWnd );



		if( pad_active == 1 ) {
			SetWindowText( hWnd, "Mouse - Port 1" );


			LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );
		}
		else {
			SetWindowText( hWnd, "Mouse - Port 2" );


			LoadConfig_Mouse( GetIniPath( "nuvee_psx_mouse2.ini" ) );
		}



		hWC = GetDlgItem( hWnd, IDC_MOUSE_SENSITIVITY );
		sprintf( str, "%d", mouse_sensitivity[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_MOUSE_THRESHOLD );
		sprintf( str, "%d", mouse_threshold[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_MOUSE_DEADZONE );
		sprintf( str, "%d", mouse_deadzone[ THIS_PAD ] );
		SetWindowText( hWC, str );

		// check special hack
		if( mouse_usb_converter[ THIS_PAD ] )
			CheckDlgButton( hWnd, IDC_MOUSE_USB_CONVERTER_HACK, TRUE );

		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_MOUSE_OK:
			GetConfig_Mouse( hWnd );


			if( pad_active == 1 )
				SaveConfig_Mouse( GetIniPath( "nuvee_psx_mouse1.ini" ) );
			else
				SaveConfig_Mouse( GetIniPath( "nuvee_psx_mouse2.ini" ) );



			Remove_RawInput( hWnd_app );
			Done_DInput();


			EndDialog( hWnd,TRUE );
			return TRUE;


		case ID_MOUSE_CANCEL:
			Remove_RawInput( hWnd );
			Done_DInput();


			EndDialog( hWnd,FALSE );
			return TRUE;


		case IDC_MOUSE_HELP1:
			MessageBox( hWnd,
				"Controls how much the mouse responds to movement\r- 100 = normal\r- 50 = half\r- 0-999 = limit",
				"Sensitivity", MB_OK );
			return TRUE;


		case IDC_MOUSE_HELP2:
			MessageBox( hWnd,
				"Sets an upper limit on how much the mouse can move\r- 128 = normal\r- 0-128 = limit",
				"Threshold", MB_OK );
			return TRUE;


		case IDC_MOUSE_HELP3:
			MessageBox( hWnd,
				"Sets how large the idle position sits\r- 0 = normal\r- 0-128 = limit",
				"Deadzone", MB_OK );
			return TRUE;


		case IDC_MOUSE_ACQUIRE:
			{
				CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)get_mouse_hid_device, NULL, 0, 0 );

				return TRUE;
			}
		}
	}

	return FALSE;
}
