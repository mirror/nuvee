#define JUSTIFIER_BUTTON_TRIGGER		0x8000
#define JUSTIFIER_BUTTON_AUX				0x4000
#define JUSTIFIER_BUTTON_START			0x0008


void PADpoll_justifier( int value )
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


		
		// average active window
		JUSTIFIER_WIDTH = justifier_width[ THIS_PAD ];
		JUSTIFIER_HEIGHT = justifier_height[ THIS_PAD ];

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



		// relative mouse
		if( data_absolute == 0 )
		{
			gun_width = 640;
			gun_height = 480;
			gun_right = 640;
			gun_bottom = 480;


			data_x = ( (float) data_x * justifier_sensitivity[ THIS_PAD ] ) / 100;
			data_y = ( (float) data_y * justifier_sensitivity[ THIS_PAD ] ) / 100;


			if( data_x > justifier_threshold[ THIS_PAD ] ) data_x = justifier_threshold[ THIS_PAD ];
			if( data_y > justifier_threshold[ THIS_PAD ] ) data_y = justifier_threshold[ THIS_PAD ];
			if( data_x < -justifier_threshold[ THIS_PAD ] ) data_x = -justifier_threshold[ THIS_PAD ];
			if( data_y < -justifier_threshold[ THIS_PAD ] ) data_y = -justifier_threshold[ THIS_PAD ];

				
			if( data_x >= -justifier_deadzone[ THIS_PAD ] && data_x <= justifier_deadzone[ THIS_PAD ] ) data_x = 0;
			if( data_y >= -justifier_deadzone[ THIS_PAD ] && data_y <= justifier_deadzone[ THIS_PAD ] ) data_y = 0;


			// relative motion
			justifier_analog_x[ THIS_PAD ] += data_x;
			justifier_analog_y[ THIS_PAD ] += data_y;


			// clip to 16-bit absolute window
			if( justifier_analog_x[ THIS_PAD ] < 0 ) justifier_analog_x[ THIS_PAD ] = 0;
			if( justifier_analog_x[ THIS_PAD ] > gun_right ) justifier_analog_x[ THIS_PAD ] = gun_right;
			if( justifier_analog_y[ THIS_PAD ] < 0 ) justifier_analog_y[ THIS_PAD ] = 0;
			if( justifier_analog_y[ THIS_PAD ] > gun_bottom ) justifier_analog_y[ THIS_PAD ] = gun_bottom;


#if 0
			if( pad_active == 1 )
			{
				static FILE *fp_justifier_debug = 0;


				if( fp_justifier_debug == 0 ) {
					fp_justifier_debug = fopen( "justifier1-debug.txt", "w" );
				}

				fprintf( fp_justifier_debug, "%d %d || %d %d\n",
					justifier_analog_x[ THIS_PAD ], justifier_analog_y[ THIS_PAD ],
					data_x, data_y );
			}
#endif



			// offscreen shots
			if( justifier_analog_x[ THIS_PAD ] == 0 || 
					justifier_analog_y[ THIS_PAD ] == 0 || 
					justifier_analog_x[ THIS_PAD ] == gun_right || 
					justifier_analog_y[ THIS_PAD ] == gun_bottom )
			{
				data_x = 0;
				data_y = 0;
			}
			else {
				// re-adjust center values
				data_x = justifier_analog_x[ THIS_PAD ] - (gun_width / 2);
				data_y = justifier_analog_y[ THIS_PAD ] - (gun_height / 2);


				// scale based on justifier window
				data_x = (data_x * (JUSTIFIER_WIDTH/2) ) / (gun_width / 2);
				data_y = (data_y * (JUSTIFIER_HEIGHT/2) ) / (gun_height / 2);


				// aiming scaling
				data_x = ( data_x * justifier_aiming_x[ THIS_PAD ] ) / 100;
				data_y = ( data_y * justifier_aiming_y[ THIS_PAD ] ) / 100;


				// re-adjust to justifier center
				data_x += justifier_screen_center_x[ THIS_PAD ];
				data_y += justifier_screen_center_y[ THIS_PAD ];


				// special edge check (valid data)
				if( data_x < 1 ) data_x = 1;
				if( data_y < 1 ) data_y = 1;
			}
		}
			
		// 16-bit device absolute (x,y) -> local justifier coordinates
		else if( data_absolute == 1 )
		{
			gun_width = justifier_lightgun_right[ THIS_PAD ] - justifier_lightgun_left[ THIS_PAD ] + 1;
			gun_height = justifier_lightgun_bottom[ THIS_PAD ] - justifier_lightgun_top[ THIS_PAD ] + 1;
			gun_right = justifier_lightgun_right[ THIS_PAD ];
			gun_bottom = justifier_lightgun_bottom[ THIS_PAD ];


			// absolute data (range)
			justifier_analog_x[ THIS_PAD ] = data_x;
			justifier_analog_y[ THIS_PAD ] = data_y;


			// offscreen data
			if( justifier_analog_x[ THIS_PAD ] == 0 || 
					justifier_analog_y[ THIS_PAD ] == 0 || 
					justifier_analog_x[ THIS_PAD ] == 65535 || 
					justifier_analog_y[ THIS_PAD ] == 65535 )
			{
				data_x = 0;
				data_y = 0;
			}
			else
			{
				data_x =
					((justifier_analog_x[ THIS_PAD ] - justifier_lightgun_left[ THIS_PAD ]) * JUSTIFIER_WIDTH) /
					gun_width;

				data_y =
					((justifier_analog_y[ THIS_PAD ] - justifier_lightgun_top[ THIS_PAD ]) * JUSTIFIER_HEIGHT) /
					gun_height;


				// re-adjust center values
				data_x = justifier_analog_x[ THIS_PAD ] - (gun_width / 2);
				data_y = justifier_analog_y[ THIS_PAD ] - (gun_height / 2);


				// scale based on justifier window
				data_x = (data_x * (JUSTIFIER_WIDTH/2) ) / (gun_width / 2);
				data_y = (data_y * (JUSTIFIER_HEIGHT/2) ) / (gun_height / 2);


				// aiming scaling
				data_x = ( data_x * justifier_aiming_x[ THIS_PAD ] ) / 100;
				data_y = ( data_y * justifier_aiming_y[ THIS_PAD ] ) / 100;


				// re-adjust to justifier center
				data_x += justifier_screen_center_x[ THIS_PAD ];
				data_y += justifier_screen_center_y[ THIS_PAD ];


				// special edge check (valid data)
				if( data_x < 1 ) data_x = 1;
				if( data_y < 1 ) data_y = 1;
			}
		}



		// draw real-time lightgun cursor
		if( gpuCursor && (justifier_cursor[ THIS_PAD ] == JUSTIFIER_SHOW_CURSOR) ) {
			int screen_x, screen_y;


			if( data_absolute == 0 )
			{
				// offscreen shot detection
				if( justifier_analog_x[ THIS_PAD ] == 640 ||
						justifier_analog_y[ THIS_PAD ] == 480 ) {
					screen_x = 0;
					screen_y = 0;
				}
				else {
					screen_x = (justifier_analog_x[ THIS_PAD ] * 512) / 640;
					screen_y = (justifier_analog_y[ THIS_PAD ] * 256) / 480;
				}
			}
			else {
				// offscreen shot detection
				if( justifier_analog_x[ THIS_PAD ] == 65535 ||
						justifier_analog_y[ THIS_PAD ] == 65535 ) {
					screen_x = 0;
					screen_y = 0;
				}
				else {
					screen_x = (justifier_analog_x[ THIS_PAD ] * 512) / gun_width;
					screen_y = (justifier_analog_y[ THIS_PAD ] * 256) / gun_height;
				}
			}


			// clip to GPU cursor coordinates
			if( screen_x < 0 ) screen_x = 0;
			if( screen_y < 0 ) screen_y = 0;
			if( screen_x > 511 ) screen_x = 511;
			if( screen_y > 255 ) screen_y = 255;



			// player = 1-8, dual lightguns = 2P max (1-2)
			//gpuCursor( ((pad_active-1) * 4) + 1,

			gpuCursor( pad_active, screen_x, screen_y );
		}
	} // pad_count



	switch( pad_cmd ) {
		// $42 = poll data
		case 0x42:
			// already done
			if( pad_count > 0 )
			{
				// $10 unacquire + x-y = (0,0)
				// - based on Lethal Enforcers
				if( pad_count == 2 && value == 0x10 &&
						justifier_trigger_latch[ THIS_PAD ] == 1 && 
						(*((short *) (pad_out+6))) == 0 &&
						(*((short *) (pad_out+8))) == 0 )
				{
					// offscreen shot
					*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;


					// new state - just reload (???)
					justifier_trigger_latch[ THIS_PAD ] = 2;
				}

				// $00 acquire + x-y > (0,0)
				// - based on Lethal Enforcers
				else if( pad_count == 2 && value == 0x00 &&
						justifier_trigger_latch[ THIS_PAD ] == 1 && 
						(*((short *) (pad_out+6))) > 0 &&
						(*((short *) (pad_out+8))) > 0 )
				{
					// on-screen shot
					*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;


					// new state
					justifier_trigger_latch[ THIS_PAD ] = 2;
				}

				break;
			}

			// ==================================
			// ==================================
			// ==================================

			// reset config mode
			if( pad_cmd == 0x42 ) {
				pad_config_mode[ THIS_PAD ] = 0;
			}


			// justifier ID
			pad_out[0] = 0x31;
			pad_out[1] = 0x5a;

			// ==================================
			// ==================================
			// ==================================

			// justifier buttons (default)
			*((short *) (pad_out+2)) = 0xffff;



			for( int lcv = 0; lcv < 3; lcv++ ) {
				int mode;

				if( data_buttons[lcv] == 0 ) continue;


				switch( lcv ) {
				case 0: mode = justifier_left[ THIS_PAD ]; break;
				case 1: mode = justifier_right[ THIS_PAD ]; break;
				case 2: mode = justifier_middle[ THIS_PAD ]; break;
				}


				switch( mode ) {
				case JUSTIFIER_RELOAD:
					data_x = 0;
					data_y = 0;


					if( justifier_trigger_latch[ THIS_PAD ] == 0 ) {
						justifier_trigger_latch[ THIS_PAD ] = 1;
					}
					break;


				case JUSTIFIER_TRIGGER:
					// request latching
					if( justifier_trigger_latch[ THIS_PAD ] == 0 ) {
						//justifier_trigger_latch[ THIS_PAD ] = 1;
					}

					// hold trigger down
					else if( justifier_trigger_latch[ THIS_PAD ] >= 2 ) {
						*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;
					}

					
					// offscreen shot
					*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;
					break;


				case JUSTIFIER_AUX:
					*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_AUX;
					break;


				case JUSTIFIER_START:
					*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_START;
					break;
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
					justifier_analog_x[ THIS_PAD ], justifier_analog_y[ THIS_PAD ],
					data_x, data_y,
					JUSTIFIER_WIDTH, JUSTIFIER_HEIGHT,
					JUSTIFIER_LEFT, JUSTIFIER_TOP, JUSTIFIER_RIGHT, JUSTIFIER_BOTTOM );
			}
#endif



			// clip to guncon window
			if( data_x < 0 ) data_x = 0;
			if( data_y < 0 ) data_y = 0;

			// ==================================
			// ==================================
			// ==================================

			// special cursor mode

			// x-data
			*((short *) (pad_out+8)) = data_x;

			// y-data (normal)
			*((short *) (pad_out+6)) = data_y;



			if( justifier_trigger_latch[ THIS_PAD ] >= 2 )
			{
				justifier_trigger_latch[ THIS_PAD ]++;


				/*
				[00] TRIGGER (2) + [10] (3) + [00] (4) + [10] (5) CURSOR
				- [10] = okay shot if flash
				- [00] = okay shot if no screen flash
				- based on Lethal Enforcers

				Lethal Enforcers = 4-5 (60 Hz in-game refresh)
				Crypt Killer = 6-9 (30 Hz in-game refresh)


				Problem:
				- Real gun seems to be able to detect white flash state
				- Returns cursor data based on flash
				*/

				// Lethal Enforcers
				if( justifier_aiming_model[ THIS_PAD ] == JUSTIFIER_MODEL_HOMEBREW )
				{
					if(	justifier_trigger_latch[ THIS_PAD ] == 5 )
					{
						pad_out[0] = 0x38;


						// ????
						*((short *) (pad_out+2)) = 0xffff;
						*((short *) (pad_out+4)) = 0xffff;
					}

					// force trigger
					else if( justifier_trigger_latch[ THIS_PAD ] < 5 )
						*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;

					// trigger hold done - LE menu
					else {
						if( (pad_out[3] & 0x80) == 0x80 )
							justifier_trigger_latch[ THIS_PAD ] = 0;
		
						pad_out[3] |= 0x80;
					}
				}

				// Crypt Killer
				else if( justifier_aiming_model[ THIS_PAD ] == JUSTIFIER_MODEL_CUSTOM )
				{
					if( justifier_trigger_latch[ THIS_PAD ] == 6 ||
							justifier_trigger_latch[ THIS_PAD ] == 8 )
					{
						pad_out[0] = 0x38;


						// ????
						*((short *) (pad_out+2)) = 0xffff;
						*((short *) (pad_out+4)) = 0xffff;
					}

					// force trigger
					else if( justifier_trigger_latch[ THIS_PAD ] < 9 )
						*((short *) (pad_out+2)) &= ~JUSTIFIER_BUTTON_TRIGGER;

					// trigger hold done
					else {
						if( (pad_out[3] & 0x80) == 0x80 )
							justifier_trigger_latch[ THIS_PAD ] = 0;

						pad_out[3] |= 0x80;
					}
				}
			}
			break;


		// ENTER_CONFIG_MODE
		case 0x43:
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

			// justifier ID
			pad_out[0] = 0x31;
			pad_out[1] = 0x5a;
			break;


		// QUERY_MODEL
		case 0x45:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x31;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;	// model
				pad_out[3] = 0x01;	// # of modes

				pad_out[4] = 0x00;	// mode cur offset
				pad_out[5] = 0x01;	// # of act
				pad_out[6] = 0x01;	// # of comb
				pad_out[7] = 0x00;
			}
			break;


		// QUERY_ACT
		case 0x46:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x31;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x03;
				pad_out[5] = 0x00;

				pad_out[6] = 0x00;
				pad_out[7] = 0x1e;
			}
			break;


		// QUERY_COMB
		case 0x47:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x31;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x01;
				pad_out[5] = 0x00;
				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		// QUERY_MODE
		case 0x4c:
			// already done
			if( pad_count > 0 ) {
				break;
			}

			// ==================================
			// ==================================

			// invalid command (not in config?)
			if( pad_config_mode[ THIS_PAD ] == 0 ) {
				pad_out[0] = 0x31;
				pad_out[1] = 0xff;
			}
			else {
				// config ID
				pad_out[0] = 0xf3;
				pad_out[1] = 0x5a;

				// unknown (??)
				pad_out[2] = 0x00;
				pad_out[3] = 0x00;

				pad_out[4] = 0x00;	// hi-mode
				pad_out[5] = 0x03;	// lo-mode

				pad_out[6] = 0x00;
				pad_out[7] = 0x00;
			}
			break;


		default:
			// already done
			if( pad_count > 0 ) break;


			pad_out[0] = 0x31;
			pad_out[1] = 0xff;

			// invalid command
			pad_out[2] = 0xff;
			pad_out[3] = 0xff;
			break;
	}
}
