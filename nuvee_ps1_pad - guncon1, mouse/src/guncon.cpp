/*
Grazvydas "notaz" Ignotas
- offscreen = (1,10)

256 mode
- x = 0x5a + (356 * xn >> 10);
- y = 0x20 + y;




pSXAuthor

256x240 mode
- center = ~(255,136)
- range = ~(356,240)


320x240 mode
- center = ~(256,136)
- range = ~(384,240)


512x240 mode
- center = ~(256,136)
- range = ~(384,240)
*/


bool Guncon_Illegal_Command( int model )
{
	// seems to work better
	if( model == GUNCON_MODEL_HOMEBREW ) {
		pad_out[0] = 0x63;
		pad_out[1] = 0xff;
				
		pad_out[2] = 0xff;
		pad_out[3] = 0xff;
		pad_out[4] = 0xff;
		pad_out[5] = 0xff;
		pad_out[6] = 0xff;
		pad_out[7] = 0xff;
		return 1;
	}


	return 0;
}			



void PADpoll_guncon( int value )
{
	int reload;

	int data_x, data_y, data_z;
	int data_buttons[3];
	int data_absolute;

	int gun_width, gun_height;
	int gun_right, gun_bottom;


	if( pad_count == 0 ) {
		reload = 0;
		pad_cmd = value;


		// guncon - average active window
		GUNCON_WIDTH = guncon_width[ THIS_PAD ];
		GUNCON_HEIGHT = guncon_height[ THIS_PAD ];

		// ===================================
		// ===================================
		// ===================================

		data_x = device_x[ device_hid[ THIS_PAD ] ];
		data_y = device_y[ device_hid[ THIS_PAD ] ];
		data_z = device_z[ device_hid[ THIS_PAD ] ];

		data_buttons[0] = device_buttons[ device_hid[ THIS_PAD ] ][0];
		data_buttons[1] = device_buttons[ device_hid[ THIS_PAD ] ][1];
		data_buttons[2] = device_buttons[ device_hid[ THIS_PAD ] ][2];

		data_absolute = device_absolute[ device_hid[ THIS_PAD ] ];


#if 0
			//if( pad_active == 2 )
			{
				static FILE *fp_guncon_debug = 0;


				if( fp_guncon_debug == 0 ) {
					fp_guncon_debug = fopen( "guncon1-debug.txt", "w" );
				}

				fprintf( fp_guncon_debug, "%d || %d %d || %d %d\n",
					pad_active,
					guncon_analog_x[ THIS_PAD ], guncon_analog_y[ THIS_PAD ],
					data_x, data_y );
			}
#endif


		// relative mouse
		if( data_absolute == 0 )
		{
			gun_width = 640;
			gun_height = 480;
			gun_right = 640;
			gun_bottom = 480;


			data_x = ( (float) data_x * guncon_sensitivity[ THIS_PAD ] ) / 100;
			data_y = ( (float) data_y * guncon_sensitivity[ THIS_PAD ] ) / 100;


			if( data_x > guncon_threshold[ THIS_PAD ] ) data_x = guncon_threshold[ THIS_PAD ];
			if( data_y > guncon_threshold[ THIS_PAD ] ) data_y = guncon_threshold[ THIS_PAD ];
			if( data_x < -guncon_threshold[ THIS_PAD ] ) data_x = -guncon_threshold[ THIS_PAD ];
			if( data_y < -guncon_threshold[ THIS_PAD ] ) data_y = -guncon_threshold[ THIS_PAD ];

				
			if( data_x >= -guncon_deadzone[ THIS_PAD ] && data_x <= guncon_deadzone[ THIS_PAD ] ) data_x = 0;
			if( data_y >= -guncon_deadzone[ THIS_PAD ] && data_y <= guncon_deadzone[ THIS_PAD ] ) data_y = 0;


			// relative motion
			guncon_analog_x[ THIS_PAD ] += data_x;
			guncon_analog_y[ THIS_PAD ] += data_y;


			// clip to 16-bit absolute window
			if( guncon_analog_x[ THIS_PAD ] < 0 ) guncon_analog_x[ THIS_PAD ] = 0;
			if( guncon_analog_x[ THIS_PAD ] > gun_right ) guncon_analog_x[ THIS_PAD ] = gun_right;
			if( guncon_analog_y[ THIS_PAD ] < 0 ) guncon_analog_y[ THIS_PAD ] = 0;
			if( guncon_analog_y[ THIS_PAD ] > gun_bottom ) guncon_analog_y[ THIS_PAD ] = gun_bottom;


#if 0
			if( pad_active == 2 )
			{
				static FILE *fp_guncon_debug = 0;


				if( fp_guncon_debug == 0 ) {
					fp_guncon_debug = fopen( "guncon1-debug.txt", "w" );
				}

				fprintf( fp_guncon_debug, "%d %d || %d %d\n",
					guncon_analog_x[ THIS_PAD ], guncon_analog_y[ THIS_PAD ],
					data_x, data_y );
			}
#endif



			// offscreen shots
			if( guncon_analog_x[ THIS_PAD ] == 0 || 
					guncon_analog_y[ THIS_PAD ] == 0 || 
					guncon_analog_x[ THIS_PAD ] == gun_right || 
					guncon_analog_y[ THIS_PAD ] == gun_bottom )
			{
				data_x = 0;
				data_y = 0;
			}
			else {
				// re-adjust center values
				data_x = guncon_analog_x[ THIS_PAD ] - (gun_width / 2);
				data_y = guncon_analog_y[ THIS_PAD ] - (gun_height / 2);


				// scale based on guncon window
				data_x = (data_x * (GUNCON_WIDTH/2) ) / (gun_width / 2);
				data_y = (data_y * (GUNCON_HEIGHT/2) ) / (gun_height / 2);


				// Moorhuhn X - 1P hack (homebrew model)
				if( guncon_screen_center_x[ THIS_PAD ] == -1 &&
						guncon_screen_center_y[ THIS_PAD ] == -1 &&
						data_x < 0 )
				{
					// use different scale
					data_x = ( data_x * 66.000f ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}

				// Moorhuhn 2/3 - 1P hack (homebrew model)
				else if( guncon_screen_center_x[ THIS_PAD ] == -2 &&
								 guncon_screen_center_y[ THIS_PAD ] == -2 &&
								 data_x < 0 )
				{
					// use different scale
					data_x = ( data_x * 66.000f ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}

				else {
					// aiming scaling
					data_x = ( data_x * guncon_scale_x[ THIS_PAD ] ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}



				// Moorhuhn X - 1P hack (homebrew model)
				if( guncon_screen_center_x[ THIS_PAD ] == -1 &&
						guncon_screen_center_y[ THIS_PAD ] == -1 ) {
					data_x += 270;
					data_y += 180;
				}

				// Moorhuhn 2/3 - 1P hack (homebrew model)
				else if( guncon_screen_center_x[ THIS_PAD ] == -2 &&
							 	 guncon_screen_center_y[ THIS_PAD ] == -2 ) {
					data_x += 270;
					data_y += 182;
				}

				else {
					// re-adjust to guncon center
					data_x += guncon_screen_center_x[ THIS_PAD ];
					data_y += guncon_screen_center_y[ THIS_PAD ];
				}


				// special edge check (valid data)
				if( data_x < 1 ) data_x = 1;
				if( data_y < 1 ) data_y = 1;
			}
		}
			
		// 16-bit device absolute (x,y) -> local guncon coordinates
		else if( data_absolute == 1 )
		{
			gun_width = guncon_lightgun_right[ THIS_PAD ] - guncon_lightgun_left[ THIS_PAD ] + 1;
			gun_height = guncon_lightgun_bottom[ THIS_PAD ] - guncon_lightgun_top[ THIS_PAD ] + 1;
			gun_right = guncon_lightgun_right[ THIS_PAD ];
			gun_bottom = guncon_lightgun_bottom[ THIS_PAD ];


			// absolute data (range)
			guncon_analog_x[ THIS_PAD ] = data_x;
			guncon_analog_y[ THIS_PAD ] = data_y;


			// offscreen data
			if( guncon_analog_x[ THIS_PAD ] == 0 || 
					guncon_analog_y[ THIS_PAD ] == 0 || 
					guncon_analog_x[ THIS_PAD ] == 65535 || 
					guncon_analog_y[ THIS_PAD ] == 65535 )
			{
				data_x = 0;
				data_y = 0;
			}
			else
			{
				data_x =
					((guncon_analog_x[ THIS_PAD ] - guncon_lightgun_left[ THIS_PAD ]) * GUNCON_WIDTH) /
					gun_width;

				data_y =
					((guncon_analog_y[ THIS_PAD ] - guncon_lightgun_top[ THIS_PAD ]) * GUNCON_HEIGHT) /
					gun_height;


				// re-adjust center values
				data_x = guncon_analog_x[ THIS_PAD ] - (gun_width / 2);
				data_y = guncon_analog_y[ THIS_PAD ] - (gun_height / 2);


				// scale based on guncon window
				data_x = (data_x * (GUNCON_WIDTH/2) ) / (gun_width / 2);
				data_y = (data_y * (GUNCON_HEIGHT/2) ) / (gun_height / 2);


				// Moorhuhn X - 1P hack (homebrew model)
				if( guncon_screen_center_x[ THIS_PAD ] == -1 &&
						guncon_screen_center_y[ THIS_PAD ] == -1 &&
						data_x < 0 )
				{
					// use different scale
					data_x = ( data_x * 66.000f ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}

				// Moorhuhn 2/3 - 1P hack (homebrew model)
				else if( guncon_screen_center_x[ THIS_PAD ] == -2 &&
								 guncon_screen_center_y[ THIS_PAD ] == -2 &&
								 data_x < 0 )
				{
					// use different scale
					data_x = ( data_x * 66.000f ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}
				else {
					// aiming scaling
					data_x = ( data_x * guncon_scale_x[ THIS_PAD ] ) / 100;
					data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;
				}


				// Moorhuhn X - 1P hack (homebrew model)
				if( guncon_screen_center_x[ THIS_PAD ] == -1 &&
						guncon_screen_center_y[ THIS_PAD ] == -1 ) {
					data_x += 270;
					data_y += 180;
				}

				// Moorhuhn 2/3 - 1P hack (homebrew model)
				else if( guncon_screen_center_x[ THIS_PAD ] == -2 &&
							 	 guncon_screen_center_y[ THIS_PAD ] == -2 ) {
					data_x += 270;
					data_y += 182;
				}

				else {
					// re-adjust to guncon center
					data_x += guncon_screen_center_x[ THIS_PAD ];
					data_y += guncon_screen_center_y[ THIS_PAD ];
				}


				// special edge check (valid data)
				if( data_x < 1 ) data_x = 1;
				if( data_y < 1 ) data_y = 1;
			}
		}




		// draw real-time lightgun cursor (true device position)
		if( guncon_cursor[ THIS_PAD ] == GUNCON_SHOW_CURSOR ) {
			int screen_x, screen_y;


			if( data_absolute == 0 )
			{
				screen_x = (guncon_analog_x[ THIS_PAD ] * 512) / 640;
				screen_y = (guncon_analog_y[ THIS_PAD ] * 256) / 480;
			}
			else {
				// offscreen shot detection
				if( guncon_analog_x[ THIS_PAD ] == 65535 ||
						guncon_analog_y[ THIS_PAD ] == 65535 ) {
					screen_x = 0;
					screen_y = 0;
				}
				else {
					screen_x = (guncon_analog_x[ THIS_PAD ] * 512) / gun_width;
					screen_y = (guncon_analog_y[ THIS_PAD ] * 256) / gun_height;
				}
			}



			// clip to GPU cursor coordinates
			if( screen_x < 0 ) screen_x = 0;
			if( screen_y < 0 ) screen_y = 0;
			if( screen_x > 511 ) screen_x = 511;
			if( screen_y > 255 ) screen_y = 255;



			if( gpuCursor ) {
				// player = 1-8, dual lightguns = 2P max (1-2)
				//gpuCursor( ((pad_active-1) * 4) + 1,

				gpuCursor( pad_active, screen_x, screen_y );
			}

			// windows cursor
			else if( hWnd_app == GetForegroundWindow() )
			{
				int w,h, x,y;
				RECT r;


				// skip gun2 cursor if gun1 cursor on (+ mouse)
				if( mouse_active == 2 && pad_active == 2 &&
						guncon_cursor[0] == GUNCON_SHOW_CURSOR )
				{}
				else {
					// debug - force windows cursor
					ShowCursor(1);

					GetWindowRect( hWnd_app, &r );


					// chop window title bar
					// - mult-monitor = left may be non-zeo
					if( r.top != 0 ) {
						r.top += 32;


						r.top += 0;
						r.left += 4;
						r.right -= 4;
						r.bottom -= 4;
					}

					SnapCursor();


					w = r.right - r.left;
					h = r.bottom - r.top;

					x = r.left + w * screen_x / 512;
					y = r.top + h * screen_y / 256;


					SetCursorPos( x,y );
				}
			} // end gpu + win32
		} // end cursor
	} // pad_count



#if 0
	{
		static FILE *fp_cmd = 0;

		if( fp_cmd == 0 ) fp_cmd = fopen("cmd-log.txt","w");

		fprintf( fp_cmd, "%X\n", pad_cmd );
	}
#endif


	switch( pad_cmd ) {
		// $42 = poll data
		case 0x42:
			// already done
			if( pad_count > 0 ) break;

			// ==================================
			// ==================================
			// ==================================

			// reset config mode
			pad_config_mode[ THIS_PAD ] = 0;


			// guncon ID
			pad_out[0] = 0x63;
			pad_out[1] = 0x5a;

			// ==================================
			// ==================================
			// ==================================

			// $2000 = trigger
			// $0008 = A
			// $4000 = B

			// mouse buttons (default)
			pad_out[2] = 0xff;
			pad_out[3] = 0xff;


			for( int lcv = 0; lcv < 3; lcv++ ) {
				int mode;

				if( data_buttons[lcv] == 0 ) continue;

				if( lcv == 0 ) mode = guncon_left[ THIS_PAD ];
				if( lcv == 1 ) mode = guncon_right[ THIS_PAD ];
				if( lcv == 2 ) mode = guncon_middle[ THIS_PAD ];


				switch( mode ) {
				case GUNCON_RELOAD:
					pad_out[3] &= ~0x20;

					data_x = 0;
					data_y = 0;

					reload = 1;
					break;

				case GUNCON_TRIGGER:
					pad_out[3] &= ~0x20;
					break;

				case GUNCON_A:
					pad_out[2] &= ~0x08;
					break;

				case GUNCON_B:
					pad_out[3] &= ~0x40;
					break;
				}
			}

			// ==================================
			// ==================================
			// ==================================

			/*
			visual alignment hack
			- x = hold trigger
			
			- 0 = wheel up
			- 1 = wheel down
			- 2 = wheel up
			- 3 = wheel down
			- 4 = wheel up
			- 5 = wheel down
			
			- 6 = release trigger
			- 7 = press trigger

			- 8 = perfect crosshair centering
			*/


			// trigger down
			if( (pad_out[3] & 0x20) == 0 )
			{
				switch( guncon_alignment_auto[ THIS_PAD ] ) {
				case 0:
					if( data_z > 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 1:
					if( data_z < 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 2:
					if( data_z > 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 3:
					if( data_z < 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 4:
					if( data_z > 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 5:
					if( data_z < 0 ) guncon_alignment_auto[ THIS_PAD ]++;
					break;

				case 6:
					// not possible
					break;

				case 7:
					guncon_analog_x[ THIS_PAD ] = gun_width / 2;
					guncon_analog_y[ THIS_PAD ] = gun_height / 2;

					data_x = guncon_screen_center_x[ THIS_PAD ];
					data_y = guncon_screen_center_y[ THIS_PAD ];

					guncon_alignment_auto[ THIS_PAD ] = 0;
					break;
				}
			}

			// trigger up - check calibration
			else {
				if( guncon_alignment_auto[ THIS_PAD ] < 6 ) {
					guncon_alignment_auto[ THIS_PAD ] = 0;
				}
				else if( guncon_alignment_auto[ THIS_PAD ] == 6 ) {
					guncon_alignment_auto[ THIS_PAD ]++;
				}
			}

			// ==================================
			// ==================================
			// ==================================

			// offscreen reload - semi-automatic trigger
			if( guncon_reload[ THIS_PAD ] == GUNCON_RELOAD_SEMI )
			{
				static int guncon_reload_auto;

				if( guncon_reload_auto == 0 )
				{
					if( data_x < 0 || data_y < 0 )
						guncon_reload_auto = 1;
				}
				else
				{
					guncon_reload_auto++;


					if( guncon_reload_auto < 1+3 ) {
						pad_out[3] |= 0x20;
						reload = 0;
					}

					else if( guncon_reload_auto < 1+6 ) {
						pad_out[3] &= ~0x20;
						reload = 1;
					}

					// wait time
					else if( guncon_reload_auto > 30 ) {
						guncon_reload_auto = 0;

						reload = 0;
					}
				}
			}

			// ==================================
			// ==================================
			// ==================================

#if 0
			if( pad_active == 1 )
			{
				static FILE *fp_guncon_debug = 0;


				if( fp_guncon_debug == 0 ) {
					fp_guncon_debug = fopen( "guncon2-debug.txt", "w" );
				}

				fprintf( fp_guncon_debug, "[%d %d] %d %d + %d %d [%d %d | %d %d %d %d]\n",
					gpu_screen_x, gpu_screen_y,
					guncon_analog_x[ THIS_PAD ], guncon_analog_y[ THIS_PAD ],
					data_x, data_y,
					GUNCON_WIDTH, GUNCON_HEIGHT,
					GUNCON_LEFT, GUNCON_TOP, GUNCON_RIGHT, GUNCON_BOTTOM );
			}
#endif



			// clip to guncon window
			if( data_x < 0 ) data_x = 0;
			if( data_y < 0 ) data_y = 0;

			// ==================================
			// ==================================
			// ==================================

			// offscreen reload - manual trigger
			if( data_x == 0 || data_y == 0 )
			{
				// trigger pulled
				if( (pad_out[3] & 0x20) == 0 )
					reload = 1;
			}

			// ==================================
			// ==================================
			// ==================================

			// analog x-y
			*((short *) (pad_out+4)) = (s16) data_x;
			*((short *) (pad_out+6)) = (s16) data_y;


			// special handling
			if( reload == 1 ) {
				*((short *) (pad_out+4)) = 0;
				*((short *) (pad_out+6)) = 0;
			}
			break;



		// ENTER_CONFIG_MODE
		case 0x43:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				// set config mode
				if( pad_count == 2 )
				{
					pad_config_mode[ THIS_PAD ] = value;
				}
				break;
			}

			// ==================================
			// ==================================
			// ==================================

			if( pad_config_mode[ THIS_PAD ] == 0 )
				pad_out[0] = 0x63;
			else
				pad_out[0] = 0xf3;

			pad_out[1] = 0x5a;

			pad_out[2] = 0xff;
			pad_out[3] = 0xff;

			pad_out[4] = 0x00;
			pad_out[5] = 0x00;

			pad_out[6] = 0x00;
			pad_out[7] = 0x00;
			break;


		// QUERY_POLL (???)
		case 0x41:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x00;
				pad_out[5] = 0x00;
				
				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		// SET_MODE_AND_LOCK
		case 0x44:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x00;
				pad_out[5] = 0x00;
				
				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		// QUERY_MODEL
		// - Time Crisis Project Titan
		case 0x45:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;


				// PS2: Endgame (u)
				if( guncon_aiming_model[ THIS_PAD ] == GUNCON_MODEL_CUSTOM ) {
					pad_out[2] = 0x00;
					pad_out[3] = 0x00;
					pad_out[4] = 0x00;
					pad_out[5] = 0x00;
					pad_out[6] = 0x00;
					pad_out[7] = 0x00;
				}

				else {
					// unknown (??)
					pad_out[2] = 0x01;	// model?
					pad_out[3] = 0x01;	// # of modes (2?)

					pad_out[4] = 0x00;	// current mode
					pad_out[5] = 0x01;	// # of act (2?)
					pad_out[6] = 0x01;	// # of comb
					pad_out[7] = 0x00;
				}
			}
			break;


		// QUERY_ACT
		case 0x46:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;


				// PS2: Endgame (u)
				if( guncon_aiming_model[ THIS_PAD ] == GUNCON_MODEL_CUSTOM ) {
					pad_out[2] = 0x00;
					pad_out[3] = 0x00;
					pad_out[4] = 0x00;
					pad_out[5] = 0x00;
					pad_out[6] = 0x00;
					pad_out[7] = 0x00;
				}

				else {
					// unknown (??)
					pad_out[2] = 0x00;	// index of actuator

					pad_out[3] = 0x00;
					pad_out[4] = 0x06;	// function (0? not in use?)
					pad_out[5] = 0x00;	// subfunction (02?)
					pad_out[6] = 0x00;	// length (0 = off?)
					pad_out[7] = 0x1e;	// amps (0a?)
				}
			}
			break;


		// QUERY_COMB
		case 0x47:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;


				// PS2: Endgame (u)
				if( guncon_aiming_model[ THIS_PAD ] == GUNCON_MODEL_CUSTOM ) {
					pad_out[2] = 0x00;
					pad_out[3] = 0x00;
					pad_out[4] = 0x00;
					pad_out[5] = 0x00;
					pad_out[6] = 0x00;
					pad_out[7] = 0x00;
				}

				else {
					// unknown (??)
					pad_out[2] = 0x00;	// index of comb

					pad_out[3] = 0x00;
					pad_out[4] = 0x01;	// # of actuators in comb (2?)

					pad_out[5] = 0x00;	// act0 in comb
					pad_out[6] = 0x00;	// act1 in comb
					pad_out[7] = 0x00;	// act2 in comb
				}
			}
			break;


		// QUERY_MODE
		case 0x4c:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

		// ==================================
		// ==================================


			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;


				// PS2: Endgame (u)
				if( guncon_aiming_model[ THIS_PAD ] == GUNCON_MODEL_CUSTOM ) {
					pad_out[2] = 0x00;
					pad_out[3] = 0x00;
					pad_out[4] = 0x00;
					pad_out[5] = 0x00;
					pad_out[6] = 0x00;
					pad_out[7] = 0x00;
				}

				else {
					// unknown (??)
					pad_out[2] = 0x00;	// write: select 00 or 01 block data

					pad_out[3] = 0x00;
					pad_out[4] = 0x00;	// hi-mode
					pad_out[5] = 0x06;	// lo-mode

					pad_out[6] = 0x00;
					pad_out[7] = 0x00;
				}
			}
			break;


		// SET_ACT_ALIGN
		// - Resident Evil Survivor (PAL) (??)
		case 0x4d:
			if( Guncon_Illegal_Command( guncon_aiming_model[ THIS_PAD ] ) == 1 ) break;


			// already done
			if( pad_count > 0 ) {
				break;
			}

		// ==================================
		// ==================================


			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x63;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// mapping between poll bytes and actuators channels
				//   ex. 00/00, 01/01, ff/ff, ff/ff, ff/ff, ff/ff
				//       (dualshock) 00 = actuator #1 (small motor), 01 = actuator #2 (large motor)

				// default is $ff - previous actuator mapping
				pad_out[2] = 0xff;
				pad_out[3] = 0xff;

				pad_out[4] = 0xff;
				pad_out[5] = 0xff;

				pad_out[6] = 0xff;
				pad_out[7] = 0xff;
			}
			break;



		default:
			// already done
			if( pad_count > 0 ) break;


			if(0)
			{
				static int test = 0;

				if( test == 0 ) {
					test = 1;

					MessageBox( NULL, "Unknown guncon cmd found", "Guncon", MB_OK );
				}
			}


			pad_out[0] = 0xff;

			// Gunfighter / Point Blank 3 / Ghostbusters
			pad_out[1] = 0xff;

			// invalid command
			pad_out[2] = 0xff;
			pad_out[3] = 0xff;

			pad_out[4] = 0xff;
			pad_out[5] = 0xff;
			pad_out[6] = 0xff;
			pad_out[7] = 0xff;

			break;
	}
}
