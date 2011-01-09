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
#ifdef DEBUG
#include <errno.h>
#endif

int dial(char *host, char *port){
   struct addrinfo hints;
   struct addrinfo *p, *res;
   int sock = -1;
   int i;

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   if(!host)
      hints.ai_flags = AI_PASSIVE; /* use my IP */

   if(0 != getaddrinfo(host, port, &hints, &res)){
      printf("Can not retrieve server information.\n");
      return -1;
   }

   for(p = res; p; p = p->ai_next){
      if(0 > (sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
         continue;

      if(host){
         if(0 > connect(sock, p->ai_addr, p->ai_addrlen)){
            close(sock);
            sock = -1;
         } else
            break;
      } else {
         i = 1;
         if(0 > setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &i, 
                  sizeof(int))){
            printf("Cannot set listening socket options.\n");
            sock = -1;
            continue;
         }
         if(0 > bind(sock, p->ai_addr, p->ai_addrlen)){
            close(sock);
            printf("Cannot bind listening socket.\n");
            continue;
         }
      }
   }
   freeaddrinfo(res);

   if(!host && sock > 0){
      if(-1 == listen(sock, 3)){
         /* Shouldn't have more than 1 trying to connect
          * at once, so three is enough.
          */
#ifdef DEBUG
         i = errno;
         switch(i){
            case EADDRINUSE: 
               printf("EADDRINUSE.\n"); 
               break;
            case EBADF:
               printf("EBADF.\n");
               break;
            case ENOTSOCK:
               printf("ENOTSOCK.\n");
               break;
            case EOPNOTSUPP:
               printf("EOPNOTSUPP.\n");
               break;
            case 0:
               printf("No error.\n");
               break;
            default:
               printf("Who knows?! %d, %d\n", i, sock);
               break;
         }
#endif
         printf("Cannot listen on local socket.\n");
         exit(1);
      }
   }
   return sock;
}

void parse_srvr(char *in, t_setting *se, int fd){
   char *n;
   char **l;
   char **vect;
   int i;
   char tmp[128];

   if(in && *in && se){
      memset(tmp, '\0', 128);
      vect = srvr_to_vect(in);
      if(!vect)
         exit(1);

      if(!strcmp(vect[0], "PONG")) /* Server is replying to a PING, ignore */
         return;
      if(!strcmp(vect[0], "PING")){
         sprintf(tmp, "PONG %s\r\n", vect[1]);
#ifdef DEBUG
         printf(">>> %s", tmp);
#endif
         wrap_send(fd, tmp);
         return;
      }
      if(!strcmp(vect[1], "451") || !strcmp(vect[2], "JOIN")){
         identify(fd, NULL);
         sleep(2);
         join_chans(fd, NULL);
      }
      if(!strcmp(vect[1], "PRIVMSG")){
         for(l = se->ausers; *l && strcmp(*l, vect[0]); l++)
            ;
         if(!(*l))
            return;

         if(vect[3] && *vect[3] == '.'){
            vect[3]++;
            n = slice(vect[3], ' ');
            if(!strcmp(n, "test")){
               if(!strcmp(vect[2], se->nick))
                  sprintf(tmp, "PRIVMSG %s Hello %s\r\n", *l, *l);
               else
                  sprintf(tmp, "PRIVMSG %s Hello Slaves\r\n", vect[2]);
               wrap_send(fd, tmp);
            } else if(!strcmp(n, "quit")){
               wrap_send(fd, "QUIT Goodbye slaves!\r\n");
               kill(se->listening_pid, SIGKILL);
               exit(0);
            } else if(!strcmp(n, "start_signal")){
               if(!fork()){
                  char *args[] = {"/usr/bin/python", "./src/irc/bat_sig.py", 
                     NULL};
                  execvp(*args, args);
               }
            } else if(!strcmp(n, "start_test")){
               if(!fork()){
                  char *args[] = {"/usr/bin/python", "./src/irc/test.py", NULL};
                  execvp(*args, args);
               }
            } else if(!strcmp(n, "help")){
               sprintf(tmp, "PRIVMSG %s ===COMMANDS===\r\n", *l);
               wrap_send(fd, tmp);
               sprintf(tmp, "PRIVMSG %s .help\r\n", *l);
               wrap_send(fd, tmp);
               sprintf(tmp, "PRIVMSG %s .start_signal\r\n",
                     *l);
               wrap_send(fd, tmp);
               sprintf(tmp, "PRIVMSG %s .test\r\n", *l);
               wrap_send(fd, tmp);
               sprintf(tmp, "PRIVMSG %s .quit\r\n", *l);
               wrap_send(fd, tmp);
            }
         }
      }
      for(i = 0, l = vect; *l; l++)
         ;
      for( ; i; i--)
         free(*(vect + i));
      free(vect);
   }
}

char **srvr_to_vect(char *srvr){
   char **tmp;
   int i, j;
   
   if(srvr && *srvr){
      if(*srvr == ':')
         srvr++;
      tmp = (char **)xmalloc(5 * sizeof(char *));
      i = find(srvr, '!');
      if(i < 0){
         i = find(srvr, ' ');
         *tmp = strndup(srvr, i);
         srvr += i + 1;
      } else {
         *tmp = strndup(srvr, i);
         srvr += find(srvr, ' ') + 1;
      }
      for(j = 1; j < 3; j++){
         i = find(srvr, ' ');
         if(i < 0)
            break;
         *(tmp + j) = strndup(srvr, i);
         srvr += i + 1;
      }
      srvr++;
      i = find(srvr, '\r');
      *(tmp + j) = strndup(srvr, i);
      for(j++ ; j < 5; j++)
         *(tmp + j) = NULL;
      return tmp;
   }
   return NULL;
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
   static t_setting *s;

   if(se)
      s = se;
   if(fd && se){
      /* 1st: PASS yourpassword */
      sprintf(tmp, "PASS %s\r\n", se->pass);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
      /* 2nd: NICK yournick */
      sprintf(tmp, "NICK %s\r\n", se->nick);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
      /* 3rd: USER nick 0 * :realname */
      sprintf(tmp, "USER %s 0 * :%s\r\n", se->nick, se->realname);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
   } else if(fd && s){
      sprintf(tmp, "PASS %s\r\n", s->pass);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
   } else {
      printf("Could not identify.\n");
      exit(0);
   }
}

void join_chans(int fd, t_setting *se){
   char tmp[MAXLEN];
   static t_setting *s;
   
   if(fd && se){
      s = se;
      zero(tmp);
      sprintf(tmp, "JOIN %s\r\n", se->_chans);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
   } else if(fd && s){
      zero(tmp);
      sprintf(tmp, "JOIN %s\r\n", s->_chans);
#ifdef DEBUG
      printf(">>> %s", tmp);
#endif
      wrap_send(fd, tmp);
   }
}

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
      i = find(str, ch);
      for(count = 0; i > 0; count++){
         i = (0 < find(str + i + 1, ch)) ? (i + find(str + i + 1, ch) + count + 1) : -1;
      }
      return count + 1;
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
   int i = -1;

   if(*str && str){
      i = find(str, ch);
      if(i > 0)
         *(str + i) = '\0';
   }
   return (str + i + 1);
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
