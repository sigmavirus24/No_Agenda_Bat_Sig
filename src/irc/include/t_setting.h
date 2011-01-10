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
#ifndef __T_SETTING_H__
#define __T_SETTING_H__

typedef struct s_setting {
   char *pass;
   char *nick;
   char *realname;
   char *serv;
   char *port;
   char *_chans;
   char **chans;
   int num_chans;
   char *_ausers;
   char **ausers;
   int listening_pid;
} t_setting;
#endif

/* vim: set ts=3 sw=3 et: */
