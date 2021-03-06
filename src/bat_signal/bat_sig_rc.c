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
#include "./include/bat_sig_rc.h"
#define MAX_SIZE 1024

void bat_sig_rc(t_setting *s){
	FILE *rc;
	char *str;
	char *home;
	char *file;
	int i;
	int len;

	if(s->path_to_jingle && s->browser){
		home = getenv("HOME");
		len = strlen(home) + strlen("/.nabatsignalrc") + 1;
		file = (char *)xmalloc(len * sizeof(char));
		memset(file, '\0', len);
		strcat(file, home);
		strcat(file, "/.nabatsignalrc");
		if(NULL != (rc = fopen(file, "r"))){
			str = (char *)xmalloc(MAX_SIZE * sizeof(char));
			memset(str, '\0', MAX_SIZE);
			while(NULL != fgets(str, MAX_SIZE, rc)){
				len = strlen(str) - 1;
				*(str + len) = '\0';
				i = find_char(str, ' ');
				if(!strncmp(str, "jingle", i)){
					s->path_to_jingle = strdup(str + i + 1);
					s->set_from_rc[0] = s->jingle_set = 1;
				} else if(!strncmp(str, "browser", i)){
					s->browser = strdup(str + i + 1);
					s->set_from_rc[1] = s->browser_set = 1;
				} else if(!strncmp(str, "mp3player", i)){
					s->mp3player = strdup(str + i + 1);
					s->set_from_rc[2] = s->mp3player_set = 1;
            } else if(!strncmp(str, "gtk", i)){
					if(!strcmp(str + i + 1, "on"))
						s->gtk_on = 1;
            } else if(!strncmp(str, "irc", i)){
					if(!strcmp(str + i + 1, "on"))
						s->irc = 1;
            }
			}
         free(file);
		} else {
			s->mp3player = s->browser = s->path_to_jingle = NULL;
			s->mp3player_set = s->browser_set = s->jingle_set = 0;
			memset(s->set_from_rc, 0, 3);
			s->gtk_on = 0;
		}
	}
}
/* vim: set sw=3 ts=3 et: */
