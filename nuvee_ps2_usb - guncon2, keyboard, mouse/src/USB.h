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


#ifndef __USB_H__
#define __USB_H__



#define usbRs8(mem)		usbregs[(mem) & 0xffff]
#define usbRs16(mem)	(*(s16*)&usbregs[(mem) & 0xffff])
#define usbRs32(mem)	(*(s32*)&usbregs[(mem) & 0xffff])
#define usbRu8(mem)		(*(u8*) &usbregs[(mem) & 0xffff])
#define usbRu16(mem)	(*(u16*)&usbregs[(mem) & 0xffff])
#define usbRu32(mem)	(*(u32*)&usbregs[(mem) & 0xffff])



#define USB_LOG __Log


/*
 * URB OHCI HCD (Host Controller Driver) for USB.
 * 
 * (C) Copyright 1999 Roman Weissgaerber <weissg@vienna.at>
 * (C) Copyright 2000-2001 David Brownell <dbrownell@users.sourceforge.net>
 * 
 * usb-ohci.h
 */

/*
 * The HCCA (Host Controller Communications Area) is a 256 byte
 * structure defined in the OHCI spec. that the host controller is
 * told the base address of.  It must be 256-byte aligned.
 */
 

#define NUM_INTS 32	/* part of the OHCI standard */
struct ohci_hcca {
	u32	int_table[NUM_INTS];	/* Interrupt ED table */
	u16	frame_no;			/* current frame number */
	u16	pad1;					/* set to 0 on each frame_no change */
	u32	done_head;		/* info returned for an interrupt */
	u8	reserved_for_hc[116];
};

/*
 * Maximum number of root hub ports.  
 */
#define MAX_ROOT_PORTS	15	/* maximum OHCI root hub ports */

/*
 * This is the structure of the OHCI controller's memory mapped I/O
 * region.  This is Memory Mapped I/O.  You must use the readl() and
 * writel() macros defined in asm/io.h to access these!!
 */


struct ohci_regs {
	/* control and status registers */
	u32	revision;
	u32	control;
	u32	status;
	u32	intrstatus;
	u32	intrenable;
	u32	intrdisable;

	/* memory pointers */
	u32	hcca;
	u32	periodcurrent;
	u32	controlhead;
	u32	controlcurrent;
	u32	bulkhead;
	u32	bulkcurrent;
	u32	donehead;

	/* frame counters */
	u32	fminterval;
	u32	fmremaining;
	u32	fmnumber;
	u32	periodicstart;
	u32	lsthresh;

	/* Root hub ports */
	struct ohci_roothub_regs {
		u32	a;
		u32	b;
		u32	status;
		u32	portstatus[MAX_ROOT_PORTS];
	} roothub;



	// non-registers
	u8 ohci_donehead_timer;

	int reset_roothub_timer;
	int reset_port_timer[2];
};


// endpoint descriptor
struct ohci_ed {
	u32 flags;
	u32 tail;
	u32 head;
	u32 next;
};

// General transfer descriptor
struct ohci_td {
	u32 flags;
	u32 cbp;
	u32 next;
	u32 be;
};



/* OHCI CONTROL AND STATUS REGISTER MASKS */

/*
 * HcControl (control) register masks
 */
#define OHCI_CTRL_CBSR	(3 << 0)		/* control/bulk service ratio */
#define OHCI_CTRL_PLE		(1 << 2)		/* periodic list enable */
#define OHCI_CTRL_IE		(1 << 3)		/* isochronous enable */
#define OHCI_CTRL_CLE		(1 << 4)		/* control list enable */
#define OHCI_CTRL_BLE		(1 << 5)		/* bulk list enable */
#define OHCI_CTRL_HCFS	(3 << 6)		/* host controller functional state */
#define OHCI_CTRL_IR		(1 << 8)		/* interrupt routing */
#define OHCI_CTRL_RWC		(1 << 9)		/* remote wakeup connected */
#define OHCI_CTRL_RWE		(1 << 10)		/* remote wakeup enable */

/* pre-shifted values for HCFS */
#define OHCI_USB_RESET					(0 << 6)
#define OHCI_USB_RESUME					(1 << 6)
#define OHCI_USB_OPERATIONAL		(2 << 6)
#define OHCI_USB_SUSPEND				(3 << 6)


#define OHCI_HCCA_SIZE				0x100
#define OHCI_HCCA_MASK				0xffffff00


/*
 * HcCommandStatus (cmdstatus) register masks
 */
#define OHCI_STATUS_HCR		(1 << 0)	/* host controller reset */
#define OHCI_STATUS_CLF  	(1 << 1)	/* control list filled */
#define OHCI_STATUS_BLF  	(1 << 2)	/* bulk list filled */
#define OHCI_STATUS_OCR  	(1 << 3)	/* ownership change request */
#define OHCI_STATUS_SOC  	(3 << 16)	/* scheduling overrun count */


// Roothub - port status
#define OHCI_PORT_CCS         (1<<0)		// current connect status (0: detached, 1: attached)
#define OHCI_PORT_PES         (1<<1)		// port enable status (0: off, 1: on)
#define OHCI_PORT_PSS         (1<<2)		// port suspend status (0: not suspend, 1: suspend)
#define OHCI_PORT_POCI        (1<<3)		// port over-current indicator (0: no, 1: yes)
#define OHCI_PORT_PRS         (1<<4)		// port reset status (0: reset off, 1: reset active)
#define OHCI_PORT_PPS         (1<<8)		// port power status (0: no power, 1: power)
#define OHCI_PORT_LSDA        (1<<9)		// low speed device attached (0: full, 1: low speed)

#define OHCI_PORT_CSC         (1<<16)		// connect status change (0: no change, 1: change)
#define OHCI_PORT_PESC        (1<<17)		// port enable status change (0: no, 1: yes)
#define OHCI_PORT_PSSC        (1<<18)		// port suspend status change (0: no, 1: yes)
#define OHCI_PORT_OCIC        (1<<19)		// port over-current indictator change (0: no, 1: yes)
#define OHCI_PORT_PRSC        (1<<20)		// port reset status change (0: in-progress, 1: done)
#define OHCI_PORT_WTC         (OHCI_PORT_CSC|OHCI_PORT_PESC|OHCI_PORT_PSSC \
                               |OHCI_PORT_OCIC|OHCI_PORT_PRSC)

/*
 * masks used with interrupt registers:
 * HcInterruptStatus (intrstatus)
 * HcInterruptEnable (intrenable)
 * HcInterruptDisable (intrdisable)
 */
#define OHCI_INTR_SO		(1 << 0)	/* scheduling overrun */
#define OHCI_INTR_WDH		(1 << 1)	/* writeback of done_head */
#define OHCI_INTR_SF		(1 << 2)	/* start frame */
#define OHCI_INTR_RD		(1 << 3)	/* resume detect */
#define OHCI_INTR_UE		(1 << 4)	/* unrecoverable error */
#define OHCI_INTR_FNO		(1 << 5)	/* frame number overflow */
#define OHCI_INTR_RHSC	(1 << 6)	/* root hub status change */
#define OHCI_INTR_OC		(1 << 30)	/* ownership change */
#define OHCI_INTR_MIE		(1 << 31)	/* master interrupt enable */



/* Bitfields for the first word of an Endpoint Desciptor.  */
/*
FA = Function Address
EN = Endpoint Number
D = Direction (00/11 = Check TD, 01 = OUT, 10 = IN)
S = Speed (0 = full, 1 = low-speed)
K = Skip (skip to next ED, no processing)
F = Format (0 = normal, 1 = isochronous)
MPS = Maximum Packet Size

TailP = Queue Tail Pointer (TD)

HeadP = Points to next TD to process with EP
- H/C

NextED = next ED in list (if non-zero)
*/

#define OHCI_ED_FA_SHIFT  0
#define OHCI_ED_FA_MASK   (0x7f<<OHCI_ED_FA_SHIFT)
#define OHCI_ED_EN_SHIFT  7
#define OHCI_ED_EN_MASK   (0xf<<OHCI_ED_EN_SHIFT)
#define OHCI_ED_D_SHIFT   11
#define OHCI_ED_D_MASK    (3<<OHCI_ED_D_SHIFT)
#define OHCI_ED_S         (1<<13)
#define OHCI_ED_K         (1<<14)
#define OHCI_ED_F         (1<<15)
#define OHCI_ED_MPS_SHIFT 7
#define OHCI_ED_MPS_MASK  (0xf<<OHCI_ED_FA_SHIFT)



#define OHCI_ED_DIR_TD1			(0 << OHCI_ED_D_SHIFT)	// check TD packet
#define OHCI_ED_DIR_OUT			(1 << OHCI_ED_D_SHIFT)	// force OUT
#define OHCI_ED_DIR_IN			(2 << OHCI_ED_D_SHIFT)	// force IN
#define OHCI_ED_DIR_TD2			(3 << OHCI_ED_D_SHIFT)	// check TD packet


/* Flags in the head field of an Endpoint Desciptor.  */
/*
HeadP flags:
- H = Halted (stop, error)
- C = Carry (retired TD)
*/
#define OHCI_ED_H         1
#define OHCI_ED_C         2



/* Bitfields for the first word of a Transfer Desciptor.  */
/*
R = Rounding (0 = exact fill)
DP = Direction/PID (00 = SETUP (to ep), 01 = OUT (to ep), 02 = IN (from ep))
DI = Delay Interrupt (000-110 = # frames before IRQ, 111 = no interrupt after TD done)
T = Data Toggle
EC = Error count
CC = Condition code

CBP = Current Buffer Pointer (real-time transfer address)
NextTD = next TD address
BE = Buffer End (address of last byte in transfer buffer for TD)
*/

#define OHCI_TD_R         (1<<18)
#define OHCI_TD_DP_SHIFT  19
#define OHCI_TD_DP_MASK   (3<<OHCI_TD_DP_SHIFT)
#define OHCI_TD_DI_SHIFT  21
#define OHCI_TD_DI_MASK   (7<<OHCI_TD_DI_SHIFT)
#define OHCI_TD_T0        (1<<24)
#define OHCI_TD_T1        (1<<24)
#define OHCI_TD_EC_SHIFT  26
#define OHCI_TD_EC_MASK   (3<<OHCI_TD_EC_SHIFT)
#define OHCI_TD_CC_SHIFT  28
#define OHCI_TD_CC_MASK   (0xf<<OHCI_TD_CC_SHIFT)


#define OHCI_DPTR_MASK    0xfffffff0

#define OHCI_BM(val, field) \
  (((val) & OHCI_##field##_MASK) >> OHCI_##field##_SHIFT)

#define OHCI_SET_BM(val, field, newval) do { \
    val &= ~OHCI_##field##_MASK; \
    val |= ((newval) << OHCI_##field##_SHIFT) & OHCI_##field##_MASK; \
    } while(0)


#define OHCI_TD_DIR_SETUP     (0 << OHCI_TD_DP_SHIFT)
#define OHCI_TD_DIR_OUT       (1 << OHCI_TD_DP_SHIFT)
#define OHCI_TD_DIR_IN        (2 << OHCI_TD_DP_SHIFT)
#define OHCI_TD_DIR_RESERVED  (3 << OHCI_TD_DP_SHIFT)



#define OHCI_CC_NOERROR             0x0
#define OHCI_CC_CRC                 0x1
#define OHCI_CC_BITSTUFFING         0x2
#define OHCI_CC_DATATOGGLEMISMATCH  0x3
#define OHCI_CC_STALL               0x4
#define OHCI_CC_DEVICENOTRESPONDING 0x5
#define OHCI_CC_PIDCHECKFAILURE     0x6
#define OHCI_CC_UNDEXPETEDPID       0x7
#define OHCI_CC_DATAOVERRUN         0x8
#define OHCI_CC_DATAUNDERRUN        0x9
#define OHCI_CC_BUFFEROVERRUN       0xc
#define OHCI_CC_BUFFERUNDERRUN      0xd

/**********************/

#define PSXCLK	36864000	/* 36.864 Mhz */


#endif
