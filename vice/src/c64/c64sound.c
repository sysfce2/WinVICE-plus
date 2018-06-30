/*
 * c64sound.c - C64/C128 sound emulation.
 *
 * Written by
 *  Marco van den Heuvel <blackystardust68@yahoo.com>
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

#include "vice.h"

#include <stdio.h>
#include <string.h>

#include "cartio.h"
#include "cartridge.h"
#include "machine.h"
#include "sid.h"
#include "sid-resources.h"
#include "sound.h"
#include "types.h"

static uint8_t machine_sid2_read(uint16_t addr)
{
    return sid2_read(addr);
}

static void machine_sid2_store(uint16_t addr, uint8_t byte)
{
    sid2_store(addr, byte);
}

static uint8_t machine_sid3_read(uint16_t addr)
{
    return sid3_read(addr);
}

static void machine_sid3_store(uint16_t addr, uint8_t byte)
{
    sid3_store(addr, byte);
}

static uint8_t machine_sid4_read(uint16_t addr)
{
    return sid4_read(addr);
}

static void machine_sid4_store(uint16_t addr, uint8_t byte)
{
    sid4_store(addr, byte);
}

static uint8_t machine_sid5_read(uint16_t addr)
{
    return sid5_read(addr);
}

static void machine_sid5_store(uint16_t addr, uint8_t byte)
{
    sid5_store(addr, byte);
}

static uint8_t machine_sid6_read(uint16_t addr)
{
    return sid6_read(addr);
}

static void machine_sid6_store(uint16_t addr, uint8_t byte)
{
    sid6_store(addr, byte);
}

static uint8_t machine_sid7_read(uint16_t addr)
{
    return sid7_read(addr);
}

static void machine_sid7_store(uint16_t addr, uint8_t byte)
{
    sid7_store(addr, byte);
}

static uint8_t machine_sid8_read(uint16_t addr)
{
    return sid8_read(addr);
}

static void machine_sid8_store(uint16_t addr, uint8_t byte)
{
    sid8_store(addr, byte);
}
/* ---------------------------------------------------------------------*/

static io_source_t stereo_sid_device = {
    "Stereo SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xde00, 0xde1f, 0x1f,
    1, /* read is always valid */
    machine_sid2_store,
    machine_sid2_read,
    NULL, /* TODO: peek */
    sid2_dump,
    0,
    0,
    0
};

static io_source_t triple_sid_device = {
    "Triple SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xdf00, 0xdf1f, 0x1f,
    1, /* read is always valid */
    machine_sid3_store,
    machine_sid3_read,
    NULL, /* TODO: peek */
    sid3_dump,
    0,
    0,
    0
};

static io_source_t quad_sid_device = {
    "Quad SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xdf80, 0xdf9f, 0x1f,
    1, /* read is always valid */
    machine_sid4_store,
    machine_sid4_read,
    NULL, /* TODO: peek */
    sid4_dump,
    0,
    0,
    0
};

static io_source_t fifth_sid_device = {
    "Fifth SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xd480, 0xd49f, 0x1f,
    1, /* read is always valid */
    machine_sid5_store,
    machine_sid5_read,
    NULL, /* TODO: peek */
    sid5_dump,
    0,
    0,
    0
};

static io_source_t sixth_sid_device = {
    "Sixth SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xd4a0, 0xd4bf, 0x1f,
    1, /* read is always valid */
    machine_sid6_store,
    machine_sid6_read,
    NULL, /* TODO: peek */
    sid6_dump,
    0,
    0,
    0
};

static io_source_t seventh_sid_device = {
    "Seventh SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xd4c0, 0xd4df, 0x1f,
    1, /* read is always valid */
    machine_sid7_store,
    machine_sid7_read,
    NULL, /* TODO: peek */
    sid7_dump,
    0,
    0,
    0
};

static io_source_t eigth_sid_device = {
    "Eigth SID",
    IO_DETACH_RESOURCE,
    "SidStereo",
    0xd4e0, 0xd4ff, 0x1f,
    1, /* read is always valid */
    machine_sid8_store,
    machine_sid8_read,
    NULL, /* TODO: peek */
    sid8_dump,
    0,
    0,
    0
};

static io_source_list_t *stereo_sid_list_item = NULL;
static io_source_list_t *triple_sid_list_item = NULL;
static io_source_list_t *quad_sid_list_item = NULL;
static io_source_list_t *fifth_sid_list_item = NULL;
static io_source_list_t *sixth_sid_list_item = NULL;
static io_source_list_t *seventh_sid_list_item = NULL;
static io_source_list_t *eigth_sid_list_item = NULL;

/* ---------------------------------------------------------------------*/

static sound_chip_t sid_sound_chip = {
    sid_sound_machine_open,
    sid_sound_machine_init,
    sid_sound_machine_close,
    sid_sound_machine_calculate_samples,
    sid_sound_machine_store,
    sid_sound_machine_read,
    sid_sound_machine_reset,
    sid_sound_machine_cycle_based,
    sid_sound_machine_channels,
    1 /* chip enabled */
};

static uint16_t sid_sound_chip_offset = 0;

void sid_sound_chip_init(void)
{
    sid_sound_chip_offset = sound_chip_register(&sid_sound_chip);
}

/* ---------------------------------------------------------------------*/

int machine_sid2_check_range(unsigned int sid2_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid2_adr >= 0xd400 && sid2_adr <= 0xd4e0) || (sid2_adr >= 0xd700 && sid2_adr <= 0xdfe0)) {
            sid_stereo_address_start = sid2_adr;
            stereo_sid_device.start_address = sid2_adr;
            sid_stereo_address_end = sid2_adr + 0x1f;
            stereo_sid_device.end_address = sid2_adr + 0x1f;
            if (stereo_sid_list_item != NULL) {
                io_source_unregister(stereo_sid_list_item);
                stereo_sid_list_item = io_source_register(&stereo_sid_device);
            } else {
                if (sid_stereo >= 1) {
                    stereo_sid_list_item = io_source_register(&stereo_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid2_adr >= 0xd400 && sid2_adr <= 0xdfe0) {
            sid_stereo_address_start = sid2_adr;
            stereo_sid_device.start_address = sid2_adr;
            sid_stereo_address_end = sid2_adr + 0x1f;
            stereo_sid_device.end_address = sid2_adr + 0x1f;
            if (stereo_sid_list_item != NULL) {
                io_source_unregister(stereo_sid_list_item);
                stereo_sid_list_item = io_source_register(&stereo_sid_device);
            } else {
                if (sid_stereo >= 1) {
                    stereo_sid_list_item = io_source_register(&stereo_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid3_check_range(unsigned int sid3_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid3_adr >= 0xd400 && sid3_adr <= 0xd4e0) || (sid3_adr >= 0xd700 && sid3_adr <= 0xdfe0)) {
            sid_triple_address_start = sid3_adr;
            triple_sid_device.start_address = sid3_adr;
            sid_triple_address_end = sid3_adr + 0x1f;
            triple_sid_device.end_address = sid3_adr + 0x1f;
            if (triple_sid_list_item != NULL) {
                io_source_unregister(triple_sid_list_item);
                triple_sid_list_item = io_source_register(&triple_sid_device);
            } else {
                if (sid_stereo >= 2) {
                    triple_sid_list_item = io_source_register(&triple_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid3_adr >= 0xd400 && sid3_adr <= 0xdfe0) {
            sid_triple_address_start = sid3_adr;
            triple_sid_device.start_address = sid3_adr;
            sid_triple_address_end = sid3_adr + 0x1f;
            triple_sid_device.end_address = sid3_adr + 0x1f;
            if (triple_sid_list_item != NULL) {
                io_source_unregister(triple_sid_list_item);
                triple_sid_list_item = io_source_register(&triple_sid_device);
            } else {
                if (sid_stereo >= 2) {
                    triple_sid_list_item = io_source_register(&triple_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid4_check_range(unsigned int sid4_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid4_adr >= 0xd400 && sid4_adr <= 0xd4e0) || (sid4_adr >= 0xd700 && sid4_adr <= 0xdfe0)) {
            sid_quad_address_start = sid4_adr;
            quad_sid_device.start_address = sid4_adr;
            sid_quad_address_end = sid4_adr + 0x1f;
            quad_sid_device.end_address = sid4_adr + 0x1f;
            if (quad_sid_list_item != NULL) {
                io_source_unregister(quad_sid_list_item);
                quad_sid_list_item = io_source_register(&quad_sid_device);
            } else {
                if (sid_stereo >= 3) {
                    quad_sid_list_item = io_source_register(&quad_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid4_adr >= 0xd400 && sid4_adr <= 0xdfe0) {
            sid_quad_address_start = sid4_adr;
            quad_sid_device.start_address = sid4_adr;
            sid_quad_address_end = sid4_adr + 0x1f;
            quad_sid_device.end_address = sid4_adr + 0x1f;
            if (quad_sid_list_item != NULL) {
                io_source_unregister(quad_sid_list_item);
                quad_sid_list_item = io_source_register(&quad_sid_device);
            } else {
                if (sid_stereo >= 3) {
                    quad_sid_list_item = io_source_register(&quad_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid5_check_range(unsigned int sid_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid_adr >= 0xd400 && sid_adr <= 0xd4e0) || (sid_adr >= 0xd700 && sid_adr <= 0xdfe0)) {
            sid_5th_address_start = sid_adr;
            fifth_sid_device.start_address = sid_adr;
            sid_5th_address_end = sid_adr + 0x1f;
            fifth_sid_device.end_address = sid_adr + 0x1f;
            if (fifth_sid_list_item != NULL) {
                io_source_unregister(fifth_sid_list_item);
                fifth_sid_list_item = io_source_register(&fifth_sid_device);
            } else {
                if (sid_stereo >= 4) {
                    fifth_sid_list_item = io_source_register(&fifth_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid_adr >= 0xd400 && sid_adr <= 0xdfe0) {
            sid_5th_address_start = sid_adr;
            fifth_sid_device.start_address = sid_adr;
            sid_5th_address_end = sid_adr + 0x1f;
            fifth_sid_device.end_address = sid_adr + 0x1f;
            if (fifth_sid_list_item != NULL) {
                io_source_unregister(fifth_sid_list_item);
                fifth_sid_list_item = io_source_register(&fifth_sid_device);
            } else {
                if (sid_stereo >= 4) {
                    fifth_sid_list_item = io_source_register(&fifth_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid6_check_range(unsigned int sid_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid_adr >= 0xd400 && sid_adr <= 0xd4e0) || (sid_adr >= 0xd700 && sid_adr <= 0xdfe0)) {
            sid_6th_address_start = sid_adr;
            sixth_sid_device.start_address = sid_adr;
            sid_6th_address_end = sid_adr + 0x1f;
            sixth_sid_device.end_address = sid_adr + 0x1f;
            if (sixth_sid_list_item != NULL) {
                io_source_unregister(sixth_sid_list_item);
                sixth_sid_list_item = io_source_register(&sixth_sid_device);
            } else {
                if (sid_stereo >= 5) {
                    sixth_sid_list_item = io_source_register(&sixth_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid_adr >= 0xd400 && sid_adr <= 0xdfe0) {
            sid_6th_address_start = sid_adr;
            sixth_sid_device.start_address = sid_adr;
            sid_6th_address_end = sid_adr + 0x1f;
            sixth_sid_device.end_address = sid_adr + 0x1f;
            if (sixth_sid_list_item != NULL) {
                io_source_unregister(sixth_sid_list_item);
                sixth_sid_list_item = io_source_register(&sixth_sid_device);
            } else {
                if (sid_stereo >= 5) {
                    sixth_sid_list_item = io_source_register(&sixth_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid7_check_range(unsigned int sid_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid_adr >= 0xd400 && sid_adr <= 0xd4e0) || (sid_adr >= 0xd700 && sid_adr <= 0xdfe0)) {
            sid_7th_address_start = sid_adr;
            seventh_sid_device.start_address = sid_adr;
            sid_7th_address_end = sid_adr + 0x1f;
            seventh_sid_device.end_address = sid_adr + 0x1f;
            if (seventh_sid_list_item != NULL) {
                io_source_unregister(seventh_sid_list_item);
                seventh_sid_list_item = io_source_register(&seventh_sid_device);
            } else {
                if (sid_stereo >= 6) {
                    seventh_sid_list_item = io_source_register(&seventh_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid_adr >= 0xd400 && sid_adr <= 0xdfe0) {
            sid_7th_address_start = sid_adr;
            seventh_sid_device.start_address = sid_adr;
            sid_7th_address_end = sid_adr + 0x1f;
            seventh_sid_device.end_address = sid_adr + 0x1f;
            if (seventh_sid_list_item != NULL) {
                io_source_unregister(seventh_sid_list_item);
                seventh_sid_list_item = io_source_register(&seventh_sid_device);
            } else {
                if (sid_stereo >= 6) {
                    seventh_sid_list_item = io_source_register(&seventh_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}

int machine_sid8_check_range(unsigned int sid_adr)
{
    if (machine_class == VICE_MACHINE_C128) {
        if ((sid_adr >= 0xd400 && sid_adr <= 0xd4e0) || (sid_adr >= 0xd700 && sid_adr <= 0xdfe0)) {
            sid_8th_address_start = sid_adr;
            eigth_sid_device.start_address = sid_adr;
            sid_8th_address_end = sid_adr + 0x1f;
            eigth_sid_device.end_address = sid_adr + 0x1f;
            if (eigth_sid_list_item != NULL) {
                io_source_unregister(eigth_sid_list_item);
                eigth_sid_list_item = io_source_register(&eigth_sid_device);
            } else {
                if (sid_stereo >= 7) {
                    eigth_sid_list_item = io_source_register(&eigth_sid_device);
                }
            }
            return 0;
        }
    } else {
        if (sid_adr >= 0xd400 && sid_adr <= 0xdfe0) {
            sid_8th_address_start = sid_adr;
            eigth_sid_device.start_address = sid_adr;
            sid_8th_address_end = sid_adr + 0x1f;
            eigth_sid_device.end_address = sid_adr + 0x1f;
            if (eigth_sid_list_item != NULL) {
                io_source_unregister(eigth_sid_list_item);
                eigth_sid_list_item = io_source_register(&eigth_sid_device);
            } else {
                if (sid_stereo >= 7) {
                    eigth_sid_list_item = io_source_register(&eigth_sid_device);
                }
            }
            return 0;
        }
    }
    return -1;
}


void machine_sid2_enable(int val)
{
    if (stereo_sid_list_item != NULL) {
        io_source_unregister(stereo_sid_list_item);
        stereo_sid_list_item = NULL;
    }
    if (triple_sid_list_item != NULL) {
        io_source_unregister(triple_sid_list_item);
        triple_sid_list_item = NULL;
    }
    if (quad_sid_list_item != NULL) {
        io_source_unregister(quad_sid_list_item);
        quad_sid_list_item = NULL;
    }
    if (fifth_sid_list_item != NULL) {
        io_source_unregister(fifth_sid_list_item);
        fifth_sid_list_item = NULL;
    }
    if (sixth_sid_list_item != NULL) {
        io_source_unregister(sixth_sid_list_item);
        sixth_sid_list_item = NULL;
    }
    if (seventh_sid_list_item != NULL) {
        io_source_unregister(seventh_sid_list_item);
        seventh_sid_list_item = NULL;
    }
    if (eigth_sid_list_item != NULL) {
        io_source_unregister(eigth_sid_list_item);
        eigth_sid_list_item = NULL;
    }

    if (val >= 1) {
        stereo_sid_list_item = io_source_register(&stereo_sid_device);
    }
    if (val >= 2) {
        triple_sid_list_item = io_source_register(&triple_sid_device);
    }
    if (val >= 3) {
        quad_sid_list_item = io_source_register(&quad_sid_device);
    }
    if (val >= 4) {
        fifth_sid_list_item = io_source_register(&fifth_sid_device);
    }
    if (val >= 5) {
        sixth_sid_list_item = io_source_register(&sixth_sid_device);
    }
    if (val >= 6) {
        seventh_sid_list_item = io_source_register(&seventh_sid_device);
    }
    if (val >= 7) {
        eigth_sid_list_item = io_source_register(&eigth_sid_device);
    }
}

void sound_machine_prevent_clk_overflow(sound_t *psid, CLOCK sub)
{
    sid_sound_machine_prevent_clk_overflow(psid, sub);
}

char *sound_machine_dump_state(sound_t *psid)
{
    return sid_sound_machine_dump_state(psid);
}

void sound_machine_enable(int enable)
{
    sid_sound_machine_enable(enable);
}
