#include <stdio.h>


char ps1_justifier_default_config[] = "\
; cmdline_id  game_name\n\
; sensitivity x-y  center x-y\n\
; model  width-height\n\
\n\
\n\
\n\
";


char ps2_justifier_default_config[] = "\
; cmdline_id  game_name\n\
; sensitivity x-y  center x-y\n\
; model  width-height\n\
\n\
\n\
\n\
";


void LoadConfig_Justifier_Presets( char *file_name )
{
	FILE *fp;


	justifier_num_profiles = 1;

	strcpy( justifier_profile_id[0], "custom" );
	strcpy( justifier_profile_name[0], "(-- Custom --)  use aiming values" );
		
	justifier_profile_aiming_x[0] = 97.625;
	justifier_profile_aiming_y[0] = 94.625;
	justifier_profile_center_x[0] = 274;
	justifier_profile_center_y[0] = 168;


	fp = fopen( file_name, "r" );


	// create file
	if( !fp ) {
		fp = fopen( file_name, "w" );
			
		if( emu_type == 1 )
			fprintf( fp, "%s", ps1_justifier_default_config );
		else if( emu_type == 2 )
			fprintf( fp, "%s", ps2_justifier_default_config );

		fclose(fp);


		fp = fopen( file_name, "r" );
	}


	if( fp ) {
		char line[1024], str[1024];
		int lcv1,lcv2;


		while( !feof(fp) ) {
			fgets( line, 1024, fp );
			if( feof(fp) ) break;

			if( line[0] == ';' ) continue;
			if( line[0] == 0x0d || line[0] == 0x0a ) continue;
			if( line[0] == 0 ) continue;


			sscanf( line, "%s",
				&justifier_profile_id[ justifier_num_profiles ] );




			for( lcv1 = 0; lcv1 < strlen(line); lcv1++ ) {
				if( line[lcv1] == '"' ) break;
			}
			lcv1++;

			for( lcv2 = lcv1; lcv2 < strlen(line); lcv2++ ) {
				if( line[lcv2] == '"' ) break;
			}
			line[lcv2] = 0;

			strcpy( justifier_profile_name[ justifier_num_profiles ], line + lcv1 );




			fgets( line, 1024, fp );
			sscanf( line, "%f %f %d %d\n",
				&justifier_profile_aiming_x[ justifier_num_profiles ],
				&justifier_profile_aiming_y[ justifier_num_profiles ],
				&justifier_profile_center_x[ justifier_num_profiles ],
				&justifier_profile_center_y[ justifier_num_profiles ] );



			fgets( line, 1024, fp );
			sscanf( line, "%s %d %d\n",
				&str,
				&justifier_profile_width[ justifier_num_profiles ],
				&justifier_profile_height[ justifier_num_profiles ] );


			justifier_profile_model[ justifier_num_profiles ] = JUSTIFIER_MODEL_HOMEBREW;
			if( strcmp( str, "homebrew" ) == 0 )
				justifier_profile_model[ justifier_num_profiles ] = JUSTIFIER_MODEL_HOMEBREW;
			else if( strcmp( str, "custom" ) == 0 )
				justifier_profile_model[ justifier_num_profiles ] = JUSTIFIER_MODEL_CUSTOM;
			else if( strcmp( str, "unlicensed" ) == 0 )
				justifier_profile_model[ justifier_num_profiles ] = JUSTIFIER_MODEL_UNLICENSED;
			else if( strcmp( str, "konami" ) == 0 )
				justifier_profile_model[ justifier_num_profiles ] = JUSTIFIER_MODEL_KONAMI;



			{
				int lcv2;


				// throw out id dupes
				lcv2 = 1;
				while( lcv2 < justifier_num_profiles ) {
					if( strcmp( justifier_profile_id[ justifier_num_profiles ], justifier_profile_id[ lcv2 ] ) == 0 )
						break;

					lcv2++;
				}


				// pass
				if( lcv2 == justifier_num_profiles )
					justifier_num_profiles++;
			}
		}

		fclose( fp );
	}



	for( int lcv = 1; lcv < justifier_num_profiles; lcv++ ) {
		if( strcmp( justifier_aiming_id[ THIS_PAD ], justifier_profile_id[lcv] ) == 0 ) {
			justifier_alignment[ THIS_PAD ] = lcv;

			// crosshair alignment
			justifier_screen_center_x[ THIS_PAD ] = justifier_profile_center_x[ lcv ];
			justifier_screen_center_y[ THIS_PAD ] = justifier_profile_center_y[ lcv ];
			justifier_scale_x[ THIS_PAD ] = justifier_profile_aiming_x[ lcv ];
			justifier_scale_y[ THIS_PAD ] = justifier_profile_aiming_y[ lcv ];

			justifier_aiming_model[ THIS_PAD ] = justifier_profile_model[ lcv ];
			justifier_width[ THIS_PAD ] = justifier_profile_width[ lcv ];
			justifier_height[ THIS_PAD ] = justifier_profile_height[ lcv ];

			break;
		}
	}
}



void LoadConfig_Justifier( char *file_name )
{
	FILE *fp;


	fp = fopen( file_name, "r" );
	if( fp ) {
		fscanf( fp, "Sensitivity = %f\n", &justifier_sensitivity[ THIS_PAD ] );
		fscanf( fp, "Threshold = %d\n", &justifier_threshold[ THIS_PAD ] );
		fscanf( fp, "Deadzone = %d\n", &justifier_deadzone[ THIS_PAD ] );

		fscanf( fp, "Left = %d\n", &justifier_left[ THIS_PAD ] );
		fscanf( fp, "Right = %d\n", &justifier_right[ THIS_PAD ] );
		fscanf( fp, "Middle = %d\n", &justifier_middle[ THIS_PAD ] );

		fscanf( fp, "Reload = %d\n", &justifier_reload[ THIS_PAD ] );
		fscanf( fp, "Calibration = %d\n", &justifier_calibrate[ THIS_PAD ] );
		fscanf( fp, "Cursor = %d\n", &justifier_cursor[ THIS_PAD ] );

		fscanf( fp, "GUID = %s\n", &justifier_guid_name[ THIS_PAD ] );

		fscanf( fp, "Lightgun_left = %d\n", &justifier_lightgun_left[ THIS_PAD ] );
		fscanf( fp, "Lightgun_top = %d\n", &justifier_lightgun_top[ THIS_PAD ] );
		fscanf( fp, "Lightgun_right = %d\n", &justifier_lightgun_right[ THIS_PAD ] );
		fscanf( fp, "Lightgun_bottom = %d\n", &justifier_lightgun_bottom[ THIS_PAD ] );

		fscanf( fp, "Model = %d\n", &justifier_model[ THIS_PAD ] );
		fscanf( fp, "Alignment = %d\n", &justifier_alignment[ THIS_PAD ] );

		fscanf( fp, "Aiming scale X = %f\n", &justifier_aiming_x[ THIS_PAD ] );
		fscanf( fp, "Aiming scale Y = %f\n", &justifier_aiming_y[ THIS_PAD ] );
		fscanf( fp, "Aiming profile = %s\n", &justifier_aiming_id[ THIS_PAD ] );

		fclose( fp );
	}


	// check for device id
	device_hid[ THIS_PAD ] = 0;

	for( int lcv = 1; lcv < raw_mouse_count(); lcv++ ) {
		if( strcmp( justifier_guid_name[ THIS_PAD ], get_raw_mouse_name(lcv) ) == 0 ) {
			device_hid[ THIS_PAD ] = lcv;
			break;
		}
	}



	// check justifier profile
	justifier_alignment[ THIS_PAD ] = 0;

	justifier_scale_x[ THIS_PAD ] = justifier_aiming_x[ THIS_PAD ];
	justifier_scale_y[ THIS_PAD ] = justifier_aiming_y[ THIS_PAD ];
	justifier_screen_center_x[ THIS_PAD ] = justifier_profile_center_x[ 0 ];
	justifier_screen_center_y[ THIS_PAD ] = justifier_profile_center_y[ 0 ];

	justifier_aiming_model[ THIS_PAD ] = justifier_model[ THIS_PAD ];
	justifier_width[ THIS_PAD ] = 384;
	justifier_height[ THIS_PAD ] = 256;




	// override
	if( cmdline_justifier_id[ THIS_PAD ][0] != 0 )
		strcpy( justifier_aiming_id[ THIS_PAD ], cmdline_justifier_id[ THIS_PAD ] );

	if( cmdline_justifier_model[ THIS_PAD ] != -1 )
		justifier_model[ THIS_PAD ] = cmdline_justifier_model[ THIS_PAD ];




	for( int lcv = 1; lcv < justifier_num_profiles; lcv++ ) {
		if( strcmp( justifier_aiming_id[ THIS_PAD ], justifier_profile_id[lcv] ) == 0 ) {
			justifier_alignment[ THIS_PAD ] = lcv;


			// crosshair alignment
			justifier_screen_center_x[ THIS_PAD ] = justifier_profile_center_x[ lcv ];
			justifier_screen_center_y[ THIS_PAD ] = justifier_profile_center_y[ lcv ];
			justifier_scale_x[ THIS_PAD ] = justifier_profile_aiming_x[ lcv ];
			justifier_scale_y[ THIS_PAD ] = justifier_profile_aiming_y[ lcv ];

			justifier_aiming_model[ THIS_PAD ] = justifier_profile_model[ lcv ];
			justifier_width[ THIS_PAD ] = justifier_profile_width[ lcv ];
			justifier_height[ THIS_PAD ] = justifier_profile_height[ lcv ];

			break;
		}
	}
}


void SaveConfig_Justifier( char *file_name )
{
	FILE *fp;


	fp = fopen( file_name, "w" );
	if( fp ) {
		fprintf( fp, "Sensitivity = %f\n", justifier_sensitivity[ THIS_PAD ] );
		fprintf( fp, "Threshold = %d\n", justifier_threshold[ THIS_PAD ] );
		fprintf( fp, "Deadzone = %d\n", justifier_deadzone[ THIS_PAD ] );

		fprintf( fp, "Left = %d\n", justifier_left[ THIS_PAD ] );
		fprintf( fp, "Right = %d\n", justifier_right[ THIS_PAD ] );
		fprintf( fp, "Middle = %d\n", justifier_middle[ THIS_PAD ] );

		fprintf( fp, "Reload = %d\n", justifier_reload[ THIS_PAD ] );
		fprintf( fp, "Calibration = %d\n", justifier_calibrate[ THIS_PAD ] );
		fprintf( fp, "Cursor = %d\n", justifier_cursor[ THIS_PAD ] );

		fprintf( fp, "GUID = %s\n", justifier_guid_name[ THIS_PAD ] );

		fprintf( fp, "Lightgun_left = %d\n", justifier_lightgun_left[ THIS_PAD ] );
		fprintf( fp, "Lightgun_top = %d\n", justifier_lightgun_top[ THIS_PAD ] );
		fprintf( fp, "Lightgun_right = %d\n", justifier_lightgun_right[ THIS_PAD ] );
		fprintf( fp, "Lightgun_bottom = %d\n", justifier_lightgun_bottom[ THIS_PAD ] );

		fprintf( fp, "Model = %d\n", justifier_model[ THIS_PAD ] );
		fprintf( fp, "Alignment = %d\n", justifier_alignment[ THIS_PAD ] );

		fprintf( fp, "Aiming scale X = %f\n", justifier_aiming_x[ THIS_PAD ] );
		fprintf( fp, "Aiming scale Y = %f\n", justifier_aiming_y[ THIS_PAD ] );
		fprintf( fp, "Aiming profile = %s\n", justifier_aiming_id[ THIS_PAD ] );

		fclose( fp );
	}
}


DWORD WINAPI get_justifier_hid_device()
{
	int lcv, reset_data;
	int calibrate_check;


	if( !rawinput_active ) {
		MessageBox( NULL,
			"No RawInput API found - only system mouse is allowed",
			"Acquire", MB_OK );

		return 0;
	}


	if( pad_active == 1 ) {
		LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier1.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse or lightgun you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse or fire trigger at game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Justifier 1 - Acquire", MB_OK );
	}
	else if( pad_active == 2 ) {
		LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier2.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse or lightgun you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse or fire trigger at game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Justifier 2 - Acquire", MB_OK );
	}


	device_hid[ THIS_PAD ] = 0;

		
	for( lcv = 0; lcv < raw_mouse_count(); lcv++ )
		reset_raw_mouse_data(lcv);


	while( device_hid[ THIS_PAD ] == 0 )
	{
		for( lcv = 1; lcv < raw_mouse_count(); lcv++ )
		{
			// check for active device
			if( is_raw_mouse_button_pressed(lcv,0) ||
					is_raw_mouse_button_pressed(lcv,1) ||
					is_raw_mouse_button_pressed(lcv,2) )
			{
				device_hid[ THIS_PAD ] = lcv;

				strcpy( justifier_guid_name[ THIS_PAD ], get_raw_mouse_name(lcv) );
				break;
			}
		}

		Sleep(1);
	}



	calibrate_check = 0;

	if(	device_absolute[ device_hid[ THIS_PAD ] ] )
	{
		if( pad_active == 1 ) {
			if( IDOK == MessageBox( NULL, "Ready to calibrate the device\r\r1- Close box\r2- Wave gun around past all edges of screen - left/right/top/bottom\r3- Press gun button when finished\r\r\rTO SKIP THIS TEST:\r- Click cancel",
							"Justifier 1 - Calibration", MB_OKCANCEL ) ) {
				calibrate_check = 1;
			}
		}
		else if( pad_active == 2 ) {
			if( IDOK == MessageBox( NULL, "Ready to calibrate the device\r\r1- Close box\r2- Wave gun around past all edges of screen - left/right/top/bottom\r3- Press gun button when finished\r\r\rTO SKIP THIS TEST:\r- Click cancel",
							"Justifier 2 - Calibration", MB_OKCANCEL ) ) {
				calibrate_check = 1;
			}
		}
	}


	if( calibrate_check )
	{
		reset_raw_mouse_data( device_hid[ THIS_PAD ] );


		reset_data = 0;
		while(1) {
			int x,y;

			// check for active device
			if( is_raw_mouse_button_pressed(device_hid[ THIS_PAD ],0) ||
					is_raw_mouse_button_pressed(device_hid[ THIS_PAD ],1) ||
					is_raw_mouse_button_pressed(device_hid[ THIS_PAD ],2) ) {
				break;
			}


			// read data (16-bit unsigned absolute values)
			x = get_raw_mouse_x_delta( device_hid[ THIS_PAD ] );
			y = get_raw_mouse_y_delta( device_hid[ THIS_PAD ] );


			// let system rest
			Sleep(1);


			// ignore offscreen data
			if( x == 0 || y == 0 || x == 65535 || y == 65535 )
				continue;


			if( reset_data == 0 ) {
				reset_data = 1;

				justifier_lightgun_left[ THIS_PAD ] = 65534;
				justifier_lightgun_top[ THIS_PAD ] = 65534;
				justifier_lightgun_right[ THIS_PAD ] = 1;
				justifier_lightgun_bottom[ THIS_PAD ] = 1;
			}


			// look for screen edges
			if( justifier_lightgun_left[ THIS_PAD ] > x ) justifier_lightgun_left[ THIS_PAD ] = x;
			if( justifier_lightgun_right[ THIS_PAD ] < y ) justifier_lightgun_right[ THIS_PAD ] = y;
			if( justifier_lightgun_top[ THIS_PAD ] > x ) justifier_lightgun_top[ THIS_PAD ] = x;
			if( justifier_lightgun_bottom[ THIS_PAD ] < y ) justifier_lightgun_bottom[ THIS_PAD ] = y;
		}
	}


	if( pad_active == 1 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Justifier 1 - Acquire", MB_OK );


		SaveConfig_Justifier( GetIniPath( "nuvee_psx_justifier1.ini" ) );
	}
	else if( pad_active == 2 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Justifier 2 - Acquire", MB_OK );


		SaveConfig_Justifier( GetIniPath( "nuvee_psx_justifier2.ini" ) );
	}

	return 0;
}



void GetConfig_Justifier( HWND hWnd )
{
	HWND hWC;
	char str[256];


	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_SENSITIVITY );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &justifier_sensitivity[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_THRESHOLD );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_threshold[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_DEADZONE );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_deadzone[ THIS_PAD ] );



	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_LEFT );
	justifier_left[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_RIGHT );
	justifier_right[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_MIDDLE );
	justifier_middle[ THIS_PAD ] = ComboBox_GetCurSel( hWC );



	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_RELOAD );
	justifier_reload[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_ALIGNMENT );
	justifier_alignment[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_CURSOR );
	justifier_cursor[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_MODEL );
	justifier_model[ THIS_PAD ] = ComboBox_GetCurSel( hWC );




	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_LIGHTGUN_LEFT );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_lightgun_left[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_LIGHTGUN_TOP );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_lightgun_top[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_LIGHTGUN_RIGHT );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_lightgun_right[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_LIGHTGUN_BOTTOM );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &justifier_lightgun_bottom[ THIS_PAD ] );



	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_AIMING_X );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &justifier_aiming_x[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_JUSTIFIER_AIMING_Y );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &justifier_aiming_y[ THIS_PAD ] );



	// update profile id
	strcpy( justifier_aiming_id[ THIS_PAD ], justifier_profile_id[ justifier_alignment[ THIS_PAD ] ] );
}


BOOL CALLBACK JustifierDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	HWND hWC;
	char str[256];


	switch (msg)
	{
	case WM_INITDIALOG:
		LoadConfig_Justifier_Presets( GetIniPath( "nuvee_psx_justifier_profiles.ini" ) );


		if( pad_active == 1 ) {
			SetWindowText( hWnd, "Justifier - Port 1" );


			LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier1.ini" ) );
		}
		else {
			SetWindowText( hWnd, "Justifier - Port 2" );


			LoadConfig_Justifier( GetIniPath( "nuvee_psx_justifier2.ini" ) );
		}


		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_SENSITIVITY );
		sprintf( str, "%f", justifier_sensitivity[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_THRESHOLD );
		sprintf( str, "%d", justifier_threshold[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_DEADZONE );
		sprintf( str, "%d", justifier_deadzone[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_MODEL );
		sprintf( str, "%d", justifier_model[ THIS_PAD ] );
		SetWindowText( hWC, str );



		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_LEFT );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "Aux" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_SetCurSel( hWC, justifier_left[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_RIGHT );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "Aux" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_SetCurSel( hWC, justifier_right[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_MIDDLE );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "Aux" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_SetCurSel( hWC, justifier_middle[ THIS_PAD ] );



		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_RELOAD );
		ComboBox_AddString( hWC, "Manual" );
		ComboBox_AddString( hWC, "Semi" );
		ComboBox_SetCurSel( hWC, justifier_reload[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_CURSOR );
		ComboBox_AddString( hWC, "Visible" );
		ComboBox_AddString( hWC, "Hidden" );
		ComboBox_SetCurSel( hWC, justifier_cursor[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_MODEL );
		ComboBox_AddString( hWC, "Homebrew" );
		ComboBox_AddString( hWC, "Custom" );
		ComboBox_AddString( hWC, "Unlicensed" );
		ComboBox_AddString( hWC, "Konami (N/A)" );
		ComboBox_SetCurSel( hWC, justifier_model[ THIS_PAD ] );




		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_LIGHTGUN_LEFT );
		sprintf( str, "%d", justifier_lightgun_left[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_LIGHTGUN_TOP );
		sprintf( str, "%d", justifier_lightgun_top[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_LIGHTGUN_RIGHT );
		sprintf( str, "%d", justifier_lightgun_right[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_LIGHTGUN_BOTTOM );
		sprintf( str, "%d", justifier_lightgun_bottom[ THIS_PAD ] );
		SetWindowText( hWC, str );



		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_ALIGNMENT );

		for( int lcv = 0; lcv < justifier_num_profiles; lcv++ ) {
			ComboBox_AddString( hWC, justifier_profile_name[lcv] );
		}

		ComboBox_SetCurSel( hWC, justifier_alignment[ THIS_PAD ] );


		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_AIMING_X );
		sprintf( str, "%f", justifier_aiming_x[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_AIMING_Y );
		sprintf( str, "%f", justifier_aiming_y[ THIS_PAD ] );
		SetWindowText( hWC, str );

		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_JUSTIFIER_OK:
			GetConfig_Justifier( hWnd );


			if( pad_active == 1 )
				SaveConfig_Justifier( GetIniPath( "nuvee_psx_justifier1.ini" ) );
			else
				SaveConfig_Justifier( GetIniPath( "nuvee_psx_justifier2.ini" ) );


			EndDialog( hWnd,TRUE );
			return TRUE;


		case ID_JUSTIFIER_CANCEL:
			EndDialog( hWnd,FALSE );
			return TRUE;


		case IDC_JUSTIFIER_HELP1:
			MessageBox( hWnd,
				"Controls how much the device responds to movement\r- 100 = normal\r- 50 = half\r- 0-999 = limit",
				"Sensitivity", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP2:
			MessageBox( hWnd,
				"Sets an upper limit on how much the mouse can move\r- 512 = normal\r- 0-512 = limit",
				"Threshold", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP3:
			MessageBox( hWnd,
				"Sets how large the idle position sits\r- 0 = normal\r- 0-512 = limit",
				"Deadzone", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP4:
			MessageBox( hWnd,
				"Specify the method to reload the gun\r- Manual = offscreen shot (move mouse outside of screen, then shoot)\r\r- Semi = move mouse outside of screen (auto-reload shot)",
				"Reload", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP_ALIGNMENT:
			MessageBox( hWnd,
				"NOTE:\rLightgun works best at 640x480\r\r\rTune visual crosshairs for each game\r- makes visual crosshair + shots more precise\r\r- At higher resolutions, you may have to calibrate slightly __off-center__ to get better shot markers\r\rCustom aiming values\r- If your shots overshoot, set a higher aiming x-y value\r- If your shots undershoot, set a lower aiming x-y value\r\rSample X = 94.5, 97.5, 98.5, 100.0\rSample Y = 94.5, 94.625, 100.0\r\r\rAuto-calibration (mouse only)\r- 1. Go to the gun calibration screen\r- 2. Hold down trigger\r- 3. Roll mouse wheel up-down-up-down-up-down\r- 4. Release trigger\r- 5. Fire a test shot to finish gun centering",
				"Alignment", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP6:
			MessageBox( hWnd,
				"NOTE:\rLightgun works best at 640x480\r\r\rPC lightguns use an edge coordinate system\r- acquire device to calibrate and adjust if needed",
				"Gun edges", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_HELP_MODEL:
			MessageBox( hWnd,
				"Some games will only work with certain Justifier model parameters\r\r\r1- Homebrew = home-made type gun, should work well also\r\r2- Custom = works with many USA games; best version to try first\r\r3- Unlicensed = attempts to mimic a 3rd party lightgun; try this if custom / homebrew fails\r\r4- Konami = authentic lightgun (the best choice); correct values currently unknown",
				"Model", MB_OK );
			return TRUE;


		case IDC_JUSTIFIER_ACQUIRE:
			{
				CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)get_justifier_hid_device, NULL, 0, 0 );

				return TRUE;
			}


		case IDC_JUSTIFIER_EDIT_PROFILES:
			{
				STARTUPINFO         siStartupInfo;
				PROCESS_INFORMATION piProcessInfo;

				memset(&siStartupInfo, 0, sizeof(siStartupInfo));
				memset(&piProcessInfo, 0, sizeof(piProcessInfo));

				siStartupInfo.cb = sizeof(siStartupInfo);

				strcpy( temp_str, "c:\\windows\\notepad.exe \"" );
				strcat( temp_str, ini_path );
				strcat( temp_str, "\\nuvee_psx_justifier_profiles.ini\"" );
				if( CreateProcessA(NULL,
					temp_str,
					0,0,FALSE,0,0,0,
					&siStartupInfo, &piProcessInfo) == TRUE )
				{
			    WaitForSingleObject( piProcessInfo.hProcess, INFINITE );
		      CloseHandle( piProcessInfo.hProcess );
	        CloseHandle( piProcessInfo.hThread );


					// reload profiles
					LoadConfig_Justifier_Presets( GetIniPath( "nuvee_psx_justifier_profiles.ini" ) );
					
					

					hWC = GetDlgItem( hWnd, IDC_JUSTIFIER_ALIGNMENT );

					while( ComboBox_DeleteString( hWC, 0 ) )
					{};

					for( int lcv = 0; lcv < justifier_num_profiles; lcv++ ) {
						ComboBox_AddString( hWC, justifier_profile_name[lcv] );
					}

					ComboBox_SetCurSel( hWC, justifier_alignment[ THIS_PAD ] );
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}
