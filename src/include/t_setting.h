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
#ifndef __T_SETTINGS_H__
#define __T_SETTINGS_H__

typedef struct s_settings {
	char set_from_rc[3];
	char *path_to_jingle;
	char jingle_set;
	char *browser;
	char browser_set;
	char *mp3player;
	char mp3player_set;
	char gtk_on;
	char use_ssl;
	char irc;
} t_setting;

#endif
/* vim: set sw=8 ts=8: */
