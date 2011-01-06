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
#include "./include/main.h"

int main(int argc, char **argv){
		char *nick, *pass, *name;
		char *host, *port;
		char **list;
		int fd;

		setbuf(stdout, NULL);
		setbuf(stderr, NULL);
		if(argc > 1){
				printf("No Agenda IRC Bot Version "VERSION": no arguments allowed. See ~/.nabotrc.\n");
				return 0;
		}

		fd = dial(host, port);
		if(fd){
				if(SIG_ERR == signal(SIGPIPE, borked_pipe)){
						printf("Cannot set up signal handler.\n");
						close(fd);
						return 0;
				}
				identify(fd, pass, nick, name);
		} else {
				printf("No Agenda IRC Bot Version "VERSION": not able to connect to "host" at "port".\n");
				return 0;
		}
		return 0;
}

void borked_pipe(int signo){
		printf("Connection with network broken.");
		exit(0);
}
/* vim: set ts=8 sw=8: */
