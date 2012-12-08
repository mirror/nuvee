void PADpoll_mouse( int value )
{
	int data_x, data_y, data_z;
	int data_buttons[3];



	if( pad_count == 0 ) {
		pad_cmd = value;


		data_x = device_x[ device_hid[ THIS_PAD ] ];
		data_y = device_y[ device_hid[ THIS_PAD ] ];
		data_z = device_z[ device_hid[ THIS_PAD ] ];

		data_buttons[0] = device_buttons[ device_hid[ THIS_PAD ] ][0];
		data_buttons[1] = device_buttons[ device_hid[ THIS_PAD ] ][1];
		data_buttons[2] = device_buttons[ device_hid[ THIS_PAD ] ][2];
	}


	switch( pad_cmd ) {
		// $42 = poll data
		case 0x42:
			// already done
			if( pad_count > 0 ) break;

			// ==================================
			// ==================================
			// ==================================

			// reset config mode
			if( pad_cmd == 0x42 ) {
				pad_config_mode[ THIS_PAD ] = 0;
			}


			// mouse ID
			pad_out[0] = 0x12;
			pad_out[1] = 0x5a;

			// ==================================
			// ==================================
			// ==================================

			// mouse buttons (default)
			pad_out[2] = 0xff;
			pad_out[3] = 0xfc;


			if( data_buttons[0] )
				pad_out[3] &= ~0x08;

			if( data_buttons[1] )
				pad_out[3] &= ~0x04;



			// special 3rd button + mousewheel
			if( mouse_usb_converter[ THIS_PAD ] ) {
				if( data_buttons[2] )
					pad_out[3] &= ~0x80;

				if( data_z > 0 )
					pad_out[3] &= ~0x40;

				if( data_z < 0 )
					pad_out[3] &= ~0x20;
			}

			// ==================================
			// ==================================
			// ==================================

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

			// ==================================
			// ==================================
			// ==================================

			// mouse analog
			pad_out[4] = (s8) data_x;
			pad_out[5] = (s8) data_y;
			break;


		default:
			// already done
			if( pad_count > 0 ) break;


			pad_out[0] = 0x12;
			pad_out[1] = 0xff;

			// invalid command
			pad_out[2] = 0xff;
			pad_out[3] = 0xff;

			pad_out[4] = 0xff;
			pad_out[5] = 0xff;
			break;
	}
}
