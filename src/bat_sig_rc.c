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

void bat_sig_rc(char **jingle, char **browse, char *gtk, char *ssl){
	FILE *rc;
	char *s;
	char *home;
	char *file;
	int i;
	int len;

	if(*jingle && *browse && gtk && ssl){
		home = getenv("HOME");
		len = strlen(home) + strlen("/.nabatsignalrc") + 1;
		file = (char *)xmalloc(len * sizeof(char));
		memset(file, '\0', len);
		strcat(file, home);
		strcat(file, "/.nabatsignalrc");
		if(NULL != (rc = fopen(file, "r"))){
			s = (char *)xmalloc(MAX_SIZE * sizeof(char));
			memset(s, '\0', MAX_SIZE);
			while(NULL != fgets(s, MAX_SIZE, rc)){
				len = strlen(s);
				*(s + len - 1) = '\0';
				i = find_char(s, ' ');
				if(!strncmp(s, "browser", i))
					*browse = strdup(s + i + 1);
				if(!strncmp(s, "jingle", i))
					*jingle = strdup(s + i + 1);
				if(!strncmp(s, "ssl", i))
					if(!strcmp(s + i + 1, "on"))
						*ssl = 1;
				if(!strncmp(s, "gtk", i))
					if(!strcmp(s + i + 1, "on"))
						*gtk = 1;
			}
		} else {
			*jingle = NULL;
			*browse = NULL;
		}
	}
}
