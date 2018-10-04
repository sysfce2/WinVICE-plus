/** \file   archdep_default_save_resource_file_name.c
 * \brief   Retrieve default resource file path for saving
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 *
 * Get path to default resource file (vicerc/vice.ini)
 */

/*
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
#include "archdep_defs.h"
#include "archdep_default_resource_file_name.h"

#include "archdep_default_save_resource_file_name.h"


/** \brief  Get path to default resource file
 *
 * \return  heap-allocated path, free with lib_free()
 */
char *archdep_default_save_resource_file_name(void)
{
    return archdep_default_resource_file_name();
}
