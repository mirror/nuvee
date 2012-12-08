u8 DIK_to_USB[256];


// near-Dell USB keyboard
u8 keyboard_device_descriptor[] = {
	0x12,						// Length
	0x01,						// Type (Descriptor)
	0x01, 0x01,			// USB version # (1.1)

	0x00,						// Class
	0x00,						// Subclass
	0x00,						// Protocol
	0x08,						// Max Packet Size

	0x3c, 0x41,			// Vendor ID (Dell)
	0x10, 0x20,			// Product ID (USB keyboard)
	0x00, 0x01,			// Device version # (1.0)

	0x00,						// Manufacturer string index #
	0x00,						// Product string index #
	0x00,						// Serial number string index #
	0x01,						// # configurations
};


// near-Dell USB keyboard
u8 keyboard_config_descriptor[] = {
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
	0x01,						// Protocol (Keyboard)
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
	0x08, 0x00,			// Max packet size

	0x08,						// Polling interval (frame counts) (** normally 0x0a)
};


u8 keyboard_report_descriptor[] = {
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

// =======================================
// =======================================
// =======================================

void Create_Keyboard_Table()
{
	// DInput keyboard --> Microsoft standard (USB)
	DIK_to_USB[ DIK_ESCAPE ] = 0x29;
	DIK_to_USB[ DIK_1 ] = 0x1e;
	DIK_to_USB[ DIK_2 ] = 0x1f;
	DIK_to_USB[ DIK_3 ] = 0x20;
	DIK_to_USB[ DIK_4 ] = 0x21;
	DIK_to_USB[ DIK_5 ] = 0x22;
	DIK_to_USB[ DIK_6 ] = 0x23;
	DIK_to_USB[ DIK_7 ] = 0x24;
	DIK_to_USB[ DIK_8 ] = 0x25;
	DIK_to_USB[ DIK_9 ] = 0x26;
	DIK_to_USB[ DIK_0 ] = 0x27;
	DIK_to_USB[ DIK_MINUS ] = 0x2d;
	DIK_to_USB[ DIK_EQUALS ] = 0x2e;
	DIK_to_USB[ DIK_BACKSPACE ] = 0x2a;

	DIK_to_USB[ DIK_TAB ] = 0x2b;
	DIK_to_USB[ DIK_Q ] = 0x14;
	DIK_to_USB[ DIK_W ] = 0x1a;
	DIK_to_USB[ DIK_E ] = 0x08;
	DIK_to_USB[ DIK_R ] = 0x15;
	DIK_to_USB[ DIK_T ] = 0x17;
	DIK_to_USB[ DIK_Y ] = 0x1c;
	DIK_to_USB[ DIK_U ] = 0x18;
	DIK_to_USB[ DIK_I ] = 0x0c;
	DIK_to_USB[ DIK_O ] = 0x12;
	DIK_to_USB[ DIK_P ] = 0x13;
	DIK_to_USB[ DIK_LBRACKET ] = 0x2f;
	DIK_to_USB[ DIK_RBRACKET ] = 0x30;
	DIK_to_USB[ DIK_RETURN ] = 0x28;
	DIK_to_USB[ DIK_LCONTROL ] = 0xe0;

	DIK_to_USB[ DIK_A ] = 0x04;
	DIK_to_USB[ DIK_S ] = 0x16;
	DIK_to_USB[ DIK_D ] = 0x07;
	DIK_to_USB[ DIK_F ] = 0x09;
	DIK_to_USB[ DIK_G ] = 0x0a;
	DIK_to_USB[ DIK_H ] = 0x0b;
	DIK_to_USB[ DIK_J ] = 0x0d;
	DIK_to_USB[ DIK_K ] = 0x0e;
	DIK_to_USB[ DIK_L ] = 0x0f;
	DIK_to_USB[ DIK_SEMICOLON ] = 0x33;
	DIK_to_USB[ DIK_APOSTROPHE ] = 0x34;
	DIK_to_USB[ DIK_GRAVE ] = 0x35;
	DIK_to_USB[ DIK_LSHIFT ] = 0xe1;
	DIK_to_USB[ DIK_BACKSLASH ] = 0x31;

	DIK_to_USB[ DIK_Z ] = 0x1d;
	DIK_to_USB[ DIK_X ] = 0x1b;
	DIK_to_USB[ DIK_C ] = 0x06;
	DIK_to_USB[ DIK_V ] = 0x19;
	DIK_to_USB[ DIK_B ] = 0x05;
	DIK_to_USB[ DIK_N ] = 0x11;
	DIK_to_USB[ DIK_M ] = 0x10;
	DIK_to_USB[ DIK_COMMA ] = 0x36;
	DIK_to_USB[ DIK_PERIOD ] = 0x37;
	DIK_to_USB[ DIK_SLASH ] = 0x38;
	DIK_to_USB[ DIK_RSHIFT ] = 0xe5;
	DIK_to_USB[ DIK_NUMPADSTAR ] = 0x55;
	DIK_to_USB[ DIK_LALT ] = 0xe2;
	DIK_to_USB[ DIK_SPACE ] = 0x2c;
	DIK_to_USB[ DIK_CAPSLOCK ] = 0x39;

	DIK_to_USB[ DIK_F1 ] = 0x3a;
	DIK_to_USB[ DIK_F2 ] = 0x3b;
	DIK_to_USB[ DIK_F3 ] = 0x3c;
	DIK_to_USB[ DIK_F4 ] = 0x3d;
	DIK_to_USB[ DIK_F5 ] = 0x3e;
	DIK_to_USB[ DIK_F6 ] = 0x3f;
	DIK_to_USB[ DIK_F7 ] = 0x40;
	DIK_to_USB[ DIK_F8 ] = 0x41;
	DIK_to_USB[ DIK_F9 ] = 0x42;
	DIK_to_USB[ DIK_F10 ] = 0x43;

	DIK_to_USB[ DIK_NUMLOCK ] = 0x53;
	DIK_to_USB[ DIK_SCROLL ] = 0x47;
	DIK_to_USB[ DIK_NUMPAD7 ] = 0x5f;
	DIK_to_USB[ DIK_NUMPAD8 ] = 0x60;
	DIK_to_USB[ DIK_NUMPAD9 ] = 0x61;
	DIK_to_USB[ DIK_NUMPADMINUS ] = 0x56;
	DIK_to_USB[ DIK_NUMPAD4 ] = 0x5c;
	DIK_to_USB[ DIK_NUMPAD5 ] = 0x97;
	DIK_to_USB[ DIK_NUMPAD6 ] = 0x5e;
	DIK_to_USB[ DIK_NUMPADPLUS ] = 0x57;
	DIK_to_USB[ DIK_NUMPAD1 ] = 0x59;
	DIK_to_USB[ DIK_NUMPAD2 ] = 0x5a;
	DIK_to_USB[ DIK_NUMPAD3 ] = 0x5b;
	DIK_to_USB[ DIK_NUMPAD0 ] = 0x62;
	DIK_to_USB[ DIK_NUMPADPERIOD ] = 0x63;

	DIK_to_USB[ DIK_F11 ] = 0x44;
	DIK_to_USB[ DIK_F12 ] = 0x45;
	DIK_to_USB[ DIK_NUMPADENTER ] = 0x58;
	DIK_to_USB[ DIK_RCONTROL ] = 0xe4;
	DIK_to_USB[ DIK_NUMPADSLASH ] = 0x54;
	DIK_to_USB[ DIK_RALT ] = 0xe6;
	DIK_to_USB[ DIK_HOME ] = 0x4a;
	DIK_to_USB[ DIK_UPARROW ] = 0x52;
	DIK_to_USB[ DIK_PGUP ] = 0x4b;
	DIK_to_USB[ DIK_LEFTARROW ] = 0x50;
	DIK_to_USB[ DIK_RIGHTARROW ] = 0x4f;
	DIK_to_USB[ DIK_END ] = 0x4d;
	DIK_to_USB[ DIK_DOWNARROW ] = 0x51;
	DIK_to_USB[ DIK_PGDN ] = 0x4e;
	DIK_to_USB[ DIK_INSERT ] = 0x49;
	DIK_to_USB[ DIK_DELETE ] = 0x4c;
	DIK_to_USB[ DIK_LWIN ] = 0xe3;
	DIK_to_USB[ DIK_RWIN ] = 0xe7;
}


void Update_Keyboard( int device_port )
{
	if( DIK_to_USB[ DIK_ESCAPE ] != 0x29 )
		Create_Keyboard_Table();


	pad_active = device_port;


	// assume only 1 keyboard usable
}


int Poll_Keyboard( int device_port, u8 *pad_out )
{
	int key_buf, lcv;


	// 8 bytes total
	memset( pad_out, 0, 8 );


	// special keys
	if( di_keystate[ DIK_LCONTROL ] )	pad_out[0] |= (1<<0);
	if( di_keystate[ DIK_LSHIFT ] )		pad_out[0] |= (1<<1);
	if( di_keystate[ DIK_LALT ] )			pad_out[0] |= (1<<2);
	if( di_keystate[ DIK_LWIN ] )			pad_out[0] |= (1<<3);

	if( di_keystate[ DIK_RCONTROL ] )	pad_out[0] |= (1<<4);
	if( di_keystate[ DIK_RSHIFT ] )		pad_out[0] |= (1<<5);
	if( di_keystate[ DIK_RALT ] )			pad_out[0] |= (1<<6);
	if( di_keystate[ DIK_RWIN ] )			pad_out[0] |= (1<<7);


	// Unreal Tournament - up to 6 normal keys (start at 2!)
	key_buf = 2;
	for( lcv = 0; lcv < 256; lcv++ ) {
		// skip unused - not pressed
		if( DIK_to_USB[ lcv ] == 0 ) continue;
		if( di_keystate[ lcv ] == 0 ) continue;


		// add key to list
		pad_out[ key_buf++ ] = DIK_to_USB[ lcv ];

		// buffer full
		if( key_buf == 8 ) break;
	}


	return 8;
}
