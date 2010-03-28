/*
 * magicformel.c - Cartridge handling, Magic Formel cart.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Groepaz <groepaz@gmx.net>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

/*

FIXME: the emulation still does not work 100%. perhaps the schematics have an
       error, or maybe i have just overlooked something.
       
things that work are those from the 64k ROM, the basic toolkit, the freezer,
monitor, fastloader. anything that uses the additional 32k does NOT work yet
(and behaves very strange, press F7, then FIRE(and hold, then move stick))
       
the following is a quick overview of how the cartridge works, as its a bit
unusual and different from most other cartridges:

Magic Formel
- 64K ROM
- 8K RAM

rom bank 0x00 - 0x07 (0x08) 8192* 8 64k
ram bank 0x00 - 0x1f (0x20)  256*32  8k

Magic Formel 2
- 64K ROM
- 32K extra ROM
- 8K RAM

rom bank 0x00 - 0x0f (0x10) 8192*16 128k
ram bank 0x00 - 0x1f (0x20)  256*32   8k

ram is mapped to $deXX (one page in io1 space)
rom is mapped to $e000

*** Register Details

Writing anywhere to the IO2 area adresses a MC6521 (2 8bit i/o ports). However
its registers are not simply memory mapped, instead A0-A5 go to D0-D5, D1 to D7 
and A6-A7 to RS1/RS0.

addr    dfXX    RRDD DDDD

  R -rs1/rs0 of MC6821
  D -d0-d5 of MC6821

data      .X    .... ..D.

  D - d7 of MC6821 (Controls Mapping)

*** MC6821 Registers

RS1  RS0 (CRA2 CRB2 = control register bit 2)
 
0    0    1    .     Data A
0    0    0    .     Data Direction A
0    1    .    .     Control Register A
1    0    .    1     Data B
1    0    .    0     Data Direction B
1    1    .    .     Control Register B

*** MC6821 Port usage

PA (Output Data)

A0,A1,A2       - ROM Bank
A3             - extra ROM enable/disable (?)
A4             - RAM enable/disable (?)
A5-A7 unused

PB (Output Data)

B3,B2,B0,B1,B4 - RAM Bank
B5-B6 (unused)
B7             - enable ROM at $E000 (?)

CB2            - enable Cartridge (?)

*/

/* if defined, permanently use ultimax mode to fake full external decoding. */
/* FIXME: undefining selects 'proper' external decoding, which does NOT work
          right now. someone who knows the memory mapping internals better
          than me should take a look at it perhaps :)
*/
#define USE_ULTIMAX_CONFIG 

/* permanently enable RAM in io1 */
/* #define DEBUG_IO1_NO_DISABLE  */

/* define for debug messages */
/* #define MAGICFORMEL_DEBUG */

/* #define LOG_BANKS */
/* #define LOG_PORTS */


#ifdef MAGICFORMEL_DEBUG
#define DBG(x)  printf x
#else
#define DBG(x)
#endif

#include "vice.h"

#include <stdio.h>
#include <string.h>

#include "c64cart.h"
#include "c64cartmem.h"
#include "c64export.h"
#include "c64io.h"
#include "c64mem.h"
#include "maincpu.h"
#include "machine.h"
#include "magicformel.h"
#include "types.h"


static const c64export_resource_t export_res = {
    "Magic Formel", 1, 1
};

static unsigned int ram_page = 0;
static unsigned int io1_enabled = 0;     /* PA4 */
static unsigned int kernal_enabled = 0;  /* PBZ */
static unsigned int freeze_enabled = 0;
static unsigned int export_game = 1;
static unsigned int hwversion = 0;

/****************************************************************************
* 
****************************************************************************/
#ifdef LOG_BANKS
static int logbanks[0x10];

static void log_bank(int bank)
{
    int i;

    logbanks[bank] = 0xff;
    DBG(("["));

    for (i = 0; i < 0x10; i++) {
        if (logbanks[i] == 0xff) {
            DBG(("*"));
        } else {
            DBG(("."));
        }
    }
    DBG(("]\n"));
}
#endif

/****************************************************************************
* 
****************************************************************************/

/*
    mode & 3 = 0 : GAME=0 EXROM=0^1 roml
    mode & 3 = 1 : GAME=1 EXROM=0^1 roml & romh
    mode & 3 = 2 : GAME=0 EXROM=1^1 ram
    mode & 3 = 3 : GAME=1 EXROM=1^1 ultimax
*/
static void magicformel_config_changed(BYTE mode_phi1, BYTE mode_phi2, unsigned int wflag)
{
    if (wflag == CMODE_WRITE) {
        machine_handle_pending_alarms(maincpu_rmw_flag + 1);
    } else {
        machine_handle_pending_alarms(0);
    }

    export.game = mode_phi2 & 1;
    export.exrom = ((mode_phi2 >> 1) & 1) ^ 1;

    cartridge_romhbank_set((mode_phi2 >> 3)&0x0f);

    mem_pla_config_changed();
    if (mode_phi2 & 0x80) {
        cartridge_release_freeze();
    }
    cart_ultimax_phi1 = (mode_phi1 & 1) & ((mode_phi1 >> 1) & 1);
    cart_ultimax_phi2 = export.game & (export.exrom ^ 1) & ((~mode_phi1 >> 2) & 1);
    machine_update_memory_ptrs();
}

/****************************************************************************
* 
****************************************************************************/

/*
    magic formel switches GAME depending on ADDR ($E000-$FFFF)
*/

static int kernal_decoder(WORD addr)
{
    export_game = kernal_enabled || freeze_enabled;

    if (addr < 0xe000) {
        return 0;
    }

    return 1;
}

static void freeze_flipflop(int reset,int freeze,int clear)
{
    if (reset) {
        freeze_enabled = 1;
    } else {
        if (clear) {
            freeze_enabled = 0;
        } else if (freeze) {
            freeze_enabled = freeze;
        }
    }
/* DBG(("freeze_flipflop reset %d freeze %d clear %d -> freeze_enabled %d kernal_enabled %d\n",reset,freeze,clear,freeze_enabled,kernal_enabled)); */
}

#ifndef USE_ULTIMAX_CONFIG
static BYTE REGPARM1 magicformel_kernal64_read(WORD addr)
{
    if (kernal_decoder(addr)) {
        if (export_game) {
            return romh_banks[(addr & 0x1fff) + (romh_bank << 13)];
        }
    }
    return mem_read_without_ultimax(addr);
}
#endif

static void magicformel_init_mem(int bank)
{
#ifndef USE_ULTIMAX_CONFIG
    /* TODO */
#endif
}


/***************************************************************************
    very fake mc6821 "emulation" :)
 ***************************************************************************/

static int ctrlA = 0, ctrlB = 0;
static int dataA = 0, dataB = 0;
static int CB2 = 0, CB2state = 0;

#ifdef LOG_PORTS
static void mc6821_print_pa(BYTE data)
{
    /*
        PA (Output Data)
        
        A0,A1,A2       - ROM Bank
        A3             - extra ROM enable/disable (?)
        A4             - RAM enable/disable (?)
        A5-A7 unused
    */
    DBG(("6821 PA "));
    DBG(("[ROM BANK %02x] ", data & 0x07));
    DBG(("[EXTRA ROM %d] ", (data >> 3) & 1));
    DBG(("[RAM ENABLE %d] ", (data >> 4) & 1));
    DBG(("[UNUSED %02x] ", (data) & 0xe0));
}

static void mc6821_print_pb(BYTE data)
{
    BYTE page;

    /*
        PB (Output Data)
        
        B3,B2,B0,B1,B4 - RAM Bank
        B5-B6 (unused)
        B7             - rom enable (?)
    */
    /* ram_page = data & 0x1f; */
    page = (((data >> 3) & 1) << 0) |
           (((data >> 2) & 1) << 1) |
           (((data >> 0) & 1) << 2) |
           (((data >> 1) & 1) << 3) |
           (((data >> 4) & 1) << 4);

    DBG(("6821 PB "));
    DBG(("[RAM BANK %02x] ", page));
    DBG(("[ROM ENABLE %d] ", (data >> 7) & 1));
    DBG(("[UNUSED %02x] ", (data) & 0x60));
}
#endif

static void mc6821_reset(void)
{
    romh_bank = 0;      /* PA0..PA3 */
    io1_enabled = 0;    /* PA4 */

    ram_page = 0;       /* PB0..PB4 */
    kernal_enabled = 0; /* PB7 */

    CB2 = 0;            /* CB2 */
    CB2state = 0;

    ctrlA = 0;
    dataA = 0;

    ctrlB = 0;
    dataB = 0;
}

static BYTE mc6821_read(int port /* rs1 */,int reg /* rs0 */)
{
    BYTE data = 0;

    if (port == 0) {
        /* MC6821 Port A */
        if (reg == 1) {
            /* control register */
            data = ctrlA;
        } else {
            if (ctrlA & 0x04) {
                data = dataA;
            } else {
                /* ? */
            }
        }
    } else {
        /* MC6821 Port B */
        if (reg == 1) {
            /* control register */
            data = ctrlB;
        } else {
            if (ctrlB & 0x04) {
                data = dataB;
            } else {
                /* ? */
            }
        }
    }
    return data;
}

static void mc6821_store(int port /* rs1 */,int reg /* rs0 */,BYTE data)
{
    if (port == 0) {
        /* MC6821 Port A */
        if (reg == 1) {
            /* control register */
            ctrlA = data;
            /* DBG(("PA CTRL %02x    %02x %02x %02x %x %x\n",data,data&0x80,data&0x40,data&0x38,data&0x04,data&3)); */
        } else {
            if (ctrlA & 0x04) {
                /* output register */
                /* DBG(("PA DATA %02x\n",data)); */

                /*
                PA (Output Data)

                A0,A1,A2       - ROM Bank
                A3             - extra ROM enable/disable (?)
                A4             - RAM enable/disable (?)
                A5-A7 unused
                */
                if (hwversion == 0) {
                    romh_bank = data & 0x07;
                } else {
                    romh_bank = data & 0x0f;
                }
                freeze_flipflop(0 /* reset */,0 /* freeze */,CB2);
                kernal_decoder(0xdf00);
                magicformel_init_mem(romh_bank);

#ifdef LOG_BANKS
                log_bank(romh_bank);
#endif

                /* DBG(("ROM Bank %02x\n",data&0x0f)); */
                if (data & 0x10) {
                    /* DBG(("  %04x PA DATA %02x [IO1 RAM DISABLE?]\n",addr,data)); */
                    io1_enabled = 0;
                } else {
                    /* DBG(("  %04x PA DATA %02x [IO1 RAM ENABLE?]\n",addr,data)); */
                    io1_enabled = 1;
                }
                dataA = data;
#if 0
                mc6821_print_pa(dataA);
                mc6821_print_pb(dataB);
                DBG(("\n"));
#endif
            } else {
                /* data direction register */
                /* DBG(("PA DDR %02x\n",data)); */
            }
        }
    } else {
        /* MC6821 Port B */
        if (reg == 1) {
            /* control register */
            ctrlB = data;
            DBG(("PB CTRL %02x    %02x %02x %02x %x %x\n",data,data&0x80,data&0x40,data&0x38,data&0x04,data&3));

            if ((data & 0x30) == 0x30) {
                /* update CB2 immediately */
                CB2 = (data & 0x8) >> 3;
                CB2state = 0;

                freeze_flipflop(0 /* reset */, 0 /* freeze */, CB2);
                kernal_decoder(0xdf00);
                magicformel_init_mem(romh_bank);
            } else if ((data & 0x30) == 0x20) {
                /* TODO */
                CB2state = 1;
                DBG(("MF: PB CTRL TODO mode for CB2\n"));
            } else {
                DBG(("MF: PB CTRL unhandled mode %02x for CB2\n",(data&0x30)));
            }
        } else {
            if (ctrlB&0x04) {
                /* output register */
                /* DBG(("PB DATA %02x\n",data)); */

                /*
                PB (Output Data)

                B3,B2,B0,B1,B4 - RAM Bank
                B5-B6 (unused)
                B7             - rom enable (?)
                */
                /* ram_page = data & 0x1f; */
                ram_page =
                    (((data >> 3) & 1) << 0) |
                    (((data >> 2) & 1) << 1) |
                    (((data >> 0) & 1) << 2) |
                    (((data >> 1) & 1) << 3) |
                    (((data >> 4) & 1) << 4);

                /* DBG(("RAM Bank %02x\n",data&0x0f)); */

                if (CB2state == 1) {
                    CB2 = 0;
                }

                if (data & 0x80) {
                    /* DBG(("  %04x PB DATA %02x [ROM ENABLE?] [RAM BANK %02x]\n",0xdf00,data,ram_page)); */
                    kernal_enabled = 1;
                } else {
                    /* DBG(("  %04x PB DATA %02x [ROM DISABLE?] [RAM BANK %02x]\n",0xdf00,data,ram_page)); */
                    kernal_enabled = 0;
                }

                if (CB2state == 1) {
                    CB2state = 0;
                }

                freeze_flipflop(0 /* reset */,0 /* freeze */,CB2);
                kernal_decoder(0xdf00);
                magicformel_init_mem(romh_bank);

                dataB = data;
#ifdef LOG_PORTS
                mc6821_print_pa(dataA);
                mc6821_print_pb(dataB);
                DBG(("\n"));
#endif
            } else {
                /* data direction register */
                /* DBG(("PB DDR %02x\n",data)); */
            }
        }
    }
}


/****************************************************************************
* 
****************************************************************************/

/* some prototypes are needed */
static void REGPARM2 magicformel_io1_store(WORD addr, BYTE value);
static BYTE REGPARM1 magicformel_io1_read(WORD addr);
static void REGPARM2 magicformel_io2_store(WORD addr, BYTE value);
static BYTE REGPARM1 magicformel_io2_read(WORD addr);

static io_source_t magicformel_io1_device = {
    "MAGIC FORMEL",
    IO_DETACH_CART,
    NULL,
    0xde00, 0xdeff, 0xff,
    0,
    magicformel_io1_store,
    magicformel_io1_read
};

static io_source_t magicformel_io2_device = {
    "MAGIC FORMEL",
    IO_DETACH_CART,
    NULL,
    0xdf00, 0xdfff, 0xff,
    1, /* read is always valid */
    magicformel_io2_store,
    magicformel_io2_read
};

static io_source_list_t *magicformel_io1_list_item = NULL;
static io_source_list_t *magicformel_io2_list_item = NULL;

/* ---------------------------------------------------------------------*/

BYTE REGPARM1 magicformel_io1_read(WORD addr)
{
#ifdef DEBUG_IO1_NO_DISABLE
    if (1) {
#else
    if (io1_enabled) {
#endif
        magicformel_io1_device.io_source_valid = 1;
        return export_ram0[(ram_page << 8) + (addr & 0xff)];
    } else {
        magicformel_io1_device.io_source_valid = 0;
        DBG(("MF: read from disabled io1\n"));
    }
    return 0;
}

static void REGPARM2 magicformel_io1_store(WORD addr, BYTE value)
{
#ifdef DEBUG_IO1_NO_DISABLE
    if (1) {
#else
    if (io1_enabled) {
#endif
        export_ram0[(ram_page << 8) + (addr & 0xff)] = value;
    } else {
        DBG(("MF: write to disabled io1\n"));
    }
}

/*
    a0..a5 go to d0..d5
    a6..a7 go to rs1..rs0
    d1 goes to d7
*/

BYTE REGPARM1 magicformel_io2_read(WORD addr)
{
    int data, port, reg;

    /* fixme: what about d0..d5 ? */
    data = (addr & 0x3f);   /* d0..d5 d7 */
    port = (addr >> 7) & 1; /* rs1 */
    reg = (addr >> 6) & 1;  /* rs0 */

    DBG(("MF: read from io2 %04x data %02x port %02x reg %02x\n",addr,data,port,reg));

    return mc6821_read(port /* rs1 */, reg /* rs0 */);
}

void REGPARM2 magicformel_io2_store(WORD addr, BYTE value)
{
    int data, port, reg;

    data = (addr & 0x3f) | ((value & 2) << 6); /* d0..d5 d7 */
    port = (addr >> 7) & 1; /* rs1 */
    reg = (addr >> 6) & 1;  /* rs0 */

    mc6821_store(port /* rs1 */, reg /* rs0 */, data);
}


/****************************************************************************
* 
****************************************************************************/
/* ---------------------------------------------------------------------*/

BYTE REGPARM1 magicformel_roml_read(WORD addr)
{
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_roml_store(WORD addr, BYTE value)
{
    mem_store_without_ultimax(addr, value);
}

BYTE REGPARM1 magicformel_romh_read(WORD addr)
{
    if (export_game) {
        if (kernal_decoder(addr)) {
            return romh_banks[(addr & 0x1fff) + (romh_bank << 13)];
        }
    }
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_romh_store(WORD addr, BYTE value)
{
#if 1
    mem_store_without_ultimax(addr, value);
#else
    BYTE page;

    if (export_game) {
        if(kernal_decoder(addr)) {
            DBG(("MF: CARTRAM romh store %04x %02x '%c'\n",addr,value,value));

            page = (((addr >> 11) & 1) << 0) |
                   (((addr >> 10) & 1) << 1) |
                   (((addr >> 8) & 1) << 2) |
                   (((addr >> 9) & 1) << 3) |
                   (((addr >> 12) & 1) << 4);

            export_ram0[(page << 8) + (addr & 0xff)] = value;
        }
    } else {
        DBG(("MF: romh store %04x %02x '%c'\n",addr,value,value));
        mem_store_without_ultimax(addr, value);
    }
#endif
}

BYTE REGPARM1 magicformel_1000_7fff_read(WORD addr)
{
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_1000_7fff_store(WORD addr, BYTE value)
{
    mem_store_without_ultimax(addr, value);
}

BYTE REGPARM1 magicformel_a000_bfff_read(WORD addr)
{
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_a000_bfff_store(WORD addr, BYTE value)
{
    mem_store_without_ultimax(addr, value);
}

BYTE REGPARM1 magicformel_c000_cfff_read(WORD addr)
{
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_c000_cfff_store(WORD addr, BYTE value)
{
    mem_store_without_ultimax(addr, value);
}

BYTE REGPARM1 magicformel_d000_dfff_read(WORD addr)
{
    return mem_read_without_ultimax(addr);
}

void REGPARM2 magicformel_d000_dfff_store(WORD addr, BYTE value)
{
    mem_store_without_ultimax(addr, value);
}

/****************************************************************************/

/* ultimax, rom bank 1 */
void magicformel_freeze(void)
{
    DBG(("MF: freeze\n"));
    /* mc6821_reset(); */

    kernal_enabled = 1;   /* PB7 */
    romh_bank = 0x01;

    freeze_flipflop(0 /* reset */, 1 /* freeze */, CB2);
    kernal_decoder(0xfffe);

#ifdef USE_ULTIMAX_CONFIG
    magicformel_config_changed(2,(1<<7)|((romh_bank&0x0f)<<3)|(3), CMODE_READ);
#else
    magicformel_config_changed(2,(1<<7)|((romh_bank&0x0f)<<3)|(2), CMODE_READ);
#endif

    magicformel_init_mem(romh_bank);
}

void magicformel_config_init(void)
{
    DBG(("MF: init\n"));

    kernal_enabled = 1;   /* PB7 */

    freeze_flipflop(1 /* reset */,0 /* freeze */,CB2);
    kernal_decoder(0xfffe);

#ifdef USE_ULTIMAX_CONFIG
    magicformel_config_changed(2, (romh_bank<<3)|(3), CMODE_READ);
#else
    magicformel_config_changed(2, (romh_bank<<3)|(2), CMODE_READ);
#endif

    magicformel_init_mem(romh_bank);
}

void magicformel_reset(void)
{
    DBG(("MF: reset\n"));
    mc6821_reset();
}

void magicformel_config_setup(BYTE *rawcart)
{
    memcpy(roml_banks, rawcart, 0x20000);
    memcpy(romh_banks, rawcart, 0x20000);
}

/*
    load CRT, handles 64k (v1.2), 64k+32k (v2.0), 64k+64k (v2.0)
*/

int magicformel_crt_attach(FILE *fd, BYTE *rawcart)
{
    BYTE chipheader[0x10];
    int i, cnt = 0;

    for (i = 0; i <= 15; i++) {
        if (fread(chipheader, 0x10, 1, fd) < 1) {
            break;
        }

        if (chipheader[0xb] > 15) {
            return -1;
        }

        if (fread(&rawcart[chipheader[0xb] << 13], 0x2000, 1, fd) < 1) {
            return -1;
        }
        cnt++;
    }

    if (cnt == 8) {
        DBG(("MF: 64k ROM loaded.\n"));
        hwversion = 0;
    } else if (cnt == 12) {
        DBG(("MF: 64k+32k ROM loaded.\n"));
        hwversion = 1;
    } else if (cnt == 16) {
        DBG(("MF: 2*64k ROM loaded.\n"));
        hwversion = 2;
    } else {
        return -1;
    }

    if (c64export_add(&export_res) < 0) {
        return -1;
    }

    magicformel_io1_list_item = c64io_register(&magicformel_io1_device);
    magicformel_io2_list_item = c64io_register(&magicformel_io2_device);

    return 0;
}

void magicformel_detach(void)
{
    c64export_remove(&export_res);
    c64io_unregister(magicformel_io1_list_item);
    c64io_unregister(magicformel_io2_list_item);
    magicformel_io1_list_item = NULL;
    magicformel_io2_list_item = NULL;
}
