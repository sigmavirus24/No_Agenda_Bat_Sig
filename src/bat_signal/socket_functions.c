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

int sockets_connect(void){
	struct addrinfo hints;
	struct addrinfo *ret, *p;
	int sockfd = -1;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

	/* Not the actual sockfd yet, just using it until I get the sockfd */
	if(0 != getaddrinfo("search.twitter.com", "80", &hints, &ret)){
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

   freeaddrinfo(ret);

	return sockfd;
}

void my_close(int *sockfd){
	close(*sockfd);
	*sockfd = -1;
}

void sockets_request(int sockfd, char *get, char *host, char **mem, int memsize){
	char *p;
	int r;

	if(*mem != NULL){
		p = *mem;

		if(0 > (r = write(sockfd, get, strlen(get))))
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
   my_close(&sockfd);
}

void notice(t_tweet *tweet){
	int sock;
   int retval, len;
	struct addrinfo hints, *res, *p;
   char *message;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;

   if(0 > (retval = getaddrinfo("localhost", "33333", &hints, &res))){
      printf("Error, cannot connect to irc bot.\n");
      exit(1);
   }

   for(p = res, sock = -1; p; p = p->ai_next){
      if(0 > (sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
         continue;

      if(0 > connect(sock, p->ai_addr, p->ai_addrlen)){
         close(sock);
         sock = -1;
         continue;
      }
   }

   freeaddrinfo(res);

   if(sock > 0){
      len = strlen(tweet->text) + strlen(tweet->tweet_url) + 2;
      message = (char *)xmalloc(len * sizeof(char));

      sprintf(message, "%s %s", tweet->text, tweet->tweet_url);
      for(retval = 0; retval < len; )
         retval += send(sock, message, len, 0);
      free(message);
      close(sock);
   }
}
/* vim: set sw=3 ts=3 et: */
