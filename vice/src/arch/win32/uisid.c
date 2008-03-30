/*
 * uisid.c - Implementation of the SID settings dialog box.
 *
 * Written by
 *  Andreas Matthies <andreas.matthies@gmx.net>
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

#include <windows.h>

#include "datasette.h"
#include "res.h"
#include "resources.h"
#include "ui.h"
#include "uisid.h"
#include "winmain.h"

static char* ui_sid_samplemethod[] = 
{
    "fast", "interpolating", "resample", NULL
};


static void enable_sid_controls(HWND hwnd)
{
    int is_enabled;

    resources_get_value("SidUseResid",
        (resource_value_t *)&is_enabled);

    EnableWindow(GetDlgItem(hwnd, IDC_SID_RESID_SAMPLING),
                 is_enabled);
    EnableWindow(GetDlgItem(hwnd, IDC_SID_RESID_PASSBAND),
                 is_enabled);

    resources_get_value("SidStereo",
        (resource_value_t *)&is_enabled);

    EnableWindow(GetDlgItem(hwnd, IDC_SID_STEREOADDRESS),
                 is_enabled);

}


static void CreateAndGetSidAddress(HWND hwnd, int mode)
{
    /* mode: 0=create, 1=get */
    char st[12];
    int res_value;
    int adr, hadr, ladr, index = -1;
    HWND sid_hwnd=GetDlgItem(hwnd,IDC_SID_STEREOADDRESS);
    int cursel = SendMessage(GetDlgItem
                (hwnd,IDC_SID_STEREOADDRESS),CB_GETCURSEL,0,0);
    resources_get_value("SidStereoAddressStart",
        (resource_value_t *)&res_value);

    for (hadr = 0x4; hadr < 0x10; hadr++)
    {
        for (ladr = 0x0; ladr < 0x10; ladr += 2)
        {
            if ((hadr == 0x4) && (ladr > 0)
                || (hadr > 0x4) && (hadr < 0x8)
                || (hadr > 0xd))
            {
                index++;
                sprintf(st, "$D%X%X0", hadr, ladr);
                adr = 0xd000 + hadr*0x100 + ladr*0x10;

                if (mode == 0)
                {
                    SendMessage(sid_hwnd,CB_ADDSTRING,0,(LPARAM)st);
                    if (adr == res_value)
                        SendMessage(sid_hwnd,CB_SETCURSEL,(WPARAM) index,0);
                } else if (index == cursel)
                {
                    resources_set_value("SidStereoAddressStart",
                        (resource_value_t) adr);
                    return;
                }
            }
        }
    }
}


static void init_sid_dialog(HWND hwnd)
{
HWND    sid_hwnd;
int     res_value;
int     res_value_loop;
char    st[10];

    resources_get_value("SidFilters",
        (resource_value_t *)&res_value);
    CheckDlgButton(hwnd, IDC_SID_FILTERS, res_value
                   ? BST_CHECKED : BST_UNCHECKED);
    
    resources_get_value("SidStereo",
        (resource_value_t *)&res_value);
    CheckDlgButton(hwnd, IDC_SID_STEREO, res_value
                   ? BST_CHECKED : BST_UNCHECKED);

    CreateAndGetSidAddress(hwnd, 0);

    resources_get_value("SidUseResid",
        (resource_value_t *)&res_value);
    CheckDlgButton(hwnd, IDC_SID_RESID, res_value
                   ? BST_CHECKED : BST_UNCHECKED);

    resources_get_value("SidModel",
        (resource_value_t *)&res_value);
    CheckRadioButton(hwnd, IDC_SID_6581, IDC_SID_8580, 
                     res_value ? IDC_SID_8580 : IDC_SID_6581);

    resources_get_value("SidResidSampling",
        (resource_value_t *)&res_value);
    sid_hwnd=GetDlgItem(hwnd,IDC_SID_RESID_SAMPLING);
    for (res_value_loop = 0; ui_sid_samplemethod[res_value_loop]; res_value_loop++) {
        SendMessage(sid_hwnd,CB_ADDSTRING,0,
            (LPARAM)ui_sid_samplemethod[res_value_loop]);
    }
    SendMessage(sid_hwnd,CB_SETCURSEL,(WPARAM)res_value,0);

    resources_get_value("SidResidPassband",
        (resource_value_t *)&res_value);
    itoa(res_value, st, 10);
    SetDlgItemText(hwnd, IDC_SID_RESID_PASSBAND, st);

    enable_sid_controls(hwnd);
    
    
}



static BOOL CALLBACK dialog_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    int command;
    char st[4];
    resource_value_t dummy;

    switch (msg) {
        case WM_COMMAND:
            command=LOWORD(wparam);
            switch (command) {
                case IDC_SID_RESID:
                    resources_set_value("SidUseResid", (resource_value_t)
                        (IsDlgButtonChecked
                            (hwnd,IDC_SID_RESID)==BST_CHECKED ?
                            1 : 0 ));
                    enable_sid_controls(hwnd);
                    break;
                case IDC_SID_STEREO:
                    resources_toggle("SidStereo", &dummy);
                    enable_sid_controls(hwnd);
                    break;
                case IDC_SID_6581:
                    resources_set_value("SidModel", (resource_value_t) 0);
                    break;
                case IDC_SID_8580:
                    resources_set_value("SidModel", (resource_value_t) 1);
                    break;
                case IDOK:
                    resources_set_value("SidFilters", (resource_value_t)
                        (IsDlgButtonChecked
                            (hwnd,IDC_SID_FILTERS)==BST_CHECKED ?
                            1 : 0 ));

                    resources_set_value("SidStereo", (resource_value_t)
                        (IsDlgButtonChecked
                            (hwnd,IDC_SID_STEREO)==BST_CHECKED ?
                            1 : 0 ));

                    resources_set_value("SidResidSampling",(resource_value_t)
                        SendMessage(GetDlgItem(hwnd,IDC_SID_RESID_SAMPLING), 
                            CB_GETCURSEL,0,0));
                    
                    GetDlgItemText(hwnd, IDC_SID_RESID_PASSBAND, st, 4);
                    resources_set_value("SidResidPassband",
                        (resource_value_t) atoi(st));

                    CreateAndGetSidAddress(hwnd, 1);


                case IDCANCEL:
                    EndDialog(hwnd,0);
                    return TRUE;
            }
            return FALSE;
        case WM_CLOSE:
            EndDialog(hwnd,0);
            return TRUE;
        case WM_INITDIALOG:
            init_sid_dialog(hwnd);
            return TRUE;
    }
    return FALSE;
}


void ui_sid_settings_dialog(HWND hwnd)
{
    DialogBox(winmain_instance,(LPCTSTR)IDD_SID_SETTINGS_DIALOG,hwnd,dialog_proc);
}

