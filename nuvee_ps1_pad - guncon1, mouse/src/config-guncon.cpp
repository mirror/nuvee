#include <stdio.h>


#define GUNCON1_INI "2_01"

char ps1_guncon_default_config[] = "\
; cmdline_id  game_name\n\
; sensitivity x-y  center x-y\n\
; model  width-height\n\
\n\
\n\
\n\
die_hard2 \"Die Hard Trilogy 2 - Port 2\"\n\
100.50 96.5 270 174\n\
homebrew 384 256\n\
\n\
\n\
\n\
gearbolt \"Elemental Gearbolt\"\n\
100.75 94.5 260 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
ghostbusters \"Extreme Ghostbusters\"\n\
91.75 100.0 288 184\n\
homebrew 384 256\n\
\n\
\n\
\n\
; Calibration notice\n\
; - (1) Must aim at pig's nose to get perfect\n\
\n\
game_paradise2 \"(*) Gunbare Game Paradise 2 - Port 2\"\n\
100.75 100.75 288 178\n\
unlicensed 320 240\n\
\n\
\n\
\n\
ghoul_panic \"Ghoul Panic\"\n\
100 100 274 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
gunfighter \"Gunfighter - Jesse James\"\n\
100 94.625 288 168\n\
homebrew 384 256\n\
\n\
\n\
\n\
; NOTE: width/height = only calibration solution\n\
\n\
judge_dredd \"Judge Dredd\"\n\
77.625 94.625 274 164\n\
homebrew 512 320\n\
\n\
\n\
\n\
maximum_force \"Maximum Force\"\n\
100.15 94.625 254 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
mighty_hits \"Mighty Hits Special\"\n\
100 100 274 164\n\
unlicensed 384 256\n\
\n\
\n\
\n\
; HACK: unusual cursor movement to left (lightgun protection?)\n\
; - (-2 -2) center = Moorhuhn 2/3 1P hack\n\
\n\
moorhuhn3_1p \"Moorhuhn 2/3 - Port 1\"\n\
81.25 95.125 -2 -2\n\
homebrew 384 256\n\
\n\
\n\
\n\
moorhuhn3_2p \"Moorhuhn 2/3 - Port 2\"\n\
82.25 95.125 273 175\n\
homebrew 384 256\n\
\n\
\n\
\n\
; HACK: unusual cursor movement to left (lightgun protection?)\n\
; - (-1 -1) center = Moorhuhn X 1P hack\n\
\n\
moorhuhn_1p \"Moorhuhn X - Port 1\"\n\
81.25 90.625 -1 -1\n\
homebrew 384 260\n\
\n\
\n\
\n\
moorhuhn_2p \"Moorhuhn X - Port 2\"\n\
80.75 90.625 274 174\n\
homebrew 384 256\n\
\n\
\n\
\n\
point_blank1 \"Point Blank 1\"\n\
97.625 94.625 274 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
point_blank2 \"Point Blank 2\"\n\
97.625 94.625 274 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
point_blank3 \"Point Blank 3\"\n\
97.625 94.625 274 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
; Calibration options\n\
; - (1) = re-calibrate gun in-game again (Hold Trigger + A+B)\n\
; - (2) = aim at bottom of bullseye (ignore shot marker)\n\
\n\
re_survivor \"(*) Resident Evil Survivor\"\n\
98.625 94.500 266 204\n\
homebrew 384 256\n\
\n\
\n\
\n\
rescue_shot \"Rescue Shot\"\n\
100.000 100.000 266 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
simple1500_gun1 \"Simple 1500 - Gun Shooting 1\"\n\
100.000 94.500 266 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
simple1500_gun2 \"Simple 1500 - Gun Shooting 2\"\n\
100.000 94.500 266 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
time_crisis \"Time Crisis\"\n\
94.625 94.625 266 160\n\
homebrew 384 256\n\
\n\
\n\
\n\
project_titan \"Time Crisis - Project Titan\"\n\
100.625 94.500 266 164\n\
homebrew 384 256\n\
\n\
\n\
\n\
dummy_marker1 \"====== Guncon conversions\"\n\
100.50 96.5 270 174\n\
homebrew 384 256\n\
\n\
\n\
\n\
crypt_killer \"Crypt Killer\"\n\
100.0 100.0 253 132\n\
homebrew 384 240\n\
\n\
\n\
\n\
die_hard_trilogy \"Die Hard Trilogy (U)\"\n\
100.0 100.0 255 135\n\
homebrew 384 236\n\
\n\
\n\
\n\
die_hard_trilogy_e256 \"Die Hard Trilogy (E) (256 full)\"\n\
100.0 104.0 254 133\n\
homebrew 384 234\n\
\n\
\n\
\n\
die_hard_trilogy_e240 \"Die Hard Trilogy (E) (240 crop)\"\n\
100.0 104.0 255 135\n\
homebrew 384 234\n\
\n\
\n\
\n\
lethal_enforcers \"Lethal Enforcers (U,E)\"\n\
100.0 100.0 256 135\n\
homebrew 384 240\n\
\n\
\n\
\n\
lethal_enforcers_e240 \"Lethal Enforcers (E) (240 crop)\"\n\
100.0 93.75 256 135\n\
homebrew 384 240\n\
\n\
\n\
\n\
policenauts \"Policenauts (J) - Port 2\"\n\
100.0 100.0 256 135\n\
homebrew 384 240\n\
\n\
\n\
\n\
; gun balanced when stationary (training) - movement adds motion physics\n\
horned_owl \"(*) Project Horned Owl\"\n\
106.25 100.0 257 135\n\
homebrew 356 240\n\
\n\
\n\
\n\
rebel_assault2 \"Rebel Assault 2 - Port 2\"\n\
101.0 100.0 254 135\n\
homebrew 384 240\n\
\n\
\n\
\n\
";


char ps2_guncon_default_config[] = "\
; cmdline_id  game_name\n\
; sensitivity x-y  center x-y\n\
; model  width-height\n\
\n\
\n\
\n\
endgame \"Endgame (U)\"\n\
75.00 89.00 256 137\n\
custom 512 256\n\
\n\
\n\
\n\
; game doesn't detect G-Con 45\n\
\n\
guncom2 \"(x) Guncom 2 (E)\"\n\
75.00 95.50 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
gunfighter2 \"Gunfighter 2 - Jesse James (E)\"\n\
75.00 95.50 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
; set Port1 = none at calibration screen\n\
; - skips calibration (not working)\n\
\n\
ninja_assault \"(*) Ninja Assault (E) (50 Hz)\"\n\
75.75 100.00 258 190\n\
custom 512 256\n\
\n\
\n\
\n\
; set Port1 = none at calibration screen\n\
; - skips calibration (not working)\n\
\n\
ninja_assault_60 \"(*) Ninja Assault (E) (60 Hz)\"\n\
75.75 108.00 257 177\n\
custom 512 256\n\
\n\
\n\
\n\
; calibration note:\n\
; - after each test shot, hold gun still for a few seconds\n\
;   for calibration to detect the correct average position\n\
\n\
re_survivor2 \"(*) Resident Evil Survivor 2 (E)\"\n\
75.00 95.50 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
time_crisis2 \"Time Crisis 2 (U)\"\n\
75.00 90.25 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
time_crisis3 \"Time Crisis 3 (E)\"\n\
75.00 90.25 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
time_crisis_zone \"Time Crisis - Crisis Zone (E)\"\n\
75.00 90.25 260 164\n\
custom 512 256\n\
\n\
\n\
\n\
vampire_night_e \"Vampire Night (E)\"\n\
74.00 100.00 260 163\n\
custom 512 256\n\
\n\
\n\
\n\
vampire_night \"Vampire Night (U)\"\n\
74.00 91.75 260 164\n\
custom 512 256\n\
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


		else if( strcmp( str, GUNCON1_INI ) != 0 ) {
			fclose( fp );
			fp = 0;
		}
	}


	// create file
	if( !fp ) {
		char str[1024];


		fp = fopen( file_name, "w" );

		if( fp ) {
			fprintf( fp, "VERSION = %s\n\n", GUNCON1_INI );


			if( emu_type == 1 )
				fprintf( fp, "%s", ps1_guncon_default_config );
			else if( emu_type == 2 )
				fprintf( fp, "%s", ps2_guncon_default_config );

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


			guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_HOMEBREW;
			if( strcmp( str, "homebrew" ) == 0 )
				guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_HOMEBREW;
			else if( strcmp( str, "custom" ) == 0 )
				guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_CUSTOM;
			else if( strcmp( str, "unlicensed" ) == 0 )
				guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_UNLICENSED;
			else if( strcmp( str, "namco" ) == 0 )
				guncon_profile_model[ guncon_num_profiles ] = GUNCON_MODEL_NAMCO;



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

		fclose( fp );
	}
}


DWORD WINAPI get_guncon_hid_device()
{
	int lcv, reset_data;
	int calibrate_check;


	if( pad_active == 1 ) {
		LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon1.ini" ) );


		MessageBox( NULL,
			"Please connect the mouse or lightgun you wish to use\r\r1- Close this box\r2- Press a button on the desired device\r3- Wait for immediate confirmation\r4- If no detection, click mouse or fire trigger at game window and try again\r5- Plugin will auto-remember HID name for next session",
			"Guncon 1 - Acquire", MB_OK );
	}
	else if( pad_active == 2 ) {
		LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon2.ini" ) );


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


		SaveConfig_Guncon( GetIniPath( "nuvee_psx_guncon1.ini" ) );
	}
	else if( pad_active == 2 ) {
		MessageBox( NULL,
			"Device found - ready to use",
			"Guncon 2 - Acquire", MB_OK );


		SaveConfig_Guncon( GetIniPath( "nuvee_psx_guncon2.ini" ) );
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

		
		
		LoadConfig_Guncon_Presets( GetIniPath( "nuvee_psx_guncon_profiles.ini" ) );


		if( pad_active == 1 ) {
			SetWindowText( hWnd, "Guncon - Port 1" );


			LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon1.ini" ) );
		}
		else {
			SetWindowText( hWnd, "Guncon - Port 2" );


			LoadConfig_Guncon( GetIniPath( "nuvee_psx_guncon2.ini" ) );
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
		ComboBox_SetCurSel( hWC, guncon_left[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_RIGHT );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_SetCurSel( hWC, guncon_right[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_MIDDLE );
		ComboBox_AddString( hWC, "Reload" );
		ComboBox_AddString( hWC, "Trigger" );
		ComboBox_AddString( hWC, "A" );
		ComboBox_AddString( hWC, "B" );
		ComboBox_SetCurSel( hWC, guncon_middle[ THIS_PAD ] );



		hWC = GetDlgItem( hWnd, IDC_GUNCON_RELOAD );
		ComboBox_AddString( hWC, "Manual" );
		ComboBox_AddString( hWC, "Semi" );
		ComboBox_SetCurSel( hWC, guncon_reload[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_CURSOR );
		ComboBox_AddString( hWC, "Visible" );
		ComboBox_AddString( hWC, "Hidden" );
		ComboBox_SetCurSel( hWC, guncon_cursor[ THIS_PAD ] );

		hWC = GetDlgItem( hWnd, IDC_GUNCON_MODEL );
		ComboBox_AddString( hWC, "Homebrew" );
		ComboBox_AddString( hWC, "Custom" );
		ComboBox_AddString( hWC, "Unlicensed" );
		ComboBox_AddString( hWC, "Namco (N/A)" );
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

		return TRUE;


	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_GUNCON_OK:
			GetConfig_Guncon( hWnd );


			if( pad_active == 1 )
				SaveConfig_Guncon( GetIniPath( "nuvee_psx_guncon1.ini" ) );
			else
				SaveConfig_Guncon( GetIniPath( "nuvee_psx_guncon2.ini" ) );



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
				"NOTE:\rLightgun works best at 640x480\r\r\rTune visual crosshairs for each game\r- makes visual crosshair + shots more precise\r\r- At higher resolutions, you may have to calibrate slightly __off-center__ to get better shot markers\r\rCustom aiming values\r- If your shots overshoot, set a higher aiming x-y value\r- If your shots undershoot, set a lower aiming x-y value\r\rSample X = 94.5, 97.5, 98.5, 100.0\rSample Y = 94.5, 94.625, 100.0\r\r\rAuto-calibration (mouse only)\r- 1. Go to the gun calibration screen\r- 2. Hold down trigger\r- 3. Roll mouse wheel up-down-up-down-up-down\r- 4. Release trigger\r- 5. Fire a test shot to finish gun centering",
				"Alignment", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP6:
			MessageBox( hWnd,
				"NOTE:\rLightgun works best at 640x480\r\r\rPC lightguns use an edge coordinate system\r- acquire device to calibrate and adjust if needed",
				"Gun edges", MB_OK );
			return TRUE;


		case IDC_GUNCON_HELP_MODEL:
			MessageBox( hWnd,
				"Some games will only work with certain Guncon model parameters\r\r\r1- Homebrew = home-made type gun, should work well also\r\r2- Custom = works with many USA games; best version to try first\r\r3- Unlicensed = attempts to mimic a 3rd party lightgun; try this if custom / homebrew fails\r\r4- Namco = authentic lightgun (the best choice); correct values currently unknown",
				"Model", MB_OK );
			return TRUE;


		case IDC_GUNCON_ACQUIRE:
			{
				CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)get_guncon_hid_device, NULL, 0, 0 );

				return TRUE;
			}


		case IDC_GUNCON_DEFAULT_PROFILES:
			{
				if( IDYES == MessageBox( hWnd, "Restore profiles to default settings?", "Guncon2 profiles", MB_YESNO ) )
				{
					FILE *fp;

					fp = fopen( GetIniPath( "nuvee_psx_guncon_profiles.ini" ), "w" );

					if( fp ) {
						fprintf( fp, "VERSION = %s\n\n", GUNCON1_INI );

						if( emu_type == 1 )
							fprintf( fp, "%s", ps1_guncon_default_config );
						else if( emu_type == 2 )
							fprintf( fp, "%s", ps2_guncon_default_config );
					}
					fclose(fp);


					// reload profiles
					LoadConfig_Guncon_Presets( GetIniPath( "nuvee_psx_guncon_profiles.ini" ) );
					

					hWC = GetDlgItem( hWnd, IDC_GUNCON_ALIGNMENT );

					while( ComboBox_DeleteString( hWC, 0 ) )
					{};

					for( int lcv = 0; lcv < guncon_num_profiles; lcv++ ) {
						ComboBox_AddString( hWC, guncon_profile_name[lcv] );
					}

					ComboBox_SetCurSel( hWC, guncon_alignment[ THIS_PAD ] );
				}

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
				strcat( temp_str, "\\nuvee_psx_guncon_profiles.ini\"" );
				if( CreateProcessA(NULL,
					temp_str,
					0,0,FALSE,0,0,0,
					&siStartupInfo, &piProcessInfo) == TRUE )
				{
			    WaitForSingleObject( piProcessInfo.hProcess, INFINITE );
		      CloseHandle( piProcessInfo.hProcess );
	        CloseHandle( piProcessInfo.hThread );


					// reload profiles
					LoadConfig_Guncon_Presets( GetIniPath( "nuvee_psx_guncon_profiles.ini" ) );
					
					

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
		}
	}

	return FALSE;
}
