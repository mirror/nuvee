/*  USBlinuz
 *  Copyright (C) 2002-2004  USBlinuz Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/* Information taken from usb-ohci.c
 * QEMU USB OHCI Interface
 * Copyright (c) 2004 Gianni Tedesco
 * Copyright (c) 2004-2005 Innotek Systemberatung GmbH
 *               Author: Gianni Tedesco
*/


/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
*/



#include "usb_device.h"

#define DEVICE (usb_dev->list[ device_active ])


int device_active;
int device_port;


#include "guncon2.cpp"
#include "keyboard.cpp"
#include "mouse.cpp"



void Reset_Devices()
{
	usb_dev = (struct usb_device_list *) usbdevice;
	memset( usb_dev->list, 0, sizeof(usb_device_list) );
}


void Attach_Devices()
{
	// nuvee port0 = device 0-7
	if( port_type[0] != PAD_TYPE_NONE ) {
		usb_dev->list[0].connected = 1;
		usb_dev->list[0].lowspeed = 1;
	}


	// nuvee port1 = device 8-15
	if( port_type[1] != PAD_TYPE_NONE ) {
		usb_dev->list[8].connected = 1;
		usb_dev->list[8].lowspeed = 1;
	}
}


void Poll_Devices( int frame_no )
{
	if( port_type[0] == PAD_TYPE_MOUSE || port_type[1] == PAD_TYPE_MOUSE ||
			port_type[0] == PAD_TYPE_GUNCON || port_type[1] == PAD_TYPE_GUNCON )
		SnapCursor();



	// skip polling - no devices attached
	if( port_type[0] == PAD_TYPE_NONE && port_type[1] == PAD_TYPE_NONE ) return;



	// Latch keyboard every 8 ms (assume low-speed device)
	// - Seems okay for Dirge of Cerberus (8 ms)
	if( (frame_no & 7) == 0 )
	{
		Poll_Keyboard_DInput();


		// save last copy in case it's needed
		memcpy( keyboard_state[1], keyboard_state[0], sizeof(keyboard_state[1]) );
		memcpy( keyboard_state[0], di_keystate, sizeof(keyboard_state[0]) );


		Update_Keyboard(1);
		Update_Keyboard(2);
	}



	// Latch USB mouse every 8 ms
	// - Dirge of Cerberus (USB Mouse) (in-game = little smoother at 16 ms)
#if 1
	if( (frame_no & 15) == 0 )
	{
		if( rawinput_active )
			Poll_RawInput();
		else
			Poll_Mouse_DInput();


		Poll_Joystick_DInput();


		switch( port_type[0] ) {
		case PAD_TYPE_GUNCON: Update_Guncon2(1); break;
		case PAD_TYPE_MOUSE: Update_Mouse(1); break;
		}


		switch( port_type[1] ) {
		case PAD_TYPE_GUNCON: Update_Guncon2(2); break;
		case PAD_TYPE_MOUSE: Update_Mouse(2); break;
		}
	}
#else
	if( port_type[0] != PAD_TYPE_GUNCON && port_type[1] != PAD_TYPE_GUNCON ) {
		if( (frame_no & 15) == 0 )
		{
			if( rawinput_active )
				Poll_RawInput();
			else
				Poll_Mouse_DInput();


			Poll_Joystick_DInput();


			Update_Mouse(1);
			Update_Mouse(2);
		}
	}


	// Guncon2 should latch every VBlank (16.6666 or 20)
	// - round to 16 ms
	else if( (frame_no & 15) == 0 )
	{
		if( rawinput_active )
			Poll_RawInput();
		else
			Poll_Mouse_DInput();


		Poll_Joystick_DInput();


		Update_Guncon2(1);
		Update_Guncon2(2);
	}
#endif
}


int Read_Device_Data( u8 *td_buf )
{
	int transfer_bytes;


	switch( port_type[ device_port ] ) {
		case PAD_TYPE_GUNCON:
			// Gunfighter 2 - checks for 6 bytes, not max 8
			transfer_bytes = Poll_Guncon2( device_port+1, (u16 *) td_buf );
			break;


		case PAD_TYPE_KEYBOARD:
			transfer_bytes = Poll_Keyboard( device_port+1, (u8 *) td_buf );
			break;


		case PAD_TYPE_MOUSE:
			transfer_bytes = Poll_Mouse( device_port+1, (u8 *) td_buf );
			break;


		default:
			// no device connected
			transfer_bytes = -1;
			break;
	}


	return transfer_bytes;
}


u8 *Get_Device_Descriptor()
{
	switch( port_type[ device_port ] ) {
		case PAD_TYPE_GUNCON:
			if( ( DEVICE.req_arg1 >> 8 ) == 1 ) return guncon2_device_descriptor;
			if( ( DEVICE.req_arg1 >> 8 ) == 2 ) return guncon2_config_descriptor;
			break;

		case PAD_TYPE_KEYBOARD:
			if( ( DEVICE.req_arg1 >> 8 ) == 1 ) return keyboard_device_descriptor;
			if( ( DEVICE.req_arg1 >> 8 ) == 2 ) return keyboard_config_descriptor;
			break;

		case PAD_TYPE_MOUSE:
			if( ( DEVICE.req_arg1 >> 8 ) == 1 ) return mouse_device_descriptor;
			if( ( DEVICE.req_arg1 >> 8 ) == 2 ) return mouse_config_descriptor;
			break;
	}
}


void Device_Request_Out()
{
	switch( port_type[ device_port ] ) {
		case PAD_TYPE_GUNCON: Guncon2_Request_Out(); break;

		default:
			USB_LOG( "\t\t\t*** Unknown device request out\n" );
			break;
	}
}


void Process_Request_Class()
{
	switch( port_type[ device_port ] ) {
		case PAD_TYPE_GUNCON: Guncon2_Request_Class(); break;

		default:
			USB_LOG( "\t\t\t*** Unknown device request class\n" );
			break;
	}
}


void Process_Request_Standard()
{
	if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_DEVICE ) {
		switch( DEVICE.req ) {
			case DEVICE_GET_STATUS:
				USB_LOG( "\t\tGet status\n" );


				// return 2 bytes
				DEVICE.data_buf[0] = 0;
				DEVICE.data_buf[1] = 0;
				break;


			case DEVICE_CLEAR_FEATURE:
				USB_LOG( "\t\t***  Clear feature\n" );
				break;


			case DEVICE_SET_FEATURE:
				USB_LOG( "\t\t***  Set feature\n" );
				break;


			case DEVICE_SET_ADDRESS:
				USB_LOG( "\t\tSet address = %d\n", DEVICE.req_arg1 );


				// assign unique id
				DEVICE.id = DEVICE.req_arg1;
				break;


			case DEVICE_GET_DESCRIPTOR:
				USB_LOG( "\t\tGet descriptor = I%d : D%d\n", DEVICE.req_arg1 & 0xff, DEVICE.req_arg1>>8 );


				memcpy( DEVICE.data_buf, Get_Device_Descriptor(), DEVICE.data_len );
				break;


			case DEVICE_SET_DESCRIPTOR:
				USB_LOG( "\t\t***  Set descriptor = I%d : D%d\n", DEVICE.req_arg1 & 0xff, DEVICE.req_arg1>>8 );
				break;


			case DEVICE_GET_CONFIGURATION:
				USB_LOG( "\t\t***  Get configuration\n" );


				// return configuration #
				DEVICE.data_buf[0] = DEVICE.config;
				break;


			case DEVICE_SET_CONFIGURATION:
				USB_LOG( "\t\tSet configuration = %d\n", DEVICE.req_arg1 );


				// device now configured
				DEVICE.config = DEVICE.req_arg1;
				break;


			default:
				USB_LOG( "\t\t***  Unknown request command\n" );
				break;
		}
	}


	else if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_INTERFACE ) {
		switch( DEVICE.req ) {
			case INTERFACE_GET_STATUS:
				USB_LOG( "\t\tGet status\n" );


				// return 2 bytes
				DEVICE.data_buf[0] = 0;
				DEVICE.data_buf[1] = 0;
				break;


			case INTERFACE_CLEAR_FEATURE:
				USB_LOG( "\t\t***  Clear feature\n" );
				break;


			case INTERFACE_SET_FEATURE:
				USB_LOG( "\t\t***  Set feature\n" );
				break;


			case INTERFACE_GET_INTERFACE:
				USB_LOG( "\t\t***  Get interface\n" );
				

				// return configuration #
				DEVICE.data_buf[0] = DEVICE.interface_num;
				break;


			case INTERFACE_SET_INTERFACE:
				USB_LOG( "\t\tSet interface = %d\n", DEVICE.req_arg1 );


				// device now configured
				DEVICE.interface_num = DEVICE.req_arg1;
				break;


			default:
				USB_LOG( "\t\t***  Unknown request command\n" );
				break;
		}
	}


	else if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_ENDPOINT ) {
		switch( DEVICE.req ) {
			case ENDPOINT_GET_STATUS:
				USB_LOG( "\t\tGet status\n" );


				// return 2 bytes
				DEVICE.data_buf[0] = 0;
				DEVICE.data_buf[1] = 0;
				break;


			case ENDPOINT_CLEAR_FEATURE:
				USB_LOG( "\t\t***  Clear feature\n" );
				break;


			case ENDPOINT_SET_FEATURE:
				USB_LOG( "\t\t***  Set feature\n" );
				break;


			case ENDPOINT_SYNCH_FRAME:
				USB_LOG( "\t\t***  Synch frame\n" );
				break;


			default:
				USB_LOG( "\t\t***  Unknown request command\n" );
				break;
		}
	}
}


/*
port # = 0-1
ed_id = usb address (device id)
ed_target = endpoint # (control vs data)
td_len = transport length
td_dir = transport direction
td_buf = transport buffer
*/

int USB_Transfer( int port, int ed_id, int ed_target, int td_len, int td_dir, u8 *td_buf )
{
	int transfer_len;


	// ports = 0-1 / 0-7 = port1, 8-15 = port2
	device_port = port;
	device_active = device_port * 8;

	transfer_len = 0;


	// check target id - not okay
	if( ed_id != DEVICE.id ) return -1;


	// no device attached - skip (ex. Virtua Cop 1P - 2P)
	if( port_type[ port ] == PAD_TYPE_NONE ) return -1;


	// DATA endpoint
	if( ed_target > 0 ) {
		switch( td_dir ) {
			case OHCI_TD_DIR_OUT:
				USB_LOG( "\t\tDevice data - write\n");
				break;


			case OHCI_TD_DIR_IN:
				USB_LOG( "\t\tDevice data - read\n");


				transfer_len = Read_Device_Data( td_buf );
				break;


			default:
				break;
		}
	}


	// COMMAND endpoint
	else {
		switch( td_dir ) {
			case OHCI_TD_DIR_SETUP:
				// should be a stall, just abort
				if( td_len != 8 ) {
					USB_LOG( "\t\t***  Possible SETUP stall - abort\n" );
					return 1;
				}


				transfer_len = 8;

				DEVICE.req = td_buf[1];
				DEVICE.req_format = td_buf[0];
				DEVICE.req_arg1 = ( td_buf[3] << 8 ) | td_buf[2];
				DEVICE.req_arg2 = ( td_buf[5] << 8 ) | td_buf[4];

				DEVICE.data_len = ( td_buf[7] << 8 ) | td_buf[6];
				DEVICE.data_index = 0;


				if( usb_logging ) {
					USB_LOG( "\t\t[P%d][D%d] Setup = ", port, ed_id );


					if( ( DEVICE.req_format & REQ_TYPE ) == REQ_TYPE_STANDARD )
						USB_LOG( "Standard - " );
					else if( ( DEVICE.req_format & REQ_TYPE ) == REQ_TYPE_CLASS )
						USB_LOG( "Class - " );
					else if( ( DEVICE.req_format & REQ_TYPE ) == REQ_TYPE_VENDOR )
						USB_LOG( "Vendor - " );
					else if( ( DEVICE.req_format & REQ_TYPE ) == REQ_TYPE_RESERVED )
						USB_LOG( "Reserved - " );


					if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_DEVICE )
						USB_LOG( "Device" );
					else if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_INTERFACE )
						USB_LOG( "Interface" );
					else if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_ENDPOINT )
						USB_LOG( "Endpoint" );
					else if( ( DEVICE.req_format & REQ_TARGET ) == REQ_TARGET_OTHER )
						USB_LOG( "Other" );
					else
						USB_LOG( "Reserved" );


					USB_LOG( "\n" );
				}


				switch( DEVICE.req_format & REQ_TYPE ) {
					case REQ_TYPE_STANDARD:
						Process_Request_Standard();
						break;


					case REQ_TYPE_CLASS:
						Process_Request_Class();
						break;


					case REQ_TYPE_VENDOR:
						USB_LOG( "\t\t***  Unused request type\n" );
						break;


					default:
						USB_LOG( "\t\t***  Unknown request type\n" );
						break;
				}
				break;


			case OHCI_TD_DIR_OUT:
				if( (DEVICE.req_format & REQ_DIR) == REQ_DIR_OUT )
				{
					// assume partial transfers possible
					transfer_len = DEVICE.data_len - DEVICE.data_index;
					if( transfer_len > td_len ) transfer_len = td_len;


					// send data to device
					memcpy( DEVICE.data_buf + DEVICE.data_index, td_buf, transfer_len );

					Device_Request_Out();


					// update cursors
					DEVICE.data_index += transfer_len;
					if( DEVICE.data_index >= DEVICE.data_len ) DEVICE.data_index = DEVICE.data_len;


					USB_LOG( "\t\tDevice write = %d [%d bytes]\n", DEVICE.data_index, transfer_len );
				}
				else {
					USB_LOG( "\t\tPossible OUT ACK\n" );
				}
				break;


			case OHCI_TD_DIR_IN:
				if( (DEVICE.req_format & REQ_DIR) == REQ_DIR_IN )
				{
					// assume partial transfers possible
					transfer_len = DEVICE.data_len - DEVICE.data_index;
					if( transfer_len > td_len ) transfer_len = td_len;


					// get data from device
					memcpy( td_buf, DEVICE.data_buf + DEVICE.data_index, transfer_len );


					// update cursors
					DEVICE.data_index += transfer_len;
					if( DEVICE.data_index >= DEVICE.data_len ) DEVICE.data_index = DEVICE.data_len;


					USB_LOG( "\t\tDevice read = %d [%d bytes]\n", DEVICE.data_index, transfer_len );
				}
				else {
					USB_LOG( "\t\tPossible IN ACK\n" );
				}
				break;


			default:
				break;
		}
	}


	USB_LOG( "\t\tBytes transferred = %d\n", transfer_len );


	// transfer okay
	return transfer_len;
}
