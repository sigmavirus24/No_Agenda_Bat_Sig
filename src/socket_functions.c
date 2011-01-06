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

#include "./include/socket_functions.h"

int sockets_connect(struct addrinfo *hints, char *def){
	struct addrinfo *ret, *p;
	int sockfd;

	/* Not the actual sockfd yet, just using it until I get the sockfd */
	if(0 != getaddrinfo(def, "80", hints, &ret)){
		printf("Getaddrinfo error.\n");
		exit(1);
	}

	for(p = ret; p; p = p->ai_next){
		if(0 > (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
			continue;
		if(0 > connect(sockfd, p->ai_addr, p->ai_addrlen)){
			close(sockfd);
			sockfd = -1;
		} else
			break;
	}

	return sockfd;
}

void sockets_request(int sockfd, char *def, char *host, char **mem, int memsize){
	char *p;
	int r;

	if(*mem != NULL){
		p = *mem;

		if(0 > (r = write(sockfd, def, strlen(def))))
			return;
		if(0 > (r = write(sockfd, host, strlen(host))))
			return;

		if(0 > (r = read(sockfd, p, memsize - 1))){
			printf("Nothing to see here!\n");
			free(p);
			close(sockfd);
			exit(1);
		}
		*(p + r) = '\0';
	}
}

void my_close(int *sockfd){
	close(*sockfd);
	*sockfd = -1;
}
/* vim: set sw=8 ts=8: */
