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

void parse_srvr(char *in, char **list, int fd){
   char *p;
   char *n;
   char *t;
   char *s;
   char *next;
   char tmp[128];

   if(in && *in && *list){
      next = in;
      while(next && *next){
         memset(tmp, '\0', 128);
         /* As long as it's real and there are names on the list */
         n = in;
         if(*n == ':'){
            in = slice(++n, ' '); /* Get slice after user@servername */
            if(*in == 0)
               return;
            slice(n, '!'); /* Get username */
         }
         next = slice(in, '\r'); /* Terminate the \r\n and ignore that part */
         next++;
         p = slice(in, ' ');
         t = slice(p, ':');
         for(s = p + strlen(p) - 1; s > p && isspace(*s); s--)
            ;
         *(s + 1) = '\0';

         if(!strcmp(in, "PONG")) /* Server is replying to a PING, ignore */
            return;
         if(!strcmp(in, "PING")){
            sprintf(tmp, "PONG %s", t);
            wrap_send(fd, tmp);
            return;
         }
         if(!strcmp(in, "PRIVMSG")){
            for(; *list && strcmp(*list, n); list++)
               ;
            if(!(*list))
               return;
            /* commands to be determined later */
         } else {
            if(*t == '.'){
               t++;
               n = slice(t, ' ');
               if(!strcmp(n, "test")){
                  sprintf("PRIVMSG %s Hello World\r\n", p);
                  wrap_send(fd, p);
               }
               if(!strcmp(n, "quit")){
                  wrap_send(fd, "QUIT Goodnight slaves!\r\n");
                  exit(0);
               }
            }
         }
         in = next;
      }
   }
}

void zero(char *p){
   memset(p, '\0', MAXLEN);
}

void wrap_send(int fd, char *buff){
   int rv;
   int len;

   rv = 0;
   len = strlen(buff);
   while(rv < len)
      rv += send(fd, buff, len - rv, 0);
}

void identify(int fd, t_setting *se){
   char tmp[MAXLEN];

   if(fd && se->pass && se->nick && se->realname){
      /* 1st: PASS yourpassword */
      sprintf(tmp, "PASS %s\r\n", se->pass);
#ifdef DEBUG
      printf("%s", tmp);
#endif
      wrap_send(fd, tmp);
      /* 2nd: NICK yournick */
      sprintf(tmp, "NICK %s\r\n", se->nick);
#ifdef DEBUG
      printf("%s", tmp);
#endif
      wrap_send(fd, tmp);
      /* 3rd: USER nick 0 * :realname */
      sprintf(tmp, "USER %s 0 * :%s\r\n", se->nick, se->realname);
#ifdef DEBUG
      printf("%s", tmp);
#endif
      wrap_send(fd, tmp);
#if 0
      /* 4th: JOIN #chan1,#chan2,#chan3 key1,key2,key3 */
      sprintf(tmp, "JOIN %s\r\n", se->_chans);
#ifdef DEBUG
      printf("%s", tmp);
#endif
      wrap_send(fd, tmp);
#endif
   } else {
      printf("Could not identify.\n");
      exit(0);
   }
}

#if 1
void join_chans(int fd, t_setting *se){
   char tmp[MAXLEN];
   
   if(fd && se->_chans){
      zero(tmp);
      sprintf(tmp, "JOIN %s\r\n", se->_chans);
      wrap_send(fd, tmp);
   }
}
#endif

int find(char *str, char ch){
   int i = -1;

   if(str && *str){
      for(i = 0; *str && *str != ch; i++, str++)
         ;
      if(!(*str))
         i = -1;
   }
   return i;
}

int count(char *str, char ch){
   int i = -1;
   int count;

   if(str && *str){
      for(count = 0, i = find(str, ch); i > 0; i = find(str + i, ch), count++)
         ;
      return count;
   }
   return i;
}

void *xmalloc(int size){
   void *p;

   if(NULL == (p = malloc(size))){
      fprintf(stderr, "Virtual memory exhausted or size <= 0.\n");
      exit(1);
   }
   return p;
}

char *slice(char *str, char ch){
   int i;

   if(*str && str){
      i = find(str, ch);
      if(i > 0)
         *(str + i + 1) = '\0';
   }
   return (str + i + 2);
}	

void clean_up(t_setting *se){
   char **p;

   if(se->pass)
      free(se->pass);
   if(se->nick)
      free(se->nick);
   if(se->realname)
      free(se->realname);
   if(se->serv)
      free(se->serv);
   if(se->port)
      free(se->port);
   if(se->_chans)
      free(se->_chans);
   if(se->_ausers)
      free(se->_ausers);
   if(se->chans && *(se->chans)){
      for(p = se->chans; *p; p++)
         ;
      for(p--; p != se->chans; p--)
         free(p);
   }
   if(se->ausers && *(se->ausers)){
      for(p = se->ausers; *p; p++)
         ;
      for(p--; p != se->ausers; p--)
         free(p);
   }
}
/* vim: set ts=3 sw=3 et: */
