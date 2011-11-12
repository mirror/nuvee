#include <stdio.h>


#define GUNCON2_INI "3_00"

char ps2_guncon2_default_config[] = "\
; cmdline_id  game_name\n\
; sensitivity x-y  center x-y\n\
; model  width-height\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
dino_stalker \"Dino Stalker (U)\"\n\
90.25 92.5 390 132\n\
namco 640 240\n\
\n\
\n\
\n\
; 480i\n\
\n\
endgame \"Endgame (U)\"\n\
89.25 93.5 422 141\n\
namco 640 240\n\
\n\
\n\
\n\
; 480i, 480p\n\
; - (mouse only) use calibration hack\n\
;\n\
; NOTE: Aim a little to the left of 'X' for calibration\n\
\n\
guncom2 \"(*) Guncom 2 (E)\"\n\
90.5 114.75 390 146\n\
namco 640 256\n\
\n\
\n\
\n\
; 480i\n\
\n\
gunfighter2 \"Gunfighter 2 - Jesse James (E)\"\n\
84.5 89.0 456 164\n\
namco 640 256\n\
\n\
\n\
\n\
; 480i\n\
; - (mouse only) use calibration hack\n\
\n\
gunvari_i \"Gunvari Collection (J) (480i)\"\n\
90.25 98.0 390 138\n\
namco 640 240\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
gunvari \"Gunvari Collection (J) (480p)\"\n\
86.75 96.0 454 164\n\
namco 640 256\n\
\n\
\n\
\n\
; 480i, 480p\n\
;\n\
; NOTE: Aim a little to the left of center for calibration\n\
\n\
ninja_assault_e \"(*) Ninja Assault (E)\"\n\
90.25 94.5 390 169\n\
namco 640 256\n\
\n\
\n\
\n\
; 480i, 480p\n\
; - (mouse only) use calibration hack\n\
;\n\
; NOTE: Aim a little to the left of center for calibration\n\
\n\
ninja_assault \"(*) Ninja Assault (U)\"\n\
90.25 92.5 390 132\n\
namco 640 240\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
;\n\
; - Calibration: hold down trigger after each shot and keep gun still\n\
\n\
re_survivor2 \"(*) Resident Evil Survivor 2 (E)\"\n\
84.75 96.0 454 164\n\
namco 640 240\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
re_deadaim \"Resident Evil - Dead Aim (U)\"\n\
90.25 93.5 420 132\n\
namco 640 240\n\
\n\
\n\
\n\
; 480i\n\
; - (mouse only) use calibration hack\n\
; - options -> controller 2 = g-con 2 (shooting)\n\
\n\
starsky_hutch \"Starsky & Hutch (U)\"\n\
90.25 91.75 453 154\n\
namco 640 256\n\
\n\
\n\
\n\
; 480i, 480p\n\
; - (mouse only) use calibration hack\n\
\n\
time_crisis2 \"Time Crisis 2 (U)\"\n\
90.25 97.5 390 154\n\
namco 640 240\n\
\n\
\n\
\n\
; 480i, 480p\n\
; - (mouse only) use calibration hack\n\
\n\
time_crisis3 \"Time Crisis 3 (U)\"\n\
90.25 97.5 390 154\n\
namco 640 240\n\
\n\
\n\
\n\
; 480i\n\
; - (mouse only) use calibration hack\n\
\n\
time_crisis_zone_ui \"Time Crisis - Crisis Zone (U) (480i)\"\n\
90.25 99.0 390 153\n\
namco 640 240\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
time_crisis_zone \"Time Crisis - Crisis Zone (U) (480p)\"\n\
94.5 104.75 423 407\n\
namco 768 768\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
vampire_night \"Vampire Night (U)\"\n\
97.5 104.75 423 407\n\
namco 768 768\n\
\n\
\n\
\n\
; 480i\n\
; - (mouse only) use calibration hack\n\
\n\
virtua_cop_ei \"Virtua Cop - Elite Edition (E,J) (480i)\"\n\
88.75 100.0 454 164\n\
namco 640 256\n\
\n\
\n\
\n\
; 480p\n\
; - (mouse only) use calibration hack\n\
\n\
virtua_cop \"Virtua Cop - Elite Edition (E,J) (480p)\"\n\
85.75 92.0 456 164\n\
namco 640 256\n\
\n\
\n\
\n\
";


void LoadConfig_Guncon_Presets( char *file_name )
{
	FILE *fp;


	guncon_num_profiles = 1;

	strcpy( guncon_profile_id[0], "custom" );
	strcpy( guncon_profile_name[0], "(-- Custom --)  use aiming values" );
		
	guncon_profile_aiming_x[0] = 97.625;
	guncon_profile_aiming_y[0] = 94.625;
	guncon_profile_center_x[0] = 274;
	guncon_profile_center_y[0] = 168;


	fp = fopen( file_name, "r" );


	// check INI version
	if( fp ) {
		char line[1024], str[1024];
		int lcv1,lcv2;


		if( 1 != fscanf( fp, "VERSION = %s", &str ) ) {
			fclose( fp );
			fp = 0;
		}


		else if( strcmp( str, GUNCON2_INI ) != 0 ) {
			fclose( fp );
			fp = 0;
		}
	}


	// create file
	if( !fp ) {
		char str[1024];


		fp = fopen( file_name, "w" );

		if( fp ) {
			fprintf( fp, "VERSION = %s\n\n", GUNCON2_INI );
			fprintf( fp, "%s", ps2_guncon2_default_config );

			fclose(fp);


			fp = fopen( file_name, "r" );
			if( fp ) fgets( str, 1024, fp );
		}
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
				&guncon_profile_id[ guncon_num_profiles ] );




			for( lcv1 = 0; lcv1 < strlen(line); lcv1++ ) {
				if( line[lcv1] == '"' ) break;
			}
			lcv1++;

			for( lcv2 = lcv1; lcv2 < strlen(line); lcv2++ ) {
				if( line[lcv2] == '"' ) break;
			}
			line[lcv2] = 0;

			strcpy( guncon_profile_name[ guncon_num_profiles ], line + lcv1 );




			fgets( line, 1024, fp );
			sscanf( line, "%f %f %d %d\n",
				&guncon_profile_aiming_x[ guncon_num_profiles ],
				&guncon_profile_aiming_y[ guncon_num_profiles ],
				&guncon_profile_center_x[ guncon_num_profiles ],
				&guncon_profile_center_y[ guncon_num_profiles ] );



			fgets( line, 1024, fp );
			sscanf( line, "%s %d %d\n",
				&str,
				&guncon_profile_width[ guncon_num_profiles ],
				&guncon_profile_height[ guncon_num_profiles ] );


			guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_NAMCO;




			str[0] = 0;
			fgets( line, 1024, fp );
			sscanf( line, "%s\n",
				&str );


			guncon_profile_swap[ guncon_num_profiles ] = 0;
			if( strcmp( str, "swap_guns" ) == 0 )
				guncon_profile_swap[ guncon_num_profiles ] = 1;



			{
				int lcv2;


				// throw out id dupes
				lcv2 = 1;
				while( lcv2 < guncon_num_profiles ) {
					if( strcmp( guncon_profile_id[ guncon_num_profiles ], guncon_profile_id[ lcv2 ] ) == 0 )
						break;

					lcv2++;
				}


				// pass
				if( lcv2 == guncon_num_profiles )
					guncon_num_profiles++;
			}
		}

		fclose( fp );
	}



	for( int lcv = 1; lcv < guncon_num_profiles; lcv++ ) {
		if( strcmp( guncon_aiming_id[ THIS_PAD ], guncon_profile_id[lcv] ) == 0 ) {
			guncon_alignment[ THIS_PAD ] = lcv;

			// crosshair alignment
			guncon_screen_center_x[ THIS_PAD ] = guncon_profile_center_x[ lcv ];
			guncon_screen_center_y[ THIS_PAD ] = guncon_profile_center_y[ lcv ];
			guncon_scale_x[ THIS_PAD ] = guncon_profile_aiming_x[ lcv ];
			guncon_scale_y[ THIS_PAD ] = guncon_profile_aiming_y[ lcv ];

			guncon_aiming_model[ THIS_PAD ] = guncon_profile_model[ lcv ];
			guncon_width[ THIS_PAD ] = guncon_profile_width[ lcv ];
			guncon_height[ THIS_PAD ] = guncon_profile_height[ lcv ];


			if( guncon_profile_swap[ lcv ] ) {
				lightgun_swap = 1;
			}

			break;
		}
	}
}



void LoadConfig_Guncon( char *file_name )
{
	FILE *fp;


	fp = fopen( file_name, "r" );
	if( fp ) {
		fscanf( fp, "Sensitivity = %f\n", &guncon_sensitivity[ THIS_PAD ] );
		fscanf( fp, "Threshold = %d\n", &guncon_threshold[ THIS_PAD ] );
		fscanf( fp, "Deadzone = %d\n", &guncon_deadzone[ THIS_PAD ] );

		fscanf( fp, "Left = %d\n", &guncon_left[ THIS_PAD ] );
		fscanf( fp, "Right = %d\n", &guncon_right[ THIS_PAD ] );
		fscanf( fp, "Middle = %d\n", &guncon_middle[ THIS_PAD ] );

		fscanf( fp, "Reload = %d\n", &guncon_reload[ THIS_PAD ] );
		fscanf( fp, "Calibration = %d\n", &guncon_calibrate[ THIS_PAD ] );
		fscanf( fp, "Cursor = %d\n", &guncon_cursor[ THIS_PAD ] );

		fscanf( fp, "GUID = %s\n", &guncon_guid_name[ THIS_PAD ] );

		fscanf( fp, "Lightgun_left = %d\n", &guncon_lightgun_left[ THIS_PAD ] );
		fscanf( fp, "Lightgun_top = %d\n", &guncon_lightgun_top[ THIS_PAD ] );
		fscanf( fp, "Lightgun_right = %d\n", &guncon_lightgun_right[ THIS_PAD ] );
		fscanf( fp, "Lightgun_bottom = %d\n", &guncon_lightgun_bottom[ THIS_PAD ] );

		fscanf( fp, "Model = %d\n", &guncon_model[ THIS_PAD ] );
		fscanf( fp, "Alignment = %d\n", &guncon_alignment[ THIS_PAD ] );

		fscanf( fp, "Aiming scale X = %f\n", &guncon_aiming_x[ THIS_PAD ] );
		fscanf( fp, "Aiming scale Y = %f\n", &guncon_aiming_y[ THIS_PAD ] );
		fscanf( fp, "Aiming profile = %s\n", &guncon_aiming_id[ THIS_PAD ] );

		fscanf( fp, "Aux 1 = %d\n", &guncon_aux1[ THIS_PAD ] );
		fscanf( fp, "Aux 2 = %d\n", &guncon_aux2[ THIS_PAD ] );
		fscanf( fp, "Wheel up = %d\n", &guncon_wheelup[ THIS_PAD ] );
		fscanf( fp, "Wheel down = %d\n", &guncon_wheeldown[ THIS_PAD ] );

		fscanf( fp, "Keyboard D-pad = %d\n", &guncon_keyboard_dpad[ THIS_PAD ] );
		fscanf( fp, "Start hotkey = %d\n", &guncon_start_hotkey[ THIS_PAD ] );

		fclose( fp );
	}


	// check for device id
	device_hid[ THIS_PAD ] = 0;


	if( hidmouse_api == HIDMOUSE_API_RAWINPUT )
	{
		for( int lcv = 1; lcv < raw_mouse_count(); lcv++ ) {
			if( strcmp( guncon_guid_name[ THIS_PAD ], get_raw_mouse_name(lcv) ) == 0 ) {
				device_hid[ THIS_PAD ] = lcv;
				break;
			}
		}
	}

	else if( hidmouse_api == HIDMOUSE_API_DIRECTINPUT )
	{
		for( int lcv = 1; lcv < di_mouse_count; lcv++ ) {
			if( strcmp( guncon_guid_name[ THIS_PAD ], di_mouse_guid[lcv] ) == 0 ) {
				device_hid[ THIS_PAD ] = lcv;
				break;
			}
		}
	}


	// check joysticks (either API)
	for( int lcv = 0; lcv < di_joystick_count; lcv++ ) {
		if( strcmp( guncon_guid_name[ THIS_PAD ], di_joystick_guid[lcv] ) == 0 ) {
			device_hid[ THIS_PAD ] = lcv + 20;
			break;
		}
	}



	// check guncon profile
	guncon_alignment[ THIS_PAD ] = 0;

	guncon_scale_x[ THIS_PAD ] = guncon_aiming_x[ THIS_PAD ];
	guncon_scale_y[ THIS_PAD ] = guncon_aiming_y[ THIS_PAD ];
	guncon_screen_center_x[ THIS_PAD ] = guncon_profile_center_x[ 0 ];
	guncon_screen_center_y[ THIS_PAD ] = guncon_profile_center_y[ 0 ];

	guncon_aiming_model[ THIS_PAD ] = guncon_model[ THIS_PAD ];
	guncon_width[ THIS_PAD ] = 384;
	guncon_height[ THIS_PAD ] = 256;




	// override
	if( cmdline_guncon_id[ THIS_PAD ][0] != 0 )
		strcpy( guncon_aiming_id[ THIS_PAD ], cmdline_guncon_id[ THIS_PAD ] );

	if( cmdline_guncon_model[ THIS_PAD ] != -1 )
		guncon_model[ THIS_PAD ] = cmdline_guncon_model[ THIS_PAD ];




	for( int lcv = 1; lcv < guncon_num_profiles; lcv++ ) {
		if( strcmp( guncon_aiming_id[ THIS_PAD ], guncon_profile_id[lcv] ) == 0 ) {
			guncon_alignment[ THIS_PAD ] = lcv;

			// crosshair alignment
			guncon_screen_center_x[ THIS_PAD ] = guncon_profile_center_x[ lcv ];
			guncon_screen_center_y[ THIS_PAD ] = guncon_profile_center_y[ lcv ];
			guncon_scale_x[ THIS_PAD ] = guncon_profile_aiming_x[ lcv ];
			guncon_scale_y[ THIS_PAD ] = guncon_profile_aiming_y[ lcv ];

			guncon_aiming_model[ THIS_PAD ] = guncon_profile_model[ lcv ];
			guncon_width[ THIS_PAD ] = guncon_profile_width[ lcv ];
			guncon_height[ THIS_PAD ] = guncon_profile_height[ lcv ];

			break;
		}
	}
}


void SaveConfig_Guncon( char *file_name )
{
	FILE *fp;


	fp = fopen( file_name, "w" );
	if( fp ) {
		fprintf( fp, "Sensitivity = %f\n", guncon_sensitivity[ THIS_PAD ] );
		fprintf( fp, "Threshold = %d\n", guncon_threshold[ THIS_PAD ] );
		fprintf( fp, "Deadzone = %d\n", guncon_deadzone[ THIS_PAD ] );

		fprintf( fp, "Left = %d\n", guncon_left[ THIS_PAD ] );
		fprintf( fp, "Right = %d\n", guncon_right[ THIS_PAD ] );
		fprintf( fp, "Middle = %d\n", guncon_middle[ THIS_PAD ] );

		fprintf( fp, "Reload = %d\n", guncon_reload[ THIS_PAD ] );
		fprintf( fp, "Calibration = %d\n", guncon_calibrate[ THIS_PAD ] );
		fprintf( fp, "Cursor = %d\n", guncon_cursor[ THIS_PAD ] );

		if( guncon_guid_name[ THIS_PAD ][0] ) {
			fprintf( fp, "GUID = %s\n", guncon_guid_name[ THIS_PAD ] );
		}
		else
			fprintf( fp, "GUID = SysMouse\n" );

		fprintf( fp, "Lightgun_left = %d\n", guncon_lightgun_left[ THIS_PAD ] );
		fprintf( fp, "Lightgun_top = %d\n", guncon_lightgun_top[ THIS_PAD ] );
		fprintf( fp, "Lightgun_right = %d\n", guncon_lightgun_right[ THIS_PAD ] );
		fprintf( fp, "Lightgun_bottom = %d\n", guncon_lightgun_bottom[ THIS_PAD ] );

		fprintf( fp, "Model = %d\n", guncon_model[ THIS_PAD ] );
		fprintf( fp, "Alignment = %d\n", guncon_alignment[ THIS_PAD ] );

		fprintf( fp, "Aiming scale X = %f\n", guncon_aiming_x[ THIS_PAD ] );
		fprintf( fp, "Aiming scale Y = %f\n", guncon_aiming_y[ THIS_PAD ] );
		fprintf( fp, "Aiming profile = %s\n", guncon_aiming_id[ THIS_PAD ] );

		fprintf( fp, "Aux 1 = %d\n", guncon_aux1[ THIS_PAD ] );
		fprintf( fp, "Aux 2 = %d\n", guncon_aux2[ THIS_PAD ] );
		fprintf( fp, "Wheel up = %d\n", guncon_wheelup[ THIS_PAD ] );
		fprintf( fp, "Wheel down = %d\n", guncon_wheeldown[ THIS_PAD ] );

		fprintf( fp, "Keyboard D-pad = %d\n", guncon_keyboard_dpad[ THIS_PAD ] );
		fprintf( fp, "Start hotkey = %d\n", guncon_start_hotkey[ THIS_PAD ] );

		fclose( fp );
	}
}


DWORD WINAPI get_guncon_hid_device()
{
	int lcv, reset_data;
	int calibrate_check;


	if( pad_active == 1 ) {
		LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse or lightgun you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse or fire trigger at game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Guncon 1 - Acquire", MB_OK );
	}
	else if( pad_active == 2 ) {
		LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse or lightgun you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse or fire trigger at game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Guncon 2 - Acquire", MB_OK );
	}


	device_hid[ THIS_PAD ] = 0;

		



	// Joystick usage
	if( di_joystick_count > 0 &&
			IDYES == MessageBox( NULL,
				"Will you be using a gamepad as your Guncon?",
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

					strcpy( guncon_guid_name[ THIS_PAD ], di_joystick_guid[ lcv ] );
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
			for( lcv = raw_mouse_count()-1; lcv >= 1; lcv-- )
			{
				// check for active device
				if( is_raw_mouse_button_pressed(lcv,0) ||
						is_raw_mouse_button_pressed(lcv,1) ||
						is_raw_mouse_button_pressed(lcv,2) )
				{
					done = 1;


					device_hid[ THIS_PAD ] = lcv;


					if( lcv > 0 )
						strcpy( guncon_guid_name[ THIS_PAD ], get_raw_mouse_name(lcv) );
					else
						strcpy( guncon_guid_name[ THIS_PAD ], "RawInput SysMouse" );
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
						strcpy( guncon_guid_name[ THIS_PAD ], di_mouse_guid[ lcv ] );
					else
						strcpy( guncon_guid_name[ THIS_PAD ], "DInput SysMouse" );
					break;
				}


				Sleep(1);
			}
		} // end wait device
	}



	calibrate_check = 0;

	if(	device_absolute[ device_hid[ THIS_PAD ] ] )
	{
		if( pad_active == 1 ) {
			if( IDOK == MessageBox( NULL, "Ready to calibrate the device\r\r1- Close box\r2- Wave gun around past all edges of screen - left/right/top/bottom\r3- Press gun button when finished\r\r\rTO SKIP THIS TEST:\r- Click cancel",
							"Guncon 1 - Calibration", MB_OKCANCEL ) ) {
				calibrate_check = 1;
			}
		}
		else if( pad_active == 2 ) {
			if( IDOK == MessageBox( NULL, "Ready to calibrate the device\r\r1- Close box\r2- Wave gun around past all edges of screen - left/right/top/bottom\r3- Press gun button when finished\r\r\rTO SKIP THIS TEST:\r- Click cancel",
							"Guncon 2 - Calibration", MB_OKCANCEL ) ) {
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

				guncon_lightgun_left[ THIS_PAD ] = 65534;
				guncon_lightgun_top[ THIS_PAD ] = 65534;
				guncon_lightgun_right[ THIS_PAD ] = 1;
				guncon_lightgun_bottom[ THIS_PAD ] = 1;
			}


			// look for screen edges
			if( guncon_lightgun_left[ THIS_PAD ] > x ) guncon_lightgun_left[ THIS_PAD ] = x;
			if( guncon_lightgun_right[ THIS_PAD ] < y ) guncon_lightgun_right[ THIS_PAD ] = y;
			if( guncon_lightgun_top[ THIS_PAD ] > x ) guncon_lightgun_top[ THIS_PAD ] = x;
			if( guncon_lightgun_bottom[ THIS_PAD ] < y ) guncon_lightgun_bottom[ THIS_PAD ] = y;
		}
	}


	if( pad_active == 1 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Guncon 1 - Acquire", MB_OK );


		SaveConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );
	}
	else if( pad_active == 2 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Guncon 2 - Acquire", MB_OK );


		SaveConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );
	}

	return 0;
}



void GetConfig_Guncon( HWND hWnd )
{
	HWND hWC;
	char str[256];


	hWC = GetDlgItem( hWnd,IDC_GUNCON_SENSITIVITY );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &guncon_sensitivity[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_GUNCON_THRESHOLD );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_threshold[ THIS_PAD ] );


	hWC = GetDlgItem( hWnd,IDC_GUNCON_DEADZONE );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_deadzone[ THIS_PAD ] );




	hWC = GetDlgItem( hWnd,IDC_GUNCON_LEFT );
	guncon_left[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_RIGHT );
	guncon_right[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_MIDDLE );
	guncon_middle[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_AUX1 );
	guncon_aux1[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_AUX2 );
	guncon_aux2[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_WHEELUP );
	guncon_wheelup[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_WHEELDOWN );
	guncon_wheeldown[ THIS_PAD ] = ComboBox_GetCurSel( hWC );





	hWC = GetDlgItem( hWnd,IDC_GUNCON_RELOAD );
	guncon_reload[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_ALIGNMENT );
	guncon_alignment[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_CURSOR );
	guncon_cursor[ THIS_PAD ] = ComboBox_GetCurSel( hWC );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_MODEL );
	guncon_model[ THIS_PAD ] = ComboBox_GetCurSel( hWC );




	hWC = GetDlgItem( hWnd,IDC_GUNCON_LIGHTGUN_LEFT );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_lightgun_left[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_LIGHTGUN_TOP );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_lightgun_top[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_LIGHTGUN_RIGHT );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_lightgun_right[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_LIGHTGUN_BOTTOM );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%d", &guncon_lightgun_bottom[ THIS_PAD ] );



	hWC = GetDlgItem( hWnd,IDC_GUNCON_AIMING_X );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &guncon_aiming_x[ THIS_PAD ] );

	hWC = GetDlgItem( hWnd,IDC_GUNCON_AIMING_Y );
	GetWindowText( hWC,str,256 );
	sscanf( str, "%f", &guncon_aiming_y[ THIS_PAD ] );



	// check special hack
	guncon_calibrate[ THIS_PAD ] = 0;
	if( IsDlgButtonChecked( hWnd,IDC_GUNCON_CALIBRATION_HACK ) )
		guncon_calibrate[ THIS_PAD ] = 1;



	// check special hacks
	guncon_keyboard_dpad[ THIS_PAD ] = 0;
	if( IsDlgButtonChecked( hWnd,IDC_GUNCON_KEYBOARD_DPAD ) )
		guncon_keyboard_dpad[ THIS_PAD ] = 1;


	guncon_start_hotkey[ THIS_PAD ] = 0;
	if( IsDlgButtonChecked( hWnd,IDC_GUNCON_START_HOTKEY ) )
		guncon_start_hotkey[ THIS_PAD ] = 1;
	


	// update profile id
	strcpy( guncon_aiming_id[ THIS_PAD ], guncon_profile_id[ guncon_alignment[ THIS_PAD ] ] );
}


BOOL CALLBACK GunconDlgProc( const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam )
{
	HWND hWC;
	char str[256];


	switch (msg)
	{
	case WM_INITDIALOG:
		Init_RawInput();
		Add_RawInput( hWnd_app );

		Init_DInput( hInstance_app, hWnd );

		
		
		LoadConfig_Guncon_Presets( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ) );


		if( pad_active == 1 ) {
			SetWindowText( hWnd, "Guncon - Port 1" );


			LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );
		}
		else {
			SetWindowText( hWnd, "Guncon - Port 2" );


			LoadConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );
		}


		hWC = GetDlgItem( hWnd, IDC_GUNCON_SENSITIVITY );
		sprintf( str, "%f", guncon_sensitivity[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_THRESHOLD );
		sprintf( str, "%d", guncon_threshold[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_DEADZONE );
		sprintf( str, "%d", guncon_deadzone[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_MODEL );
		sprintf( str, "%d", guncon_model[ THIS_PAD ] );
		SetWindowText( hWC, str );



		hWC = GetDlgItem( hWnd, IDC_GUNCON_LEFT );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_left[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_RIGHT );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_right[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_MIDDLE );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_middle[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_AUX1 );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_aux1[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_AUX2 );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_aux2[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_WHEELUP );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_wheelup[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_WHEELDOWN );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_AddString( hWC, "C" );
		ComboBox_AddString( hWC, "Start" );
		ComboBox_AddString( hWC, "Select" );
		ComboBox_AddString( hWC, "D-Up" );
		ComboBox_AddString( hWC, "D-Down" );
		ComboBox_AddString( hWC, "D-Left" );
		ComboBox_AddString( hWC, "D-Right" );
		ComboBox_AddString( hWC, "A + Select" );
		ComboBox_AddString( hWC, "B + Select" );
		ComboBox_AddString( hWC, "D-Up + Select" );
		ComboBox_AddString( hWC, "D-Down + Select" );
		ComboBox_AddString( hWC, "D-Left + Select" );
		ComboBox_AddString( hWC, "D-Right + Select" );
		ComboBox_AddString( hWC, "None" );
		ComboBox_SetCurSel( hWC, guncon_wheeldown[ THIS_PAD ] );



		hWC = GetDlgItem( hWnd, IDC_GUNCON_RELOAD );
		ComboBox_AddString( hWC, "Manual" );
		ComboBox_AddString( hWC, "Semi" );
		ComboBox_SetCurSel( hWC, guncon_reload[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_CURSOR );
		ComboBox_AddString( hWC, "Visible" );
		ComboBox_AddString( hWC, "Hidden" );
		ComboBox_SetCurSel( hWC, guncon_cursor[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_MODEL );
		ComboBox_AddString( hWC, "Namco" );
		ComboBox_SetCurSel( hWC, guncon_model[ THIS_PAD ] );



		hWC = GetDlgItem( hWnd, IDC_GUNCON_LIGHTGUN_LEFT );
		sprintf( str, "%d", guncon_lightgun_left[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_LIGHTGUN_TOP );
		sprintf( str, "%d", guncon_lightgun_top[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_LIGHTGUN_RIGHT );
		sprintf( str, "%d", guncon_lightgun_right[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_LIGHTGUN_BOTTOM );
		sprintf( str, "%d", guncon_lightgun_bottom[ THIS_PAD ] );
		SetWindowText( hWC, str );



		hWC = GetDlgItem( hWnd, IDC_GUNCON_ALIGNMENT );

		for( int lcv = 0; lcv < guncon_num_profiles; lcv++ ) {
			ComboBox_AddString( hWC, guncon_profile_name[lcv] );
		}

		ComboBox_SetCurSel( hWC, guncon_alignment[ THIS_PAD ] );


		hWC = GetDlgItem( hWnd, IDC_GUNCON_AIMING_X );
		sprintf( str, "%f", guncon_aiming_x[ THIS_PAD ] );
		SetWindowText( hWC, str );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_AIMING_Y );
		sprintf( str, "%f", guncon_aiming_y[ THIS_PAD ] );
		SetWindowText( hWC, str );



		// check special hacks
		if( guncon_calibrate[ THIS_PAD ] )
			CheckDlgButton( hWnd, IDC_GUNCON_CALIBRATION_HACK, TRUE );

		if( guncon_keyboard_dpad[ THIS_PAD ] )
			CheckDlgButton( hWnd, IDC_GUNCON_KEYBOARD_DPAD, TRUE );

		if( guncon_start_hotkey[ THIS_PAD ] )
			CheckDlgButton( hWnd, IDC_GUNCON_START_HOTKEY, TRUE );

		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_GUNCON_OK:
			GetConfig_Guncon( hWnd );


			if( pad_active == 1 )
				SaveConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon1.ini" ) );
			else
				SaveConfig_Guncon( GetIniPath( "nuvee_ps2_usb_guncon2.ini" ) );


			Remove_RawInput( hWnd_app );
			Done_DInput();


			EndDialog( hWnd,TRUE );
			return TRUE;


		case ID_GUNCON_CANCEL:
			Remove_RawInput( hWnd_app );
			Done_DInput();


			EndDialog( hWnd,FALSE );
			return TRUE;


		case IDC_GUNCON_HELP1:
			MessageBox( hWnd,
				"Controls how much the device responds to movement\r- 100 = normal\r- 50 = half\r- 0-999 = limit",
				"Sensitivity", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP2:
			MessageBox( hWnd,
				"Sets an upper limit on how much the mouse can move\r- 512 = normal\r- 0-512 = limit",
				"Threshold", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP3:
			MessageBox( hWnd,
				"Sets how large the idle position sits\r- 0 = normal\r- 0-512 = limit",
				"Deadzone", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP4:
			MessageBox( hWnd,
				"Specify the method to reload the gun\r- Manual = offscreen shot (move mouse outside of screen, then shoot)\r\r- Semi = move mouse outside of screen (auto-reload shot)",
				"Reload", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP_ALIGNMENT:
			MessageBox( hWnd,
				"NOTE:\rLightgun works best at 640x480\r\r\rTune visual crosshairs for each game\r- makes visual crosshair + shots more precise\r\r- At higher resolutions, you may have to calibrate slightly __off-center__ to get better shot markers\r\rCustom aiming values\r- If your shots overshoot, set a higher aiming x-y value\r- If your shots undershoot, set a lower aiming x-y value\r\rSample X = 94.5, 97.5, 98.5, 100.0\rSample Y = 94.5, 94.625, 100.0",
				"Alignment", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP6:
			MessageBox( hWnd,
				"NOTE:\rLightgun works best at 640x480\r\r\rPC lightguns use an edge coordinate system\r- acquire device to calibrate and adjust if needed",
				"Gun edges", MB_OK );
			return TRUE;


		case IDC_GUNCON_ACQUIRE:
			{
				CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)get_guncon_hid_device, NULL, 0, 0 );

				return TRUE;
			}


		case IDC_GUNCON_EDIT_PROFILES:
			{
				STARTUPINFO         siStartupInfo;
				PROCESS_INFORMATION piProcessInfo;

				memset(&siStartupInfo, 0, sizeof(siStartupInfo));
				memset(&piProcessInfo, 0, sizeof(piProcessInfo));

				siStartupInfo.cb = sizeof(siStartupInfo);

				strcpy( temp_str, "c:\\windows\\notepad.exe \"" );
				strcat( temp_str, ini_path );
				strcat( temp_str, "\\nuvee_ps2_usb_guncon_profiles.ini\"" );
				if( CreateProcessA(NULL,
					temp_str,
					0,0,FALSE,0,0,0,
					&siStartupInfo, &piProcessInfo) == TRUE )
				{
			    WaitForSingleObject( piProcessInfo.hProcess, INFINITE );
		      CloseHandle( piProcessInfo.hProcess );
	        CloseHandle( piProcessInfo.hThread );


					// reload profiles
					LoadConfig_Guncon_Presets( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ) );
					
					

					hWC = GetDlgItem( hWnd, IDC_GUNCON_ALIGNMENT );

					while( ComboBox_DeleteString( hWC, 0 ) )
					{};

					for( int lcv = 0; lcv < guncon_num_profiles; lcv++ ) {
						ComboBox_AddString( hWC, guncon_profile_name[lcv] );
					}

					ComboBox_SetCurSel( hWC, guncon_alignment[ THIS_PAD ] );
				}
			}
			return TRUE;


		case IDC_GUNCON_HELP_CALIBRATION_HACK:
			MessageBox( hWnd,
				"NOTE:\rGuncon 2 flashes a _BLACK_ screen which is currently not emulated\r- Requires reading pixels from GSdx + seeing VBlank signals\r\r- similar to Time Crisis Zone auto-fire gun problem (hazy fog pixel detection)\r\r\rThis workaround is only needed during calibration\r- May take several attempts to register a test shot\r- Also hold down the trigger longer to get a better detection\r\r- Don't forget to TURN THIS OFF after calibration is done (for fast firing)",
				"Mouse calibration hack", MB_OK );

			return TRUE;


		case IDC_GUNCON_DEFAULT_PROFILES:
			if( IDYES == MessageBox( hWnd, "Restore profiles to default settings?", "Guncon2 profiles", MB_YESNO ) )
			{
				FILE *fp;

				fp = fopen( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ), "w" );
				fprintf( fp, "%s", ps2_guncon2_default_config );
				fclose(fp);



				// reload profiles
				LoadConfig_Guncon_Presets( GetIniPath( "nuvee_ps2_usb_guncon_profiles.ini" ) );
					
					
				hWC = GetDlgItem( hWnd, IDC_GUNCON_ALIGNMENT );

				while( ComboBox_DeleteString( hWC, 0 ) )
				{};

				for( int lcv = 0; lcv < guncon_num_profiles; lcv++ ) {
					ComboBox_AddString( hWC, guncon_profile_name[lcv] );
				}

				ComboBox_SetCurSel( hWC, guncon_alignment[ THIS_PAD ] );
			}
			return TRUE;
		} // end WM_COMMAND
	}

	return FALSE;
}
