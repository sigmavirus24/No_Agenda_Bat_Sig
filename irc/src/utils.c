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
		if(0 > connect(sockfd, p->ai_addr, p->ai_addrlen)){
			close(sock);
			sock = -1;
		} else
			break;
	}
	
	return sock;
}

void parse_srvr(char *in, char **list){
}

/* vim: set ts=8 sw=8: */
