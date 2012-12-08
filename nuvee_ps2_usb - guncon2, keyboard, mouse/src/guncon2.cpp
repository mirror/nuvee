/*
NOTE: Almost the same behavior as Guncon1
- Extra C,START,SELECT,D-Pad

- Should be 6 bytes output (we use 8 which is max packet size)
- Polls every 8 ms (8 USB frames)

- Interlace / Progressive scan mode
  (gun behaves differently per scan)



PROBLEM:

(mouse only!! real EMS TopGun 2 should do this correctly)
Calibration flashes screen -BLACK- and measures -RESPONSE- time
- This may require reading the GSdx pixels for black
  and notifying the game several USB frames later (~3-6)

- May also need VBlank notification for this to work
  (8 ms frames vs 16.6666 / 20 ms VSync)

- Time Crisis Zone also relies on this trick for autofire gun
  (hazy fog controls rate of submachine gun bullets)

- Currently using a workaround hack for calibration only

- See Virtua Cop or Vampire Night for response lag
*/


// author: numbski + smog
#define BUTTON_TRIGGER			0x2000
#define BUTTON_A						0x0008
#define BUTTON_B						0x0004
#define BUTTON_C						0x0002
#define BUTTON_SELECT				0x4000
#define BUTTON_START				0x8000
#define DPAD_UP      				0x0010
#define DPAD_DOWN						0x0040
#define DPAD_LEFT 					0x0080
#define DPAD_RIGHT 					0x0020


/*
Progressive scan = 2-shot calibration
1) Point the gun away from the TV screen
2) Hold the trigger and press the select button on the gun.
3) This toggles the words ''''Progressive scan'' to ''100hz''

Seems to work better than interlace
*/
#define PROGRESSIVE_SCAN		0x0100


int current_analog_x[2];
int current_analog_y[2];
int current_analog_z[2];


// trigger -> flash -> get x/y data
int latch_trigger_delay[2];
int latch_trigger_x[2];
int latch_trigger_y[2];


// (mouse only!! real EMS TopGun 2 should do this correctly)
//
// both progressive / interlace
// - to make things worse, game measures -RESPONSE- time (fast fire)
// - seems to work best ~8-9
#define LATCH_DELAY (8+1)

// =======================================
// =======================================
// =======================================

#define GUNCON2_SET_PARAM			9

#define GUNCON2_PARAM_X				0
#define GUNCON2_PARAM_Y				1
#define GUNCON2_PARAM_MODE		2


// Probe data by Tony Shadwick (numbski)
u8 guncon2_device_descriptor[] = {
	0x12,						// Length
	0x01,						// Type (Descriptor)
	0x00, 0x01,			// USB version # (1.0)

	0xff,						// Class
	0x00,						// Subclass
	0x00,						// Protocol
	0x08,						// Max Packet Size

	0x9a, 0x0b,			// Vendor ID
	0x6a, 0x01,			// Product ID
	0x00, 0x01,			// Device version # (1.0)

	0x00,						// Manufacturer string index #
	0x00,						// Product string index #
	0x00,						// Serial number string index #
	0x01,						// # configurations
};


// Probe data by Tony Shadwick (numbski)
u8 guncon2_config_descriptor[] = {
	0x09,						// Length
	0x02,						// Type (Config)
	0x19, 0x00,			// Total size

	0x01,						// # interfaces
	0x01,						// Configuration #
	0x00,						// index of string descriptor
	0x80,						// Attributes (bus powered)
	0x19,						// Max power in mA


	// Interface
	0x09,						// Length
	0x04,						// Type (Interface)

	0x00,						// Interface #
	0x00,						// Alternative #
	0x01,						// # endpoints

	0xff,						// Class
	0x6a,						// Subclass
	0x00,						// Protocol
	0x00,						// index of string descriptor


	// Endpoint
	0x07,						// Length
	0x05,						// Type (Endpoint)

	0x81,						// Address
	0x03,						// Attributes (interrupt transfers)
	0x08, 0x00,			// Max packet size

	0x08,						// Polling interval (frame counts)
};

// =======================================
// =======================================
// =======================================

static FILE *fp_guncon_debug = 0;

void Update_Guncon2( int device_port )
{
	int reload;

	int data_x, data_y, data_z;
	int data_absolute;

	int gun_width, gun_height;
	int gun_right, gun_bottom;


	pad_active = device_port;
	reload = 0;


	// skip update
	if( port_type[ THIS_PAD ] != PAD_TYPE_GUNCON ) return;


	// guncon - average active window
	GUNCON_WIDTH = guncon_width[ THIS_PAD ];
	GUNCON_HEIGHT = guncon_height[ THIS_PAD ];

	// ===================================
	// ===================================
	// ===================================

	data_x = device_x[ device_hid[ THIS_PAD ] ];
	data_y = device_y[ device_hid[ THIS_PAD ] ];
	data_z = device_z[ device_hid[ THIS_PAD ] ];


	for( int lcv = 0; lcv < NUM_MOUSE_BUTTONS; lcv++ ) {
		guncon_buttons[ THIS_PAD ][ lcv ] = device_buttons[ device_hid[ THIS_PAD ] ][ lcv ];
	}

	data_absolute = device_absolute[ device_hid[ THIS_PAD ] ];


#if 0
	//if( pad_active == 2 )
	{
		if( fp_guncon_debug == 0 ) {
			fp_guncon_debug = fopen( "guncon2-debug.txt", "w" );
		}

		fprintf( fp_guncon_debug, "(update) (%d)  ||  (x,y) = %d %d  ||  (delta) = %d %d\n",
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
		//if( pad_active == 2 )
		{
			if( fp_guncon_debug == 0 ) {
				fp_guncon_debug = fopen( "guncon2-debug.txt", "w" );
			}

			fprintf( fp_guncon_debug, "(relative) %d %d || %d %d\n",
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


			// aiming scaling
			data_x = ( data_x * guncon_scale_x[ THIS_PAD ] ) / 100;
			data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;


			// re-adjust to guncon center
			data_x += guncon_screen_center_x[ THIS_PAD ];
			data_y += guncon_screen_center_y[ THIS_PAD ];


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


			// aiming scaling
			data_x = ( data_x * guncon_scale_x[ THIS_PAD ] ) / 100;
			data_y = ( data_y * guncon_scale_y[ THIS_PAD ] ) / 100;


			// re-adjust to guncon center
			data_x += guncon_screen_center_x[ THIS_PAD ];
			data_y += guncon_screen_center_y[ THIS_PAD ];


			// special edge check (valid data)
			if( data_x < 1 ) data_x = 1;
			if( data_y < 1 ) data_y = 1;
		}
	}


	// save for later
	current_analog_x[ THIS_PAD ] = data_x;
	current_analog_y[ THIS_PAD ] = data_y;
	current_analog_z[ THIS_PAD ] = data_z;


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
			if( pad_active == 2 &&
					port_type[0] == PAD_TYPE_GUNCON &&
					port_type[1] == PAD_TYPE_GUNCON &&
					guncon_cursor[0] == GUNCON_SHOW_CURSOR &&
					guncon_cursor[1] == GUNCON_SHOW_CURSOR )
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


#if 0
				//if( pad_active == 2 )
				{
					if( fp_guncon_debug == 0 ) {
						fp_guncon_debug = fopen( "guncon2-debug.txt", "w" );
					}

					fprintf( fp_guncon_debug, "(win32 cursor) %d %d  ||  %d %d %d %d\n",
						x,y,
						r.left, r.top, r.right, r.bottom );
				}
#endif


				SetCursorPos( x,y );
			}
		} // end gpu + win32
	} // end cursor
}


int Poll_Guncon2( int device_port, u16 *pad_out )
{
	int reload;
	int data_x, data_y, data_z;


	pad_active = device_port;
	reload = 0;


	// guncon - average active window
	GUNCON_WIDTH = guncon_width[ THIS_PAD ];
	GUNCON_HEIGHT = guncon_height[ THIS_PAD ];


	// restore from cache
	data_x = current_analog_x[ THIS_PAD ];
	data_y = current_analog_y[ THIS_PAD ];
	data_z = current_analog_z[ THIS_PAD ];



	if( DEVICE.vars[ GUNCON2_PARAM_MODE ] & PROGRESSIVE_SCAN ) {
		// interlace mode
		data_x -= (s16) ( DEVICE.vars[ GUNCON2_PARAM_X ] ) / 2;
		data_y -= (s16) ( DEVICE.vars[ GUNCON2_PARAM_Y ] ) / 2;
	}

	else {
		// progressive mode
		data_x -= (s16) ( DEVICE.vars[ GUNCON2_PARAM_X ] );
		data_y -= (s16) ( DEVICE.vars[ GUNCON2_PARAM_Y ] );
	}


	// clip to guncon window
	if( data_x < 0 ) data_x = 0;
	if( data_y < 0 ) data_y = 0;

	// ===================================
	// ===================================
	// ===================================

	// buttons (default)
	pad_out[0] = 0xffff & ~PROGRESSIVE_SCAN;


	// usually tv scan flag (mode $0100 = interlace)
	pad_out[0] |= DEVICE.vars[ GUNCON2_PARAM_MODE ];



	// read mouse buttons
	for( int lcv = 0; lcv < 7; lcv++ ) {
		int mode;


		// check not pushed
		if( lcv < 5 ) {
			if( guncon_buttons[ THIS_PAD ][lcv] == 0 ) continue;
		}

		// assert wheel up
		else if( lcv == 5 ) {
			if( data_z <= 0 ) continue;
		}

		// assert wheel down
		else if( lcv == 6 ) {
			if( data_z >= 0 ) continue;
		}


		switch( lcv ) {
			case 0: mode = guncon_left[ THIS_PAD ]; break;
			case 1: mode = guncon_right[ THIS_PAD ]; break;
			case 2: mode = guncon_middle[ THIS_PAD ]; break;
			case 3: mode = guncon_aux1[ THIS_PAD ]; break;
			case 4: mode = guncon_aux2[ THIS_PAD ]; break;
			case 5: mode = guncon_wheelup[ THIS_PAD ]; break;
			case 6: mode = guncon_wheeldown[ THIS_PAD ]; break;
		}


		switch( mode ) {
		case GUNCON_RELOAD:
			pad_out[0] &= ~BUTTON_TRIGGER;

			data_x = 0;
			data_y = 0;

			reload = 1;
			break;


		case GUNCON_TRIGGER:
			pad_out[0] &= ~BUTTON_TRIGGER;

			// black screen flash detection
			if( latch_trigger_delay[ THIS_PAD ] == 0 )
			{
				latch_trigger_delay[ THIS_PAD ] = LATCH_DELAY;
				latch_trigger_x[ THIS_PAD ] = data_x;
				latch_trigger_y[ THIS_PAD ] = data_y;
			}
			break;


		case GUNCON_A: pad_out[0] &= ~BUTTON_A; break;
		case GUNCON_B: pad_out[0] &= ~BUTTON_B; break;
		case GUNCON_C: pad_out[0] &= ~BUTTON_C; break;

		case GUNCON_START: pad_out[0] &= ~BUTTON_START; break;
		case GUNCON_SELECT: pad_out[0] &= ~BUTTON_SELECT; break;

		case GUNCON_DPAD_UP: pad_out[0] &= ~DPAD_UP; break;
		case GUNCON_DPAD_DOWN: pad_out[0] &= ~DPAD_DOWN; break;
		case GUNCON_DPAD_LEFT: pad_out[0] &= ~DPAD_LEFT; break;
		case GUNCON_DPAD_RIGHT: pad_out[0] &= ~DPAD_RIGHT; break;

		case GUNCON_DPAD_A_SELECT: pad_out[0] &= ~BUTTON_A; pad_out[0] &= ~BUTTON_SELECT; break;
		case GUNCON_DPAD_B_SELECT: pad_out[0] &= ~BUTTON_B; pad_out[0] &= ~BUTTON_SELECT; break;

		case GUNCON_DPAD_UP_SELECT: pad_out[0] &= ~DPAD_UP; pad_out[0] &= ~BUTTON_SELECT; break;
		case GUNCON_DPAD_DOWN_SELECT: pad_out[0] &= ~DPAD_DOWN; pad_out[0] &= ~BUTTON_SELECT; break;
		case GUNCON_DPAD_LEFT_SELECT: pad_out[0] &= ~DPAD_LEFT; pad_out[0] &= ~BUTTON_SELECT; break;
		case GUNCON_DPAD_RIGHT_SELECT: pad_out[0] &= ~DPAD_RIGHT; pad_out[0] &= ~BUTTON_SELECT; break;
		}
	}



	// read keyboard dpad
	if( guncon_keyboard_dpad[ THIS_PAD ] ) {
		if( keyboard_state[0][ DIK_W ] & 0x80 ) pad_out[0] &= ~DPAD_UP;
		if( keyboard_state[0][ DIK_A ] & 0x80 ) pad_out[0] &= ~DPAD_LEFT;
		if( keyboard_state[0][ DIK_S ] & 0x80 ) pad_out[0] &= ~DPAD_DOWN;
		if( keyboard_state[0][ DIK_D ] & 0x80 ) pad_out[0] &= ~DPAD_RIGHT;

		if( keyboard_state[0][ DIK_Q ] & 0x80 ) pad_out[0] &= ~BUTTON_START;
		if( keyboard_state[0][ DIK_E ] & 0x80 ) pad_out[0] &= ~BUTTON_SELECT;
		if( keyboard_state[0][ DIK_F ] & 0x80 ) pad_out[0] &= ~BUTTON_C;
	}


	// START hack = A + B + TRIGGER
	if( guncon_start_hotkey[ THIS_PAD ] )
	{
		if( (~pad_out[0] & BUTTON_A) && 
				(~pad_out[0] & BUTTON_B) &&
				(~pad_out[0] & BUTTON_TRIGGER ) )
		{
			pad_out[0] &= ~BUTTON_START;
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
				pad_out[0] |= BUTTON_TRIGGER;
				reload = 0;
			}

			else if( guncon_reload_auto < 1+6 ) {
				pad_out[0] &= ~BUTTON_TRIGGER;
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
	//if( pad_active == 2 )
	{
		if( fp_guncon_debug == 0 ) {
			fp_guncon_debug = fopen( "guncon2-debug.txt", "w" );
		}

		fprintf( fp_guncon_debug, "(Poll) (%d) [%d %d] %d %d  +  %d %d [%d %d | %d %d %d %d]\n",
			THIS_PAD+1,
			gpu_screen_x, gpu_screen_y,
			guncon_analog_x[ THIS_PAD ], guncon_analog_y[ THIS_PAD ],
			data_x, data_y,
			GUNCON_WIDTH, GUNCON_HEIGHT,
			GUNCON_LEFT, GUNCON_TOP, GUNCON_RIGHT, GUNCON_BOTTOM );
	}
#endif

	// ==================================
	// ==================================
	// ==================================

	// offscreen reload - manual trigger
	if( data_x == 0 || data_y == 0 )
	{
		// trigger pulled
		if( ~pad_out[0] & BUTTON_TRIGGER )
			reload = 1;
	}

	// ==================================
	// ==================================
	// ==================================

	// analog x-y
	pad_out[1] = (s16) data_x;
	pad_out[2] = (s16) data_y;


	// mouse only!!
	//
	// black screen flash detection (calibration hack)
	if( guncon_calibrate[ THIS_PAD ] )
	{
		if( latch_trigger_delay[ THIS_PAD ] > 0 ) {
			// still force trigger down - calibration timing
			pad_out[0] &= ~BUTTON_TRIGGER;


			// check screen flash
			//MessageBox(0,0,0,0);


			// use trigger latch data - calibration timing
			pad_out[1] = (s16) latch_trigger_x[ THIS_PAD ];
			pad_out[2] = (s16) latch_trigger_y[ THIS_PAD ];


			latch_trigger_delay[ THIS_PAD ]--;
			if( latch_trigger_delay[ THIS_PAD ] == 0 )
			{
				// black screen = no data
				pad_out[1] = 0;
				pad_out[2] = 0;
			}
		}
	} // end calibration hack


	// special handling
	if( reload == 1 ) {
		pad_out[1] = 0;
		pad_out[2] = 0;
	}


	// Gunfighter 2 - only exact # bytes returned
	return 6;
}

// =======================================
// =======================================
// =======================================

void Guncon2_Request_Class()
{
	if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_INTERFACE ) {
		switch( DEVICE.req ) {
			case GUNCON2_SET_PARAM:
				// expect 6 bytes - y / x / mode
				USB_LOG( "\t\tGuncon2 - set parameters\n" );
				break;


			default:
				USB_LOG( "\t\t***  Unused Guncon2 request type\n" );
				break;
		}
	}

	else {
		USB_LOG( "\t\t***  Unused Guncon2 request type\n" );
	}
}


void Guncon2_Request_Out()
{
	if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_INTERFACE ) {
		switch( DEVICE.req ) {
			case GUNCON2_SET_PARAM:
				// 6 bytes = y / x / mode
				DEVICE.vars[ GUNCON2_PARAM_X ] = DEVICE.data_buf[0] | ( DEVICE.data_buf[1] << 8 );
				DEVICE.vars[ GUNCON2_PARAM_Y ] = DEVICE.data_buf[2] | ( DEVICE.data_buf[3] << 8 );
				DEVICE.vars[ GUNCON2_PARAM_MODE ] = DEVICE.data_buf[4] | ( DEVICE.data_buf[5] << 8 );


				USB_LOG( "\t\t\tGuncon2 parameters written\n" );
				break;
		}
	}
}
