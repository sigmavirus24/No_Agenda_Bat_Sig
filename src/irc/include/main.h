/* No Agenda Bat Signal
 * Copyright (C) 2010
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of this License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * See LICENSE file for license details.
 */
#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <signal.h>
#include <string.h>

int fork(void);

#include "t_setting.h"
#include "utils.h"
void read_rc(char *, t_setting *);
void borked_pipe(int);

#endif
/* vim: set ts=3 sw=3 et: */
