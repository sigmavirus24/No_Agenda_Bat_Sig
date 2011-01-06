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
#define __GNU_SOURCE

#include "./include/utils.h"

int dial(char *host, char *port){
	struct addrinfo hints;
	struct addrinfo *p, *res;
	int sock;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if(0 != getaddrinfo(host, port, &hints, &res)){
		printf("Can not retrieve server information.\n");
		return -1;
	}

	for(p = res; p; p = p->ai_next){
		if(0 > (sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
			continue;
		if(0 > connect(sock, p->ai_addr, p->ai_addrlen)){
			close(sock);
			sock = -1;
		} else
			break;
	}
	
	return sock;
}

void parse_srvr(char *in, char **list){
	char *p;

	if(in && *list){
		/* As long as it's real and there are names on the list */
	}
}

void zero(char *p){
	memset(p, '\0', MAXLEN);
}

void make_command(char *dest, char *str, int i){
	char *command;
	static char *nick;

	if(dest && str){
		zero(dest);
		switch(i){
			case 0:
				command = "PASS ";
				break;
			case 1:
				command = "NICK ";
				nick = str;
				break;
			case 2:
				command = "USER ";
				break;
			default:
				break;
		}

		strcat(dest, command);
		if(i == 2){
			strcat(dest, nick);
			strcat(dest, " 0 * :");
		}
		strcat(dest, str);
		strcat(dest, CRLF);
	}
}

void wrap_send(int fd, char *buff){
	int rv;
	int len;

	rv = 0;
	len = strlen(buff);
	while(rv < len)
		rv += send(fd, buff, len - rv, 0);
}

void identify(int fd, char *pass, char *nick, char *name){
	char tmp[MAXLEN];

	if(fd && pass && nick && name){
		/* 1st: PASS yourpassword */
		make_command(tmp, pass, 0);
		wrap_send(fd, tmp);
		/* 2nd: NICK yournick */
		make_command(tmp, nick, 1);
		wrap_send(fd, tmp);
		/* 3rd: USER nick 0 * :realname */
		make_command(tmp, name, 2);
		wrap_send(fd, tmp);
	} else {
		printf("Could not identify.\n");
		exit(0);
	}
}
/* vim: set ts=8 sw=8: */
