/** \file   archdep_program_path.h
 * \brief   Retrieve path of currently running binary - header
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 *
 * Get path to running executable.
 *
 * OS support:
 *  - Linux
 *  - Windows
 *  - MacOS
 *  - BeOS/Haiku (untested)
 *  - AmigaOS (untested)
 *  - OS/2 (untested)
 *  - MS-DOS (untested)
 *
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

#ifndef VICE_ARCHDEP_PROGRAM_PATH_H
#define VICE_ARCHDEP_PROGRAM_PATH_H

#include "vice.h"

void    archdep_program_path_set_argv0(char *argv0);
char *  archdep_program_path(void);
void    archdep_program_path_free(void);

#endif
