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


#include <stdlib.h>
#include <string.h>

#include "USB.h"

void __Log(char *fmt, ...) {
	va_list list;


	if( !usb_logging ) return;


	if( fp_debug == 0 ) fp_debug = fopen( "logs\\nuvee-log.txt", "w" );
	if( fp_debug == 0 ) fp_debug = fopen( "nuvee-log.txt", "w" );


	va_start( list, fmt );
	vfprintf( fp_debug, fmt, list );
	va_end( list);
}



#include "USB_device.cpp"


int freeze_load;


void USB_ResetRoothub( int flag )
{
	// get ready for freeze defrost
	if( flag == 1 ) {
		// NOTE: PCSX2 uses a freeze load when plugin config changes
		// - can't do this here (won't save changes after freeze load)

		freeze_load = 1;


		USB_LOG( "[Freeze] Defrost detected\n" );
	}

	else if( flag == 0 && freeze_load == 1 && ohci ) {
		// device not connected
		ohci->roothub.portstatus[0] &= ~OHCI_PORT_CCS;
		ohci->roothub.portstatus[1] &= ~OHCI_PORT_CCS;

		ohci->roothub.portstatus[0] &= ~OHCI_PORT_LSDA;
		ohci->roothub.portstatus[1] &= ~OHCI_PORT_LSDA;


		ohci->intrstatus |= OHCI_INTR_RHSC;


		// disconnect devices
		ohci->reset_roothub_timer = 40;


		// reset device info (for future reconnection)
		Reset_Devices();


		USB_LOG( "[Freeze] Roothub reset - devices changed\n" );
	}
	else
		freeze_load = 0;
}


void USBReset()
{
	memset( usbregs, 0, sizeof(usbregs) );

	ohci = (struct ohci_regs*) usbregs;


	// 1.0 USB + 2 ports
	ohci->revision = 0x10;
	ohci->roothub.a = 0x2;

	ohci->intrstatus |= OHCI_INTR_RHSC;

	// ===================================
	// ===================================
	// ===================================

	Reset_Devices();


	
	USB_LOG( "Full USB reset done\n" );
}


u8 CALLBACK USBread8( u32 addr )
{
	u8 hard;


	switch (addr) {
		default:
			hard = usbRu8(addr); 

			USB_LOG( "[R08] %X = %X\n", addr, hard );
			break;
	}


	return hard;
}


u16 CALLBACK USBread16( u32 addr )
{
	u16 hard;


	switch (addr) {
		default:
			hard = usbRu16(addr); 

			USB_LOG( "[R16] %X = %X\n", addr, hard );
			return hard;
	}

	return hard;
}


u32 CALLBACK USBread32( u32 addr )
{
	u32 hard;
	int i; 


	if( addr >= 0x1f801654 && addr < 0x1f801690 )
	{
		i = ( addr - 0x1f801654 ) / 4;
		hard = ohci->roothub.portstatus[i];

		USB_LOG( "[R32] %X = %08X  [roothub.portstatus[%d]\n", addr, hard, i );
		return hard;
	}


	switch (addr) {
		case 0x1f801600:
			hard = ohci->revision;

			USB_LOG( "[R32] %X = %08X  [revision]\n", addr, hard );
			return hard;


		case 0x1f801604:
			hard = ohci->control;

			USB_LOG( "[R32] %X = %08X  [control]\n", addr, hard );
			return hard;


		case 0x1f801608:
			//hard = ohci->status;

			// command empty (for now)
			hard = 0;

			USB_LOG( "[R32] %X = %08X  [status]\n", addr, hard );
			return hard;


		case 0x1f80160c:
			hard = ohci->intrstatus;

			USB_LOG( "[R32] %X = %08X  [intrstatus]\n", addr, hard );
			return hard;


		case 0x1f801610:
			hard = ohci->intrenable;

			USB_LOG( "[R32] %X = %08X  [intrenable]\n", addr, hard );
			return hard;


		case 0x1f801614:
			hard = ohci->intrenable;

			USB_LOG( "***  [R32] %X = %08X  [intrdisble]\n", addr, hard );
			return hard;


		case 0x1f801618:
			hard = ohci->hcca;


			USB_LOG( "[R32] %X = %08X  [hcca]\n", addr, hard );
			return hard;


		case 0x1f80161c:
			hard = ohci->periodcurrent;


			USB_LOG( "[R32] %X = %08X  [periodcurrent]\n", addr, hard );
			return hard;


		case 0x1f801620:
			hard = ohci->controlhead;


			USB_LOG( "[R32] %X = %08X  [controlhead]\n", addr, hard );
			return hard;


		case 0x1f801624:
			hard = ohci->controlcurrent;


			USB_LOG( "[R32] %X = %08X  [controlcurrent]\n", addr, hard );
			return hard;


		case 0x1f801628:
			hard = ohci->bulkhead;


			USB_LOG( "[R32] %X = %08X  [bulkhead]\n", addr, hard );
			return hard;


		case 0x1f80162c:
			hard = ohci->bulkcurrent;


			USB_LOG( "[R32] %X = %08X  [bulkcurrent]\n", addr, hard );
			return hard;


		case 0x1f801630:
			hard = ohci->donehead;


			USB_LOG( "[R32] %X = %08X  [donehead]\n", addr, hard );
			return hard;


		case 0x1f801648:
			hard = ohci->roothub.a;

			USB_LOG( "[R32] %X = %08X  [roothub.a]\n", addr, hard );
			return hard;


		case 0x1f80164C:
			// 16-bits = device attached to port #1/2
			// 16-bits of power mask on port #1/2
			hard = ohci->roothub.b;

			USB_LOG( "[R32] %X = %08X  [roothub.b]\n", addr, hard );
			return hard;


		case 0x1f801650:
			hard = ohci->roothub.status;

			USB_LOG( "[R32] %X = %08X  [roothub.status]\n", addr, hard );
			return hard;


		default:
			hard = usbRu32(addr); 

			USB_LOG( "*** [R32] %X = %08X\n", addr, hard );
			return hard;
	}


	return hard;
}


void CALLBACK USBwrite8( u32 addr, u8 value )
{
	switch (addr) {
		default:
			usbRu8(addr) = value;

			USB_LOG( "[W08] %X = %X\n", addr, value );
			return;
	}

	usbRu8(addr) = value;
}


void CALLBACK USBwrite16( u32 addr, u16 value )
{
	switch (addr) {
		default:
			usbRu16(addr) = value;

			USB_LOG( "[W16] %X = %X\n", addr, value );
			return;
	}

	usbRu16(addr) = value;
}


void CALLBACK USBwrite32( u32 addr, u32 value )
{
	int i;


	if( addr >= 0x1f801654 && addr < 0x1f801690 )
	{
		i = (addr - 0x1f801654) / 4;

		
		// Port = 1-NDP (2)
		USB_LOG( "[W32] %X = %08X  [roothub.portstatus[%d]\n", addr, value, i );
		if( i >= 2 ) return;


		// mask clearing - bits 16-20 (change in status)
		ohci->roothub.portstatus[i] &= ~( value & 0xff0000 );


		// port reset
		if( value & OHCI_PORT_PRS ) {
			// ignore illegal disconnected state


			USB_LOG( "Port %d = Reset device\n", i );


			// Gunfighter 2 - take too long (30+) and device time-out
			ohci->reset_port_timer[i] = 10;
		}



		// port power status - on / off
		if( value & OHCI_PORT_PPS ) {
			ohci->roothub.portstatus[i] |= OHCI_PORT_PPS;


			USB_LOG( "Port %d = Power on\n", i );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}

		if( value & OHCI_PORT_LSDA ) {
			ohci->roothub.portstatus[i] &= ~OHCI_PORT_PPS;


			USB_LOG( "Port %d = Power on\n", i );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}


		// port enable - on / off
		if( value & OHCI_PORT_PES ) {
			ohci->roothub.portstatus[i] |= OHCI_PORT_PES;
			ohci->roothub.portstatus[i] |= OHCI_PORT_PESC;


			USB_LOG( "Port %d = Port enabled\n", i );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}

		if( value & OHCI_PORT_CCS ) {
			ohci->roothub.portstatus[i] &= ~OHCI_PORT_PES;
			ohci->roothub.portstatus[i] |= OHCI_PORT_PESC;


			USB_LOG( "Port %d = Port disabled\n", i );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}


		// ignore port suspend/resume
		return;
	}

	switch (addr) {
		case 0x1f801600: // ohci->revision (read only)
			USB_LOG( "[W32] %X = %08X  [revision]\n", addr, value );
			return;
		

		case 0x1f801604:
			{
				u32 old_state, new_state;

				USB_LOG( "[W32] %X = %08X  [control]\n", addr, value );

				
				old_state = ohci->control & OHCI_CTRL_HCFS;
				ohci->control = value;
				new_state = ohci->control & OHCI_CTRL_HCFS;

				// no change
				if( old_state == new_state ) return;



				switch( new_state ) {
				case OHCI_USB_OPERATIONAL:
					USBirq( PSXCLK / 1000 );

					ohci->reset_roothub_timer = 40;
					return;

				case OHCI_USB_SUSPEND:
					USB_LOG( "***  USB Suspend\n" );
					return;

				case OHCI_USB_RESUME:
					USB_LOG( "***  USB Resume\n" );
					return;

				case OHCI_USB_RESET:
					USB_LOG( "USB Reset\n" );
					
					USBReset();
					return;
				}
			}
			return;


		case 0x1f801608:
			USB_LOG( "[W32] %X = %08X  [status]\n", addr, value );

			// SOC is read-only
			value = ( value & ~OHCI_STATUS_SOC );

			// '1' mask
			ohci->status |= value;



			if( ohci->status & OHCI_STATUS_HCR ) {
				USBReset();

				// clear reset bit
				ohci->status &= ~OHCI_STATUS_HCR;


				USB_LOG( "OHCI status - reset\n" );
			}
			return;


		case 0x1f80160c:
			USB_LOG( "[W32] %X = %08X  [intrstatus]\n", addr, value );

			// '1' mask clear
			ohci->intrstatus &= ~value;
			return;


		case 0x1f801610:
			USB_LOG( "[W32] %X = %08X  [intrenable]\n", addr, value );

			ohci->intrenable |= value;
			return;


		case 0x1f801614:
			USB_LOG( "[W32] %X = %08X  [intrdisable]\n", addr, value );

			ohci->intrenable &= ~value;
			return;


		case 0x1f801618:
			USB_LOG( "[W32] %X = %08X  [hcca]\n", addr, value );

			if( value == 0xffffffff ) {
				USB_LOG( "***  Invalid hcca\n" );
				return;
			}

			ohci->hcca = value;
			return;


		case 0x1f80161c:
			USB_LOG( "***  [W32] %X = %08X  [periodcurrent]\n", addr, value );

			ohci->periodcurrent = value;
			return;


		case 0x1f801620:
			USB_LOG( "[W32] %X = %08X  [controlhead]\n", addr, value );

			ohci->controlhead = value;
			return;


		case 0x1f801624:
			USB_LOG( "[W32] %X = %08X  [controlcurrent]\n", addr, value );

			ohci->controlcurrent = value;
			return;


		case 0x1f801628:
			USB_LOG( "[W32] %X = %08X  [bulkhead]\n", addr, value );

			ohci->bulkhead = value;
			return;


		case 0x1f80162c:
			USB_LOG( "[W32] %X = %08X  [bulkcurrent]\n", addr, value );

			ohci->bulkcurrent = value;
			return;


		case 0x1f801630:
			USB_LOG( "[W32] %X = %08X  [donehead]\n", addr, value );

			ohci->donehead = value;
			return;


		case 0x1f801634:
			USB_LOG( "[W32] %X = %08X  [fminterval]\n", addr, value );

			ohci->fminterval = value;
			return;


		case 0x1f801640:
			USB_LOG( "[W32] %X = %08X  [periodicstart]\n", addr, value );

			ohci->periodicstart = value;
			return;


		case 0x1f801644:
			USB_LOG( "[W32] %X = %08X  [lsthresh]\n", addr, value );

			ohci->lsthresh = value;
			return;


		case 0x1f801648:
			USB_LOG( "[W32] %X = %08X  [roothub.a]\n", addr, value );

			ohci->roothub.a = value;
			return;


		case 0x1f80164C:
			USB_LOG( "[W32] %X = %08X  [roothub.b]\n", addr, value );

			ohci->roothub.b = value;
			return;


		case 0x1f801650:
			USB_LOG( "[W32] %X = %08X  [roothub.status]\n", addr, value );

			ohci->roothub.status = value;
			return;


		default:
			USB_LOG( "(***)  [W32] %X = %08X\n", addr, value );

			usbRu32(addr) = value;
			return;
	}

	usbRu32(addr) = value;
}


void CALLBACK USBirqCallback(USBcallback callback)
{
	USBirq = callback;
}


int Process_Endpoint( u32 head )
{
	struct ohci_ed *ed;
	struct ohci_td *td;
	u32 cur_ed;
	int active;

	active = 0;


	// Endpoint Descriptor - list processing
	cur_ed = head;
	while( cur_ed != 0 ) {
		ed = (struct ohci_ed *) (ram + cur_ed);


		//USB_LOG( "[%X] ED: %X %X %X\n", cur_ed, ed->head, ed->tail, ed->next );


		// check skip, halt flags
		if( !(ed->head & OHCI_ED_H) &&
				!(ed->flags & OHCI_ED_K) )
		{
			int ed_id, ed_target;


			// target device id, endpoint #
			ed_id = ( ed->flags & OHCI_ED_FA_MASK ) >> OHCI_ED_FA_SHIFT;
			ed_target = ( ed->flags & OHCI_ED_EN_MASK ) >> OHCI_ED_EN_SHIFT;


			// process Transport Descriptor
			while( (ed->head & OHCI_DPTR_MASK) != ed->tail ) {
				int lcv;
				int td_dir, td_len;
				u8 *td_buf;
				int bytes_out;


				// control, bulk flag - all TD drained from list
				active = 1;
				td = (struct ohci_td *) ( ram + (ed->head & OHCI_DPTR_MASK) );
				td_buf = (u8 *) ( ram + td->cbp );

				// ========================================
				// ========================================
				// ========================================

				// get packet direction
				td_dir = ed->flags & OHCI_ED_D_MASK;

				if( td_dir == OHCI_ED_DIR_OUT ) td_dir = OHCI_TD_DIR_OUT;
				else if( td_dir == OHCI_ED_DIR_IN ) td_dir = OHCI_TD_DIR_IN;
				else td_dir = td->flags & OHCI_TD_DP_MASK;


				// get packet length - in / out
				// - cbp = current data addr, be = end data addr
				// - NOTE: assume same page boundary, ~0xfff address range (see QEmu)
				td_len = 0;
				if( td->cbp != 0 && td->be != 0 ) td_len = ( td->be - td->cbp ) + 1;

				// ========================================
				// ========================================
				// ========================================

				if( usb_logging )
				{
					// debugging
					if( td_dir == OHCI_TD_DIR_IN )
						USB_LOG( "\t[%X] TD [IN]: %X %X %X  (%d bytes, %X -> %X)\n",
							cur_ed, ed->head, ed->next, ed->tail,
							td_len, td->cbp, td->be );
					else if( td_dir == OHCI_TD_DIR_OUT )
						USB_LOG( "\t[%X] TD [OUT]: %X %X %X  (%d bytes, %X -> %X)\n",
							cur_ed, ed->head, ed->next, ed->tail,
							td_len, td->cbp, td->be );
					else if( td_dir == OHCI_TD_DIR_SETUP )
						USB_LOG( "\t[%X] TD [SETUP]: %X %X %X  (%d bytes, %X -> %X)\n",
							cur_ed, ed->head, ed->next, ed->tail,
							td_len, td->cbp, td->be );
					else
						USB_LOG( "\t***  [%X] TD [???]: %X %X %X  (%d bytes, %X -> %X)\n",
							cur_ed, ed->head, ed->next, ed->tail,
							td_len, td->cbp, td->be );



					// debugging - ps2 -> device
					if( td_dir != OHCI_TD_DIR_IN ) {
						USB_LOG( "\t[D%d][E%d] To device: ", ed_id, ed_target );

						for( lcv = 0; lcv < td_len; lcv++ ) {
							USB_LOG( "%02X ", td_buf[lcv] );
						}

						USB_LOG( "\n" );
					}
				}


				bytes_out = -1;

				for( lcv = 0; lcv < 2; lcv++ ) {
					// port disabled
					if( (ohci->roothub.portstatus[lcv] & OHCI_PORT_PES) == 0 ) continue;


					// transfer data through port
					bytes_out = USB_Transfer( lcv, ed_id, ed_target, td_len, td_dir, td_buf );
					if( bytes_out >= 0 )
						break;
				}



				if( usb_logging )
				{
					// debugging - device -> ps2
					if( td_dir == OHCI_TD_DIR_IN && bytes_out >= 0 ) {
						USB_LOG( "\t[D%d][E%d] From device: ", ed_id, ed_target );

						for( lcv = 0; lcv < bytes_out; lcv++ ) {
							USB_LOG( "%02X ", td_buf[lcv] );
						}

						USB_LOG( "\n" );
					}
				}

				// ========================================
				// ========================================
				// ========================================

				// safety check - no ID match
				if( bytes_out < 0 ) bytes_out = 0;


				// update data ptrs
				td->cbp += bytes_out;
				if( td->cbp >= td->be ) td->cbp = 0;


				// transfer okay - total bytes okay -or- underrun allowed
				if( bytes_out == td_len ||
						(td_dir == OHCI_TD_DIR_IN && bytes_out >= 0 && (td->flags & OHCI_TD_R)) )
				{
					td->flags |= OHCI_TD_T1;
					td->flags ^= OHCI_TD_T0;

					// clear error codes
					td->flags &= ~OHCI_TD_CC_MASK;
					td->flags &= ~OHCI_TD_EC_MASK;

					ed->head &= ~OHCI_ED_C;
					if( td->flags & OHCI_TD_T0 )
						ed->head |= OHCI_ED_C;
				}

				// assume underrun (6 of 8 bytes received)
				else {
					// error code
					td->flags &= ~OHCI_TD_CC_MASK;
					td->flags |= OHCI_CC_DATAUNDERRUN;


					// halt - error
					ed->head |= OHCI_ED_H;


					USB_LOG( "\t***  Data underrun\n" );
				}

				// ========================================
				// ========================================
				// ========================================

				// clear out TD from list
				lcv = ed->head;

				ed->head = td->next & OHCI_DPTR_MASK;
				td->next = ohci->donehead;
				ohci->donehead = lcv;
 

				// set delay interrupt (writeback for donehead)
				ohci->ohci_donehead_timer = (td->flags & OHCI_TD_DI_MASK) >> OHCI_TD_DI_SHIFT;
			}
		}


		cur_ed = ed->next & OHCI_DPTR_MASK;
	}


	return active;
}



int CALLBACK _USBirqHandler(void)
{
	int irq_assert, lcv;


	// load state - restart interrupts
	if( freeze_load == 1 ) {
		USB_LOG( "_USBirqHandler: Freeze - restart IRQ\n" );


		USBirq(PSXCLK / 1000);


		freeze_load = 0;
		return 0;
	}


#ifdef USB_LOG
	//USB_LOG("_USBirqHandler: %x\n", ohci->intrenable);
#endif

	irq_assert = 0;


	if( ram )
		hcca = (struct ohci_hcca*) &ram[ ohci->hcca ];


	// generate SF packet every 1ms ticks
	if( (ohci->control & OHCI_CTRL_HCFS) == OHCI_USB_OPERATIONAL ) {
		USBirq(PSXCLK / 1000);

		ohci->intrstatus |= OHCI_INTR_SF;
	}

	// =========================================
	// =========================================
	// =========================================

	// process endpoint lists
	if( ohci->control & OHCI_CTRL_PLE ) {
		USB_LOG( "PLE: %X  [%d]\n", hcca->int_table[ ohci->fmnumber & 0x1f ], ohci->fmnumber & 0x1f );

		Process_Endpoint( hcca->int_table[ ohci->fmnumber & 0x1f ] );
	}


	if( (ohci->control & OHCI_CTRL_CLE) && (ohci->status & OHCI_STATUS_CLF) )
	{
		USB_LOG( "CLE: %X\n", ohci->controlhead );

		if( Process_Endpoint( ohci->controlhead ) ) {
			// list now empty
			ohci->controlcurrent = 0;
			ohci->status &= ~OHCI_STATUS_CLF;
		}
	}

	
	if( (ohci->control & OHCI_CTRL_BLE) && (ohci->status & OHCI_STATUS_BLF) )
	{
		USB_LOG( "***  BLE: %X\n", ohci->bulkhead );

		if( Process_Endpoint( ohci->bulkhead ) ) {
			// list now empty
			ohci->bulkcurrent = 0;
			ohci->status &= ~OHCI_STATUS_BLF;
		}
	}

	// =========================================
	// =========================================
	// =========================================

	if (ohci->fmnumber == 0xffff) {
		ohci->fmnumber = 0;
		ohci->intrstatus |= OHCI_INTR_FNO;
	} else {
		ohci->fmnumber++;
	}


	// writeback frame #
	if( hcca ) hcca->frame_no = (u16) ohci->fmnumber;

	// =========================================
	// =========================================
	// =========================================

	// writeback interrupt
	if( !(ohci->intrstatus & OHCI_INTR_WDH) && ohci->ohci_donehead_timer == 0 )
	{
		USB_LOG( "_USBirqHandler: Writeback TD\n" );


		if( ohci->intrenable & OHCI_INTR_MIE ) {
			irq_assert = 1;

			ohci->donehead |= 1;
		}


    ohci->intrstatus |= OHCI_INTR_WDH;


		// writeback
		if( hcca ) hcca->done_head = ohci->donehead;


		ohci->donehead = 0;
		ohci->ohci_donehead_timer = 7;
	}


	if( ohci->intrenable & OHCI_INTR_MIE &&
			ohci->intrenable & OHCI_INTR_RHSC &&
			ohci->intrstatus & OHCI_INTR_RHSC )
	{
		USB_LOG( "_USBirqHandler: Root hub status change\n" );

		irq_assert = 1;
	}


	if( ohci->intrenable & OHCI_INTR_MIE &&
			ohci->intrenable & OHCI_INTR_FNO &&
			ohci->intrstatus & OHCI_INTR_FNO )
	{
		USB_LOG( "_USBirqHandler: Frame number overflow\n" );
		
		irq_assert = 1;
	}


	if( ohci->intrenable & OHCI_INTR_MIE &&
			ohci->intrenable & OHCI_INTR_SF &&
			ohci->intrstatus & OHCI_INTR_SF )
	{
		USB_LOG( "_USBirqHandler: Start of frame packet\n" );

		irq_assert = 1;
	}

	// ====================================
	// ====================================
	// ====================================

	if( ohci->ohci_donehead_timer > 0 && ohci->ohci_donehead_timer != 7 )
		ohci->ohci_donehead_timer--;


	for( lcv = 0; lcv < 2; lcv++ ) {
		if( ohci->reset_port_timer[lcv] )
		{
			ohci->reset_port_timer[lcv]--;
			if( ohci->reset_port_timer[lcv] == 0 )
			{
				ohci->roothub.portstatus[lcv] &= ~OHCI_PORT_PRS;
				ohci->roothub.portstatus[lcv] |= OHCI_PORT_PRSC;


				USB_LOG( "Port %d = Reset done\n", lcv );
				ohci->intrstatus |= OHCI_INTR_RHSC;
			}
		}
	}


	if( ohci->reset_roothub_timer ) {
		ohci->reset_roothub_timer--;

		// Let game see port0 first (!)
		if( ohci->reset_roothub_timer == 20 )
		{
			Attach_Devices();


			// connect status changed
			ohci->roothub.portstatus[0] &= ~OHCI_PORT_CCS;
			ohci->roothub.portstatus[0] |= OHCI_PORT_CSC;

			if( usb_dev->list[0].connected ) ohci->roothub.portstatus[0] |= OHCI_PORT_CCS;


			// check speed
			ohci->roothub.portstatus[0] &= ~OHCI_PORT_LSDA;

			if( usb_dev->list[0].lowspeed ) ohci->roothub.portstatus[0] |= OHCI_PORT_LSDA;


			// port now enabled
			ohci->roothub.portstatus[0] |= OHCI_PORT_PES;
			ohci->roothub.portstatus[0] |= OHCI_PORT_PESC;


			// port powered
			ohci->roothub.portstatus[0] |= OHCI_PORT_PPS;


			// port fully reset
			ohci->roothub.portstatus[0] &= ~OHCI_PORT_PRS;
			ohci->roothub.portstatus[0] |= OHCI_PORT_PRSC;


			USB_LOG( "Device #0 now connected\n" );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}


		// After time passes, then connect port1 (!)
		else if( ohci->reset_roothub_timer == 0 )
		{
			// connect status changed
			ohci->roothub.portstatus[1] &= ~OHCI_PORT_CCS;
			ohci->roothub.portstatus[1] |= OHCI_PORT_CSC;

			if( usb_dev->list[8].connected ) ohci->roothub.portstatus[1] |= OHCI_PORT_CCS;


			// check speed
			ohci->roothub.portstatus[1] &= ~OHCI_PORT_LSDA;

			if( usb_dev->list[8].lowspeed ) ohci->roothub.portstatus[1] |= OHCI_PORT_LSDA;


			// port now enabled
			ohci->roothub.portstatus[1] |= OHCI_PORT_PES;
			ohci->roothub.portstatus[1] |= OHCI_PORT_PESC;


			// port powered
			ohci->roothub.portstatus[1] |= OHCI_PORT_PPS;


			// port fully reset
			ohci->roothub.portstatus[1] &= ~OHCI_PORT_PRS;
			ohci->roothub.portstatus[1] |= OHCI_PORT_PRSC;


			USB_LOG( "Device #1 now connected\n" );
			ohci->intrstatus |= OHCI_INTR_RHSC;
		}
	}

// ===========================================
// ===========================================
// ===========================================

	// update raw input
	Poll_Devices( ohci->fmnumber );


	return irq_assert;
}
