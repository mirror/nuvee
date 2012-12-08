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


struct usb_device_data {
	u8 connected;
	u8 lowspeed;

	u8 id;
	u8 config;
	u8 interface_num;

	u8 req;						// request
	u8 req_format;		// request type
	u16 req_arg1;			// value
	u16 req_arg2;			// index

	u8 data_len;
	u8 data_index;
	u8 data_buf[256];


	// special register storage
	u16 vars[64];
};


struct usb_device_list {
	// 8 devices * 2 ports
	usb_device_data list[8 * 2];
};


// ======================================
// ======================================
// ======================================

// data transfer direction
#define REQ_DIR									(1 << 7)
#define REQ_DIR_IN							(1 << 7)
#define REQ_DIR_OUT							(0 << 7)

// type
#define REQ_TYPE								(3 << 5)
#define REQ_TYPE_STANDARD				(0 << 5)
#define REQ_TYPE_CLASS					(1 << 5)
#define REQ_TYPE_VENDOR					(2 << 5)
#define REQ_TYPE_RESERVED				(3 << 5)

// recipient
#define REQ_TARGET							(5 << 0)
#define REQ_TARGET_DEVICE				(0 << 0)
#define REQ_TARGET_INTERFACE		(1 << 0)
#define REQ_TARGET_ENDPOINT			(2 << 0)
#define REQ_TARGET_OTHER				(3 << 0)


// command
#define DEVICE_GET_STATUS							0
#define DEVICE_CLEAR_FEATURE					1
#define DEVICE_SET_FEATURE						3
#define DEVICE_SET_ADDRESS						5
#define DEVICE_GET_DESCRIPTOR					6
#define DEVICE_SET_DESCRIPTOR					7
#define DEVICE_GET_CONFIGURATION			8
#define DEVICE_SET_CONFIGURATION			9

#define INTERFACE_GET_STATUS					0
#define INTERFACE_CLEAR_FEATURE				1
#define INTERFACE_SET_FEATURE					3
#define INTERFACE_GET_INTERFACE				10
#define INTERFACE_SET_INTERFACE				11

#define ENDPOINT_GET_STATUS						0
#define ENDPOINT_CLEAR_FEATURE				1
#define ENDPOINT_SET_FEATURE					3
#define ENDPOINT_SYNCH_FRAME					18
