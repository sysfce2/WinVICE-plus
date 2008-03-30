/*
 * cbm2drive.c
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
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

#include "ieee.h"
#include "machine-drive.h"


int machine_drive_resources_init(void)
{
    return ieee_drive_resources_init();
}

int machine_drive_cmdline_options_init(void)
{
    return ieee_drive_cmdline_options_init();
}

void machine_drive_init(struct drive_context_s *drv)
{
    ieee_drive_init(drv);
}

void machine_drive_reset(struct drive_context_s *drv)
{
    ieee_drive_reset(drv);
}

void machine_drive_mem_init(struct drive_context_s *drv, unsigned int type)
{
    ieee_drive_mem_init(drv, type);
}

void machine_drive_setup_context(struct drive_context_s *drv)
{
    ieee_drive_setup_context(drv);
}

void machine_drive_idling_method(unsigned int dnr)
{
}

void machine_drive_vsync_hook(void)
{
}

void machine_drive_handle_job_code(unsigned int dnr)
{
}

void machine_drive_rom_load(void)
{
    ieee_drive_rom_load();
}

void machine_drive_rom_setup_image(unsigned int dnr)
{
    ieee_drive_rom_setup_image(dnr);
}

int machine_drive_rom_read(unsigned int type, WORD addr, BYTE *data)
{
    if (ieee_drive_rom_read(type, addr, data) == 0)
        return 0;

    return -1;
}

int machine_drive_rom_check_loaded(unsigned int type)
{
    if (ieee_drive_rom_check_loaded(type) == 0)
        return 0;

    return -1;
}

void machine_drive_rom_do_checksum(unsigned int dnr)
{
    ieee_drive_rom_do_checksum(dnr);
}

int machine_drive_snapshot_read(struct drive_context_s *ctxptr,
                                struct snapshot_s *s)
{
    return ieee_drive_snapshot_read(ctxptr, s);
}

int machine_drive_snapshot_write(struct drive_context_s *ctxptr,
                                 struct snapshot_s *s)
{
    return ieee_drive_snapshot_write(ctxptr, s);
}

int machine_drive_image_attach(struct disk_image_s *image, unsigned int unit)
{
    return ieee_drive_image_attach(image, unit);
}

int machine_drive_image_detach(struct disk_image_s *image, unsigned int unit)
{
    return ieee_drive_image_detach(image, unit);
}

void machine_drive_stub(void)
{

}

