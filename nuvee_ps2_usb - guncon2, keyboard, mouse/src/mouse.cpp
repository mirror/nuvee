// author: PS2Dev
#define BUTTON_TRIGGER			0x2000
#define BUTTON_A						0x0008
#define BUTTON_B						0x0004
#define BUTTON_C						0x0002
#define BUTTON_SELECT				0x4000
#define BUTTON_START				0x8000


// near-Logitech settings
u8 mouse_device_descriptor[] = {
	0x12,						// Length
	0x01,						// Type (Descriptor)
	0x01, 0x01,			// USB version # (1.1) (** normally 2.0)

	0x00,						// Class
	0x00,						// Subclass
	0x00,						// Protocol
	0x08,						// Max Packet Size

	0x6d, 0x04,			// Vendor ID (Logitech)
	0x51, 0xc0,			// Product ID (Optical mouse)
	0x00, 0x01,			// Device version # (1.0)

	0x00,						// Manufacturer string index #
	0x00,						// Product string index #
	0x00,						// Serial number string index #
	0x01,						// # configurations
};


// near-Logitech settings
u8 mouse_config_descriptor[] = {
	0x09,						// Length
	0x02,						// Type (Config)
	0x22, 0x00,			// Total size

	0x01,						// # interfaces
	0x01,						// Configuration #
	0x00,						// index of string descriptor
	0x80,						// Attributes (bus powered) (** 0xa0 = wakeup also)
	0x32,						// Max power in mA


	// Interface
	0x09,						// Length
	0x04,						// Type (Interface)

	0x00,						// Interface #
	0x00,						// Alternative #
	0x01,						// # endpoints

	0x03,						// Class (HID)
	0x01,						// Subclass (Boot)
	0x02,						// Protocol (Mouse)
	0x00,						// index of string descriptor


			// HID descriptor
			0x09,					//  Length
			0x21,					//  Descriptor Type

			0x10, 0x01,		//  Version #
			0x00,					//  country_code
			0x01,					//  num_descriptors

			0x22,					//  Type (Report)
			0x34, 0x00,		//  length (report) (** make sure this matches below)


	// Endpoint
	0x07,						// Length
	0x05,						// Type (Endpoint)

	0x81,						// Address, IN = Endpoint 1
	0x03,						// Attributes (interrupt transfers)
	0x04, 0x00,			// Max packet size (** normally 0x08)

	0x08,						// Polling interval (frame counts) (** normally 0x0a)
};


// near-Logitech settings
u8 mouse_report_descriptor[] = {
	0x05, 0x01,			// Usage Page (Generic Desktop)
	
	0x09, 0x02,			// Usage (Mouse)
	0xa1, 0x01,			// - Collection (Application)
	0x09, 0x01,			// -- Usage (Pointer)
	0xa1, 0x00,			// --- Collection (Physical)

	0x05, 0x09,			// Usage Page (Button)
	0x19, 0x01,			// Minimum = 1 button
	0x29, 0x03,			// Maximum = 3 buttons
	0x15, 0x00,			// Logical min = 0
	0x25, 0x01,			// Logical max = 1

	// check this order: 2.0 USB (1.1 USB flips?)
	0x75, 0x01,			// Report Size: 1
	0x95, 0x03,			// Report Count: 3 buttons
	0x81, 0x02,			// Input: data, variable, absolute

	// check this order: 2.0 USB (1.1 USB flips?)
	0x75, 0x05,			// Report Size: 5
	0x95, 0x01,			// Report Count: 1
	0x81, 0x01,			// Input: constant, variable, absolute (** not 0x03)


	0x05, 0x01,			// Usage Page (Generic Desktop)
	0x09, 0x30,			// Usage (X)
	0x09, 0x31,			// Usage (Y)
	0x09, 0x38,			// Usage (Wheel)
	0x15, 0x81,			// Logical Minimum: -127
	0x25, 0x7f,			// Logical Maximum: +127
	0x75, 0x08,			// Report Size: 8
	0x95, 0x03,			// Report Count: 3
	0x81, 0x06,			// Input: Data, variable, relative
	
	0xc0,						// End collection
	0xc0,						// End collection
};



// PS2Dev
#define PS2MOUSE_READMODE_DIFF 0
#define PS2MOUSE_READMODE_ABS 1

#define PS2MOUSE_BTN1    1
#define PS2MOUSE_BTN2    2
#define PS2MOUSE_BTN3    4

#define PS2MOUSE_BIND_RPC_ID 0x500C001
#define PS2MOUSE_READ 0x1
#define PS2MOUSE_SETREADMODE 0x2
#define PS2MOUSE_GETREADMODE 0x3
#define PS2MOUSE_SETTHRES 0x4
#define PS2MOUSE_GETTHRES 0x5
#define PS2MOUSE_SETACCEL 0x6
#define PS2MOUSE_GETACCEL 0x7
#define PS2MOUSE_SETBOUNDARY 0x8
#define PS2MOUSE_GETBOUNDARY 0x9
#define PS2MOUSE_SETPOSITION 0xA
#define PS2MOUSE_RESET 0xB
#define PS2MOUSE_ENUM 0xC
#define PS2MOUSE_SETDBLCLICKTIME 0xD
#define PS2MOUSE_GETDBLCLICKTIME 0xE
#define PS2MOUSE_GETVERSION 0x20

// =======================================
// =======================================
// =======================================

void Update_Mouse( int device_port )
{
	pad_active = device_port;


	// skip update
	if( port_type[ THIS_PAD ] != PAD_TYPE_MOUSE ) return;


	// ===================================
	// ===================================
	// ===================================

	mouse_x[ THIS_PAD ] = device_x[ device_hid[ THIS_PAD ] ];
	mouse_y[ THIS_PAD ] = device_y[ device_hid[ THIS_PAD ] ];
	mouse_z[ THIS_PAD ] = device_z[ device_hid[ THIS_PAD ] ];


	for( int lcv = 0; lcv < NUM_MOUSE_BUTTONS; lcv++ ) {
		mouse_buttons[ THIS_PAD ][ lcv ] = device_buttons[ device_hid[ THIS_PAD ] ][ lcv ];
	}



	// force cursor off
	ShowCursor(0);
}


int Poll_Mouse( int device_port, u8 *pad_out )
{
	int data_x, data_y;


	pad_active = device_port;


	data_x = mouse_x[ THIS_PAD ];
	data_y = mouse_y[ THIS_PAD ];

	// ================================
	// ================================
	// ================================

	data_x = ( data_x * mouse_sensitivity[ THIS_PAD ] ) / 100;
	data_y = ( data_y * mouse_sensitivity[ THIS_PAD ] ) / 100;


	if( data_x < -0x80 ) data_x = -0x80;
	if( data_y < -0x80 ) data_y = -0x80;
	if( data_x > 0x7f ) data_x = 0x7f;
	if( data_y > 0x7f ) data_y = 0x7f;


	if( data_x > mouse_threshold[ THIS_PAD ] )
		data_x = mouse_threshold[ THIS_PAD ];

	if( data_y > mouse_threshold[ THIS_PAD ] )
		data_y = mouse_threshold[ THIS_PAD ];

	if( data_x < -mouse_threshold[ THIS_PAD ] )
		data_x = -mouse_threshold[ THIS_PAD ];

	if( data_y < -mouse_threshold[ THIS_PAD ] )
		data_y = -mouse_threshold[ THIS_PAD ];


	if( data_x >= -mouse_deadzone[ THIS_PAD ] &&
			data_x <= mouse_deadzone[ THIS_PAD ] )
		data_x = 0;

	if( data_y >= -mouse_deadzone[ THIS_PAD ] &&
			data_y <= mouse_deadzone[ THIS_PAD ] )
		data_y = 0;

	// ================================
	// ================================
	// ================================

	// Unreal Tournament / Silent Scope
	pad_out[0] = 0x00;
	if( mouse_buttons[ THIS_PAD ][0] ) pad_out[0] |= 1;
	if( mouse_buttons[ THIS_PAD ][1] ) pad_out[0] |= 2;
	if( mouse_buttons[ THIS_PAD ][2] ) pad_out[0] |= 4;

	pad_out[1] = data_x;
	pad_out[2] = data_y;
	pad_out[3] = mouse_z[ THIS_PAD ];


	// mouse transfer size
	return 4;
}

