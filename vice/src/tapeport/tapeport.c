/*
 * tapeport.c - tapeport handling.
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

#include "datasette.h"
#include "lib.h"
#include "log.h"
#include "tapelog.h"
#include "tapeport.h"
#include "tapertc.h"
#include "uiapi.h"

static tapeport_device_list_t tapeport_head = { NULL, NULL, NULL };

static int tapeport_active = 1;

static int tapeport_devices = 0;

/* ---------------------------------------------------------------------------------------------------------- */

tapeport_device_list_t *tapeport_device_register(tapeport_device_t *device)
{
    tapeport_device_list_t *current = &tapeport_head;
    tapeport_device_list_t *retval = NULL;
    int found = 0;
    int use_id = 0;

    if (tapeport_devices > 0) {
        while (!found) {
            if (current->device) {
                if (current->device->id == (tapeport_devices - 1)) {
                    found = 1;
                }
            }
            if (!found) {
                if (current->next) {
                    current = current->next;
                } else {
                    found = 2;
                }
            }
        }
        if (found == 2) {
            log_warning(LOG_DEFAULT, "TAPEPORT insertion error, highest id not present in chain");
            return NULL;
        } else {
            if (current->device->trigger_flux_change_passthrough || current->device->set_tape_sense_passthrough) {
                use_id = tapeport_devices;
            } else {
                if (device->trigger_flux_change_passthrough || device->set_tape_sense_passthrough) {
                    use_id = tapeport_devices - 1;
                    ++current->device->id;
                } else {
                    ui_error("last tapeport device %s does not support passthrough, and %s does not support passthrough either", current->device->name, device->name);
                    return NULL;
                }
            }
        }
    }

    retval = lib_malloc(sizeof(tapeport_device_list_t));

    while (current->next != NULL) {
        current = current->next;
    }
    current->next = retval;
    retval->previous = current;
    retval->device = device;
    retval->next = NULL;
    retval->device->id = use_id;

    ++tapeport_devices;

    return retval;
}

void tapeport_device_unregister(tapeport_device_list_t *device)
{
    tapeport_device_list_t *prev;
    tapeport_device_list_t *current = &tapeport_head;
    int id;
    int end = 0;

    if (device) {
        prev = device->previous;
        prev->next = device->next;

        if (device->next) {
            device->next->previous = prev;
        }

        id = device->device->id;

        lib_free(device);

        if (tapeport_devices != id + 1) {
            while (!end) {
                if (current->device) {
                    if (current->device->id > id) {
                        --current->device->id;
                    }
                }
                if (current->next) {
                    current = current->next;
                } else {
                    end = 1;
                }
            }
        }
        --tapeport_devices;
    }
}

/* ---------------------------------------------------------------------------------------------------------- */

void tapeport_set_motor(int flag)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (tapeport_active) {
        while (!end) {
            if (current->device) {
                if (current->device->id == 0) {
                    if (current->device->set_motor) {
                        current->device->set_motor(flag);
                    }
                    end = 1;
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_toggle_write_bit(int write_bit)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (tapeport_active) {
        while (!end) {
            if (current->device) {
                if (current->device->id == 0) {
                    if (current->device->toggle_write_bit) {
                        current->device->toggle_write_bit(write_bit);
                    }
                    end = 1;
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_set_motor_next(int flag, int id)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (id == tapeport_devices - 1) {
        return;
    }

    if (tapeport_active) {
        while (!end) {
            if (current->device) {
                if (current->device->id == id + 1) {
                    if (current->device->set_motor) {
                        current->device->set_motor(flag);
                    }
                    end = 1;
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_toggle_write_bit_next(int write_bit, int id)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (id == tapeport_devices - 1) {
        return;
    }

    if (tapeport_active) {
        while (!end) {
            if (current->device) {
                if (current->device->id == id + 1) {
                    if (current->device->toggle_write_bit) {
                        current->device->toggle_write_bit(write_bit);
                    }
                    end = 1;
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_reset(void)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (tapeport_active) {
        while (!end) {
            if (current->device) {
                if (current->device->reset) {
                    current->device->reset();
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_trigger_flux_change(unsigned int on, int id)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (!tapeport_active) {
        return;
    }

    if (id == 0) {
        machine_trigger_flux_change(on);
    } else {
        while (!end) {
            if (current->device) {
                if (current->device->id == id - 1) {
                    if (current->device->trigger_flux_change_passthrough) {
                        current->device->trigger_flux_change_passthrough(on);
                    }
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

void tapeport_set_tape_sense(int sense, int id)
{
    tapeport_device_list_t *current = &tapeport_head;
    int end = 0;

    if (!tapeport_active) {
        return;
    }

    if (id == 0) {
        machine_set_tape_sense(sense);
    } else {
        while (!end) {
            if (current->device) {
                if (current->device->id == id - 1) {
                    if (current->device->set_tape_sense_passthrough) {
                        current->device->set_tape_sense_passthrough(sense);
                    }
                }
            }
            if (current->next) {
                current = current->next;
            } else {
                end = 1;
            }
        }
    }
}

/* ---------------------------------------------------------------------------------------------------------- */

int tapeport_resources_init(void)
{
    if (tapelog_resources_init() < 0) {
        return -1;
    }
    if (tapertc_resources_init() < 0) {
        return -1;
    }

    return 0;
}

void tapeport_resources_shutdown(void)
{
    tapeport_device_list_t *current;

    tapertc_resources_shutdown();

    current = tapeport_head.next;

    while (current) {
        tapeport_device_unregister(current);
        current = tapeport_head.next;
    }
}


int tapeport_cmdline_options_init(void)
{
    if (tapelog_cmdline_options_init() < 0) {
        return -1;
    }

    if (tapertc_cmdline_options_init() < 0) {
        return -1;
    }

    return 0;
}

void tapeport_enable(int val)
{
    tapeport_active = val ? 1 : 0;
}
