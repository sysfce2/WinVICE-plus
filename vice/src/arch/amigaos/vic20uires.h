/*
 * vic20uires.h
 *
 * Written by
 *  Mathias Roslund <vice.emu@amidog.se>
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

#ifndef _VIC20UIRES_H_
#define _VIC20UIRES_H_

#include "uires.h"
#include "intl.h"

static struct TranslateNewMenu UI_MENU_NAME[] = {
  TITLE(IDMS_FILE, NULL)
    ITEM(IDMS_AUTOSTART_IMAGE, NULL, IDM_AUTOSTART)
    ITEMSEPARATOR()
    ITEM(IDMS_ATTACH_DISK_IMAGE, NULL, NULL)
      SUB(IDMS_DRIVE_8, "8", IDM_ATTACH_8)
      SUB(IDMS_DRIVE_9, "9",  IDM_ATTACH_9)
      SUB(IDMS_DRIVE_10, "0", IDM_ATTACH_10)
      SUB(IDMS_DRIVE_11, "1", IDM_ATTACH_11)
    ITEM(IDMS_DETACH_DISK_IMAGE, NULL, NULL)
      SUB(IDMS_DRIVE_8, NULL, IDM_DETACH_8)
      SUB(IDMS_DRIVE_9, NULL, IDM_DETACH_9)
      SUB(IDMS_DRIVE_10, NULL, IDM_DETACH_10)
      SUB(IDMS_DRIVE_11, NULL, IDM_DETACH_11)
      SUBSEPARATOR()
      SUB(IDMS_ALL, "A", IDM_DETACH_ALL)
    ITEM(IDMS_FLIP_LIST, NULL, NULL)
      SUB(IDMS_ADD_CURRENT_IMAGE, "I",     IDM_FLIP_ADD)
      SUB(IDMS_REMOVE_CURRENT_IMAGE, "K",  IDM_FLIP_REMOVE)
      SUB(IDMS_ATTACH_NEXT_IMAGE, "N",     IDM_FLIP_NEXT)
      SUB(IDMS_ATTACH_PREVIOUS_IMAGE, "B", IDM_FLIP_PREVIOUS)
      SUBSEPARATOR()
      SUB(IDMS_LOAD_FLIP_LIST, NULL, IDM_FLIP_LOAD)
      SUB(IDMS_SAVE_FLIP_LIST, NULL, IDM_FLIP_SAVE)
    ITEMSEPARATOR()
    ITEM(IDMS_ATTACH_TAPE_IMAGE, "T", IDM_ATTACH_TAPE)
    ITEM(IDMS_DETACH_TAPE_IMAGE, NULL,           IDM_DETACH_TAPE)
    ITEM(IDMS_DATASSETTE_CONTROL, NULL, NULL)
      SUB(IDMS_STOP, NULL,          IDM_DATASETTE_CONTROL_STOP)
      SUB(IDMS_START, NULL,         IDM_DATASETTE_CONTROL_START)
      SUB(IDMS_FORWARD, NULL,       IDM_DATASETTE_CONTROL_FORWARD)
      SUB(IDMS_REWIND, NULL,        IDM_DATASETTE_CONTROL_REWIND)
      SUB(IDMS_RECORD, NULL,        IDM_DATASETTE_CONTROL_RECORD)
      SUB(IDMS_RESET, NULL,         IDM_DATASETTE_CONTROL_RESET)
      SUB(IDMS_RESET_COUNTER, NULL, IDM_DATASETTE_RESET_COUNTER)
    ITEMSEPARATOR()
#if defined(UI_VIC20)
    ITEM(IDMS_ATTACH_CART_IMAGE, NULL, NULL)
      SUB(IDMS_4_8_16KB_AT_2000, NULL, IDM_CART_VIC20_8KB_2000)
      SUB(IDMS_4_8_16KB_AT_4000, NULL, IDM_CART_VIC20_16KB_4000)
      SUB(IDMS_4_8_16KB_AT_6000, NULL, IDM_CART_VIC20_8KB_6000)
      SUB(IDMS_4_8KB_AT_A000,    NULL, IDM_CART_VIC20_8KB_A000)
      SUB(IDMS_4KB_AT_B000,      NULL, IDM_CART_VIC20_4KB_B000)
    ITEM(IDMS_DETACH_CART_IMAGE, NULL, IDM_CART_DETACH)
    ITEMSEPARATOR()
#endif
#if defined(UI_PLUS4)
    ITEM(IDMS_ATTACH_CART_IMAGE, NULL, NULL)
      SUB(IDMS_C1_LOW_IMAGE, NULL, IDM_CART_ATTACH_C1LO)
      SUB(IDMS_C1_HIGH_IMAGE, NULL, IDM_CART_ATTACH_C1HI)
      SUB(IDMS_C2_LOW_IMAGE, NULL, IDM_CART_ATTACH_C2LO)
      SUB(IDMS_C2_HIGH_IMAGE, NULL, IDM_CART_ATTACH_C2HI)
      SUBSEPARATOR()
      SUB(IDMS_FUNCTION_LOW_3PLUS1, NULL, IDM_CART_ATTACH_FUNCLO)
      SUB(IDMS_FUNCTION_HIGH_3PLUS1, NULL, IDM_CART_ATTACH_FUNCHI)
    ITEM(IDMS_DETACH_CART_IMAGE, NULL, IDM_CART_DETACH)
    ITEMSEPARATOR()
#endif
#if !defined(UI_CBM2) && !defined(UI_PET) && !defined(UI_PLUS4) && !defined(UI_VIC20)
    ITEM(IDMS_ATTACH_CART_IMAGE, NULL, NULL)
      SUB(IDMS_CRT_IMAGE, NULL,               IDM_CART_ATTACH_CRT)
      SUBSEPARATOR()
      SUB(IDMS_GENERIC_8KB_IMAGE, NULL,       IDM_CART_ATTACH_8KB)
      SUB(IDMS_GENERIC_16KB_IMAGE, NULL,      IDM_CART_ATTACH_16KB)
      SUB(IDMS_ACTION_REPLAY_IMAGE, NULL,     IDM_CART_ATTACH_AR)
      SUB(IDMS_ATOMIC_POWER_IMAGE, NULL,      IDM_CART_ATTACH_AT)
      SUB(IDMS_EPYX_FASTLOAD_IMAGE, NULL,     IDM_CART_ATTACH_EPYX)
      SUB(IDMS_IEEE488_INTERFACE_IMAGE, NULL, IDM_CART_ATTACH_IEEE488)
      SUB(IDMS_RETRO_REPLAY_IMAGE, NULL,      IDM_CART_ATTACH_RR)
      SUB(IDMS_IDE64_INTERFACE_IMAGE, NULL,   IDM_CART_ATTACH_IDE64)
      SUB(IDMS_SUPER_SNAPSHOT_4_IMAGE, NULL,  IDM_CART_ATTACH_SS4)
      SUB(IDMS_SUPER_SNAPSHOT_5_IMAGE, NULL,  IDM_CART_ATTACH_SS5)
      SUB(IDMS_STRUCTURED_BASIC_IMAGE, NULL,  IDM_CART_ATTACH_STB)
/* AmigaOS only support one submenu level */
    ITEM(IDMS_EXPERT_CART, NULL, NULL)
      SUB(IDMS_ENABLE, NULL, IDM_CART_ENABLE_EXPERT)
      SUBSEPARATOR()
      SUBTOGGLE(IDMS_OFF, NULL,    IDM_CART_MODE_OFF)
      SUBTOGGLE(IDMS_PRG, NULL,    IDM_CART_MODE_PRG)
      SUBTOGGLE(IDMS_ON, NULL,     IDM_CART_MODE_ON)
    ITEM(IDMS_SET_CART_AS_DEFAULT, NULL, IDM_CART_SET_DEFAULT)
    ITEMTOGGLE(IDMS_RESET_ON_CART_CHANGE, NULL, IDM_TOGGLE_CART_RESET)
    ITEMSEPARATOR()
    ITEM(IDMS_DETACH_CART_IMAGE, NULL,  IDM_CART_DETACH)
    ITEM(IDMS_CART_FREEZE, "Z", IDM_CART_FREEZE)
    ITEMSEPARATOR()
#endif
    ITEMTOGGLE(IDMS_PAUSE, NULL, IDM_PAUSE)
    ITEM(IDMS_MONITOR, "M", IDM_MONITOR)
    ITEM(IDMS_RESET, NULL, NULL)
      SUB(IDMS_HARD, NULL, IDM_RESET_HARD)
      SUB(IDMS_SOFT, "R", IDM_RESET_SOFT)
      SUBSEPARATOR()
      SUB(IDMS_DRIVE_8, NULL, IDM_RESET_DRIVE8)
      SUB(IDMS_DRIVE_9, NULL, IDM_RESET_DRIVE9)
      SUB(IDMS_DRIVE_10, NULL, IDM_RESET_DRIVE10)
      SUB(IDMS_DRIVE_11, NULL, IDM_RESET_DRIVE11)
    ITEMSEPARATOR()
    ITEM(IDMS_EXIT, "X", IDM_EXIT)
  TITLE(IDMS_SNAPSHOT, NULL)
    ITEM(IDMS_LOAD_SNAPSHOT_IMAGE, NULL, IDM_SNAPSHOT_LOAD)
    ITEM(IDMS_SAVE_SNAPSHOT_IMAGE, NULL, IDM_SNAPSHOT_SAVE)
    ITEM(IDMS_LOAD_QUICKSNAPSHOT_IMAGE, "L", IDM_LOADQUICK)
    ITEM(IDMS_SAVE_QUICKSNAPSHOT_IMAGE, "S", IDM_SAVEQUICK)
    ITEMSEPARATOR()
    ITEM(IDMS_START_STOP_RECORDING, NULL,       IDM_EVENT_TOGGLE_RECORD)
    ITEM(IDMS_START_STOP_PLAYBACK, NULL,        IDM_EVENT_TOGGLE_PLAYBACK)
    ITEM(IDMS_SET_MILESTONE, "G",  IDM_EVENT_SETMILESTONE)
    ITEM(IDMS_RETURN_TO_MILESTONE, "H",      IDM_EVENT_RESETMILESTONE)
    ITEM(IDMS_RECORDING_START_MODE, NULL, NULL)
      SUBTOGGLE(IDMS_SAVE_NEW_SNAPSHOT, NULL,      IDM_EVENT_START_MODE_SAVE)
      SUBTOGGLE(IDMS_LOAD_EXISTING_SNAPSHOT, NULL, IDM_EVENT_START_MODE_LOAD)
      SUBTOGGLE(IDMS_START_WITH_RESET, NULL,       IDM_EVENT_START_MODE_RESET)
      SUBTOGGLE(IDMS_OVERWRITE_PLAYBACK, NULL,       IDM_EVENT_START_MODE_PLAYBACK)
    ITEM(IDMS_SELECT_HISTORY_DIR, NULL,    IDM_EVENT_DIRECTORY)
    ITEMSEPARATOR()
    ITEM(IDMS_SAVE_STOP_MEDIA_FILE, "C", IDM_MEDIAFILE)
    ITEMSEPARATOR()
#if !defined(UI_C128) && !defined(UI_CBM2) && !defined(UI_PET) && !defined(UI_PLUS4) && !defined(UI_VIC20)
    ITEM(IDMS_NETPLAY, NULL, IDM_NETWORK_SETTINGS)
#endif
  TITLE(IDMS_OPTIONS, NULL)
    ITEM(IDMS_REFRESH_RATE, NULL, NULL)
      SUBTOGGLE(IDMS_AUTO, NULL, IDM_REFRESH_RATE_AUTO)
      SUBTOGGLE(IDMS_1_1, NULL, IDM_REFRESH_RATE_1)
      SUBTOGGLE(IDMS_1_2, NULL, IDM_REFRESH_RATE_2)
      SUBTOGGLE(IDMS_1_3, NULL, IDM_REFRESH_RATE_3)
      SUBTOGGLE(IDMS_1_4, NULL, IDM_REFRESH_RATE_4)
      SUBTOGGLE(IDMS_1_5, NULL, IDM_REFRESH_RATE_5)
      SUBTOGGLE(IDMS_1_6, NULL, IDM_REFRESH_RATE_6)
      SUBTOGGLE(IDMS_1_7, NULL, IDM_REFRESH_RATE_7)
      SUBTOGGLE(IDMS_1_8, NULL, IDM_REFRESH_RATE_8)
      SUBTOGGLE(IDMS_1_9, NULL, IDM_REFRESH_RATE_9)
      SUBTOGGLE(IDMS_1_10, NULL, IDM_REFRESH_RATE_10)
  ITEM(IDMS_MAXIMUM_SPEED, NULL, NULL)
      SUBTOGGLE(IDMS_200_PERCENT, NULL, IDM_MAXIMUM_SPEED_200)
      SUBTOGGLE(IDMS_100_PERCENT, NULL, IDM_MAXIMUM_SPEED_100)
      SUBTOGGLE(IDMS_50_PERCENT, NULL, IDM_MAXIMUM_SPEED_50)
      SUBTOGGLE(IDMS_20_PERCENT, NULL, IDM_MAXIMUM_SPEED_20)
      SUBTOGGLE(IDMS_10_PERCENT, NULL, IDM_MAXIMUM_SPEED_10)
      SUBTOGGLE(IDMS_NO_LIMIT, NULL, IDM_MAXIMUM_SPEED_NO_LIMIT)
      SUBSEPARATOR()
      SUBTOGGLE(IDMS_CUSTOM, NULL, IDM_MAXIMUM_SPEED_CUSTOM)
    ITEMTOGGLE(IDMS_WARP_MODE, "W", IDM_TOGGLE_WARP_MODE)
    ITEMSEPARATOR()
#if !defined(UI_C128)
    ITEMTOGGLE(IDMS_FULLSCREEN, "D", IDM_TOGGLE_FULLSCREEN)
    ITEMSEPARATOR()
#endif
    ITEMTOGGLE(IDMS_VIDEO_CACHE, NULL, IDM_TOGGLE_VIDEOCACHE)
    ITEMTOGGLE(IDMS_DOUBLE_SIZE, NULL, IDM_TOGGLE_DOUBLESIZE)
    ITEMTOGGLE(IDMS_DOUBLE_SCAN, NULL, IDM_TOGGLE_DOUBLESCAN)
#if !defined(UI_CBM2) && !defined(UI_PET)
    ITEMTOGGLE(IDMS_PAL_EMULATION, NULL, IDM_TOGGLE_FASTPAL)
    ITEMTOGGLE(IDMS_SCALE2X, NULL, IDM_TOGGLE_SCALE2X)
#endif
#if defined(UI_C128)
    ITEM(IDMS_VDC_SETTINGS, NULL, NULL)
      SUBTOGGLE(IDMS_DOUBLE_SIZE, NULL, IDM_TOGGLE_VDC_DOUBLESIZE)
      SUBTOGGLE(IDMS_DOUBLE_SCAN, NULL, IDM_TOGGLE_VDC_DOUBLESCAN)
      SUBSEPARATOR()
      SUBTOGGLE(IDMS_64KB_VIDEO_MEMORY, NULL, IDM_TOGGLE_VDC64KB)
#endif
    ITEMSEPARATOR()
    ITEM(IDMS_SWAP_JOYSTICKS, "J", IDM_SWAP_JOYSTICK)
    ITEMSEPARATOR()
    ITEMTOGGLE(IDMS_SOUND_PLAYBACK, NULL, IDM_TOGGLE_SOUND)
    ITEMSEPARATOR()
    ITEMTOGGLE(IDMS_TRUE_DRIVE_EMU, NULL, IDM_TOGGLE_DRIVE_TRUE_EMULATION)
    ITEMTOGGLE(IDMS_VIRTUAL_DEVICE_TRAPS, NULL, IDM_TOGGLE_VIRTUAL_DEVICES)
#if defined(UI_CBM2) || defined(UI_PET)
    ITEM(IDMS_DRIVE_SYNC_FACTOR, NULL, NULL)
      SUBTOGGLE(IDMS_PAL,  NULL, IDM_SYNC_FACTOR_PAL)
      SUBTOGGLE(IDMS_NTSC, NULL, IDM_SYNC_FACTOR_NTSC)
#endif
    ITEMSEPARATOR()
#if !defined(UI_CBM2) && !defined(UI_PET)
    ITEM(IDMS_VIDEO_STANDARD, NULL, NULL)
      SUBTOGGLE(IDMS_PAL_G, NULL, IDM_SYNC_FACTOR_PAL)
      SUBTOGGLE(IDMS_NTSC_M, NULL, IDM_SYNC_FACTOR_NTSC)
#if !defined(UI_C128) && !defined(UI_PLUS4) && !defined(UI_VIC20)
      SUBTOGGLE(IDMS_OLD_NTSC_M, NULL, IDM_SYNC_FACTOR_NTSCOLD)
#endif
#endif
#if !defined(UI_PLUS4)
    ITEMSEPARATOR()
    ITEMTOGGLE(IDMS_EMU_ID, NULL, IDM_TOGGLE_EMUID)
#if defined(UI_VIC20)
    ITEMTOGGLE (IDMS_VIC_1112_IEEE_488, NULL, IDM_IEEE488)
#endif
#if defined(UI_C128)
    ITEMTOGGLE(IDMS_IEEE_488_INTERFACE, NULL, IDM_IEEE488)
#endif
#if !defined(UI_CBM2) && !defined(UI_PET) && !defined(UI_VIC20)
    ITEMTOGGLE(IDMS_1351_MOUSE, "Q", IDM_MOUSE)
#endif
#endif
  TITLE(IDMS_SETTINGS, NULL)
    ITEM(IDMS_VIDEO_SETTINGS, NULL,     IDM_VIDEO_SETTINGS)
#if defined(UI_VIC20)
    ITEM(IDMS_VIC_SETTINGS, NULL, IDM_VIC_SETTINGS)
#endif
#if defined(UI_CBM2)
    ITEM(IDMS_CBM2_SETTINGS, NULL, IDM_CBM2_SETTINGS)
#endif
#if defined(UI_PET)
    ITEM(IDMS_PET_SETTINGS, NULL, IDM_PET_SETTINGS)
#endif
    ITEM(IDMS_PERIPHERAL_SETTINGS, NULL,IDM_DEVICEMANAGER)
    ITEM(IDMS_DRIVE_SETTINGS, NULL,     IDM_DRIVE_SETTINGS)
    ITEM(IDMS_DATASETTE_SETTINGS, NULL, IDM_DATASETTE_SETTINGS)
#if defined(UI_PLUS4)
    ITEM(IDMS_PLUS4_SETTINGS, NULL, IDM_PLUS4_SETTINGS)
#endif
#if !defined(UI_PET) && !defined(UI_PLUS4) && !defined(UI_VIC20)
    ITEM(IDMS_VICII_SETTINGS, NULL,    IDM_VICII_SETTINGS)
#endif
#ifdef AMIGA_OS4
    ITEM(IDMS_JOYSTICK_SETTINGS, NULL,  IDM_JOY_SETTINGS)
#else
    ITEM(IDMS_JOYSTICK_SETTINGS, NULL,  NULL)
      SUB(IDMS_JOYSTICK_DEVICE_SELECT, NULL, IDM_JOY_DEVICE_SELECTION)
      SUB(IDMS_JOYSTICK_FIRE_SELECT, NULL, IDM_JOY_FIRE_SELECTION)
#endif
    ITEM(IDMS_KEYBOARD_SETTINGS, NULL,  IDM_KEYBOARD_SETTINGS)
    ITEM(IDMS_SOUND_SETTINGS, NULL,     IDM_SOUND_SETTINGS)
#if !defined(UI_PET) && !defined(UI_PLUS4) && !defined(UI_VIC20)
    ITEM(IDMS_SID_SETTINGS, NULL,       IDM_SID_SETTINGS)
#endif
    ITEM(IDMS_ROM_SETTINGS, NULL,       IDM_ROM_SETTINGS)
    ITEM(IDMS_RAM_SETTINGS, NULL,       IDM_RAM_SETTINGS)
    ITEM(IDMS_RS232_SETTINGS, NULL,       IDM_RS232_SETTINGS)
#if defined(UI_C128)
    ITEM(IDMS_C128_SETTINGS, NULL,      IDM_C128_SETTINGS)
#endif
    ITEM(IDMS_CART_IO_SETTINGS, NULL, NULL)
#if !defined(UI_VIC20) && !defined(UI_CBM2) && !defined(UI_PET) && !defined(UI_PLUS4)
      SUB(IDMS_REU_SETTINGS, NULL,      IDM_REU_SETTINGS)
      SUB(IDMS_GEORAM_SETTINGS, NULL,      IDM_GEORAM_SETTINGS)
      SUB(IDMS_RAMCART_SETTINGS, NULL,      IDM_RAMCART_SETTINGS)
#if !defined(UI_C128)
      SUB(IDMS_PLUS60K_SETTINGS, NULL,      IDM_PLUS60K_SETTINGS)
      SUB(IDMS_PLUS256K_SETTINGS, NULL,      IDM_PLUS256K_SETTINGS)
      SUB(IDMS_256K_SETTINGS, NULL,      IDM_C64_256K_SETTINGS)
#endif
      SUB(IDMS_IDE64_SETTINGS, NULL,    IDM_IDE64_SETTINGS)
#ifdef HAVE_TFE
      SUB(IDMS_ETHERNET_SETTINGS, NULL, IDM_TFE_SETTINGS)
#endif
#endif
#if !defined(UI_VIC20)
      SUB(IDMS_ACIA_SETTINGS, NULL,    IDM_ACIA_SETTINGS)
#endif
#if defined(UI_PET)
      SUB(IDMS_PETREU_SETTINGS, NULL,    IDM_PETREU_SETTINGS)
#endif
#if !defined(UI_CBM2) && !defined(UI_PET) && !defined(UI_PLUS4)
      SUB(IDMS_RS232_USERPORT_SETTINGS, NULL,    IDM_RS232USER_SETTINGS)
#endif
    ITEMSEPARATOR()
    ITEM(IDMS_SAVE_CURRENT_SETTINGS, NULL, IDM_SETTINGS_SAVE)
    ITEM(IDMS_LOAD_SAVED_SETTINGS, NULL,   IDM_SETTINGS_LOAD)
    ITEM(IDMS_SET_DEFAULT_SETTINGS, NULL,  IDM_SETTINGS_DEFAULT)
    ITEMSEPARATOR()
    ITEMTOGGLE(IDMS_SAVE_SETTING_ON_EXIT, NULL, IDM_TOGGLE_SAVE_SETTINGS_ON_EXIT)
    ITEMTOGGLE(IDMS_CONFIRM_ON_EXIT, NULL,       IDM_TOGGLE_CONFIRM_ON_EXIT)
  TITLE(IDMS_LANGUAGE, NULL)
    ITEM(IDMS_LANGUAGE_ENGLISH, NULL, IDM_LANGUAGE_ENGLISH)
    ITEM(IDMS_LANGUAGE_GERMAN, NULL, IDM_LANGUAGE_GERMAN)
    ITEM(IDMS_LANGUAGE_FRENCH, NULL, IDM_LANGUAGE_FRENCH)
    ITEM(IDMS_LANGUAGE_ITALIAN, NULL, IDM_LANGUAGE_ITALIAN)
    ITEM(IDMS_LANGUAGE_DUTCH, NULL, IDM_LANGUAGE_DUTCH)
    ITEM(IDMS_LANGUAGE_POLISH, NULL, IDM_LANGUAGE_POLISH)
    ITEM(IDMS_LANGUAGE_SWEDISH, NULL, IDM_LANGUAGE_SWEDISH)
  TITLE(IDMS_HELP, NULL)
    ITEM(IDMS_ABOUT, NULL,             IDM_ABOUT)
    ITEMSEPARATOR()
    ITEM(IDMS_COMMAND_LINE_OPTIONS, NULL, IDM_CMDLINE)
    ITEMSEPARATOR()
    ITEM(IDMS_CONTRIBUTORS, NULL,         IDM_CONTRIBUTORS)
    ITEM(IDMS_LICENSE, NULL,              IDM_LICENSE)
    ITEM(IDMS_NO_WARRANTY, NULL,          IDM_WARRANTY)
  END()
};

static struct NewMenu UI_TRANSLATED_MENU_NAME[sizeof(UI_MENU_NAME)/sizeof(UI_MENU_NAME[0])];

#endif /* _VIC20UIRES_H_ */

