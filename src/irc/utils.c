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
   char **l;
   char **vect;
   int i;
   char tmp[128];

   if(in && *in && se){
      memset(tmp, '\0', 128);
      vect = srvr_to_vect(in);
      if(!vect)
         exit(1);

      if(!strcmp(vect[0], "PING")){
         sprintf(tmp, "PONG %s\r\n", vect[1]);
#ifdef DEBUG
         printf(">>> %s", tmp);
#endif
         wrap_send(fd, tmp);
      } else if(!strcmp(vect[1], "451") || !strcmp(vect[2], "JOIN")){
         identify(fd, NULL);
         sleep(2);
         join_chans(fd, NULL);
      } else if(!strcmp(vect[1], "PRIVMSG"))
         privmsg(vect, se, fd);

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
   int j;
   int count;

   if(str && *str){
      i = find(str, ch);
      for(count = 0; i > 0; count++){
         j = find(str + i + 1, ch);
         i = (0 < j) ? (i + j + count + 1) : -1;
      }
      count = (count) ? count + 1 : 0;
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
   int i = -1;

   if(*str && str){
      i = find(str, ch);
      if(i > 0)
         *(str + i) = '\0';
   }
   return (str + i + 1);
}	

void remove_head(t_list **h){
   t_list *tmp;

   if(h && *h){
      tmp = *h;
      *h = tmp->next;
      tmp->next = NULL;
      free(tmp->name);
      free(tmp);
   }
}

void new_head(char *n, t_list **h){
   t_list *new;

   if(n){
      new = (t_list *)xmalloc(sizeof(t_list));
      new->name = strdup(n);
      new->next = *h;
      *h = new;
   }
}

void clean_up(t_setting *se){
   t_list *p;

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
   if(se->chan_h){
      for(p = se->chan_h; p;)
         remove_head(&p);
   }
   if(se->user_h){
      for(p = se->chan_h; p;)
         remove_head(&p);
   }
}

void replace_spaces(char **goog, char *s){
   char *str;
   int cnt;
   int len;
   int i;

   if(goog && *goog){
      str = *goog;
      len = strlen(str);
      cnt = count(str, ' ');
      *goog = (char *)xmalloc((len + (cnt << 1) + 1) * sizeof(char));
      memset(*goog, '\0', len + (cnt << 1) + 1);
      for( ; cnt > 1; cnt--, str += i){
         i = find(str, ' ');
         strncat(*goog, str, i);
         strcat(*goog, s);
         i++;
      }
      strcat(*goog, str);
   }
}

void sprintf_send(int fd, char *to, char *fn){
   char tmp[1024];
   memset(tmp, '\0', 1024);
   sprintf(tmp, "PRIVMSG %s %s\r\n", to, fn);
   wrap_send(fd, tmp);
}


void print_help(int fd, char *nick){
   sprintf_send(fd, nick, "===COMMANDS===");
   sprintf_send(fd, nick, ".add <nick> (PRIVELEGED)");
   sprintf_send(fd, nick, ".del <nick> (PRIVELEGED)");
   sprintf_send(fd, nick, ".donate [nick]");
   sprintf_send(fd, nick, ".google <phrase>");
   sprintf_send(fd, nick, ".help");
   sprintf_send(fd, nick, ".info");
   sleep(1);
   sprintf_send(fd, nick, ".invite <nick>*");
   sprintf_send(fd, nick, ".itm");
   sprintf_send(fd, nick, ".join <#channel> [key] (PRIVELEGED)");
   sprintf_send(fd, nick, ".list_ausers (PRIVELEGED)");
   sprintf_send(fd, nick, ".list_chans (PRIVELEGED)");
   sprintf_send(fd, nick, ".mkrc (PRIVELEGED)");
   sprintf_send(fd, nick, ".opencongress <bill>");
   sprintf_send(fd, nick, ".part (PRIVELEGED)");
   sleep(1);
   sprintf_send(fd, nick, ".quit (PRIVELEGED)");
   sprintf_send(fd, nick, ".start_signal (PRIVELEGED)");
   sprintf_send(fd, nick, ".stream [nick]");
   sprintf_send(fd, nick, ".twitter <username>");
   sprintf_send(fd, nick, ".version");
   sleep(1);
   sprintf_send(fd, nick, ".wiki <phrase>");
   sprintf_send(fd, nick, "Things in <>'s are required. Things in []'s are opti"
         "onal.");
   sprintf_send(fd, nick, "* The bot must have proper permissions to do this.");
}

void sprintf_send2(int fd, char *to, char *one, char *two){
   char tmp[1024];
   memset(tmp, '\0', 1024);
   sprintf(tmp, "PRIVMSG %s %s%s\r\n", to, one, two);
   wrap_send(fd, tmp);
}

void make_rc(t_setting *se){
   FILE *rc;
   t_list *l;

   if(NULL != (rc = fopen(se->rcfilepath, "w"))){
      fprintf(rc, "password %s\nnick %s\nrealname %s\n", se->pass, se->nick,
           se->realname);
      fprintf(rc, "server %s\nport %s\nchannels %s", se->serv, se->port,
            se->chan_h->name);
      for(l = se->chan_h->next; l; l = l->next)
         fprintf(rc, ",%s", l->name);
      fprintf(rc, "\nauth_users %s", se->user_h->name);
      for(l = se->user_h->next; l; l = l->next)
         fprintf(rc, ",%s", l->name);
      fflush(rc);
      fclose(rc);
   }
}

void privmsg(char **vect, t_setting *se, int fd){
   t_list *l;
   /* char **t; */
   char *n;
   char tmp[128];
   int i;

   for(l = se->user_h; l && strcmp(l->name, vect[0]); l = l->next)
      ;

   i = (!strcmp(vect[2], se->nick)) ? 0 : 2;

   if(vect[3] && *vect[3] == '.'){
      vect[3]++;
      n = slice(vect[3], ' ');
      if(!strcmp(vect[3], "itm")){
         if(!strcmp(vect[2], se->nick)){
            sprintf_send2(fd, vect[0], "In The Morning ", vect[0]);
         } else
            sprintf_send(fd, vect[2], "In The Morning Slaves");
      } else if(l && !strcmp(vect[3], "part")){
         sprintf(tmp, "PART %s :Parting is so sad ITM\r\n", vect[2]);
         wrap_send(fd, tmp);
         if(se->chan_h){
            if(strcmp(se->chan_h->name, vect[2])){
               for(l = se->chan_h; l->next && strcmp(l->next->name, vect[2]); l = l->next)
                  ;
               remove_head(&(l->next));
            } else 
               remove_head(&(se->chan_h));
         }
      } else if(l && !strcmp(vect[3], "join") && strcmp(n, vect[3])){
         sprintf(tmp, "JOIN %s\r\n", n);
         wrap_send(fd, tmp);
         slice(n, ' '); /* Make sure there is no password attached */
         new_head(n, &(se->chan_h));
      } else if(l && !strcmp(vect[3], "quit")){
         wrap_send(fd, "QUIT Goodbye slaves!\r\n");
         kill(se->listening_pid, SIGKILL);
         exit(0);
      } else if(l && !strcmp(vect[3], "invite") && strcmp(n, vect[3])){
         sprintf(tmp, "INVITE %s %s\r\n", n, vect[2]);
         wrap_send(fd, tmp);
      } else if(l && !strcmp(vect[3], "list_chans")){
         for(l = se->chan_h; l; l = l->next)
            sprintf_send(fd, vect[0], l->name);
      } else if(l && !strcmp(vect[3], "start_signal")){
         if(!fork()){
            char *args[] = {"/usr/bin/python", "./src/irc/bat_sig.py", 
               NULL};
            execvp(*args, args);
         }
      } else if(l && !strcmp(vect[3], "start_test")){
         if(!fork()){
            char *args[] = {"/usr/bin/python", "./src/irc/test.py", NULL};
            execvp(*args, args);
         }
      } else if(!strcmp(vect[3], "info") || !strcmp(vect[3], "version")){
         sprintf_send(fd, vect[i], "No Agenda IRC Bot Version "VERSION);
         sprintf_send(fd, vect[i], "Copyright (C) 2010 Sigmavirus24");
         sprintf_send(fd, vect[i], "https://github.com/sigmavirus24/No_Agenda"
               "_Bat_Sig/tree/stable");
         sprintf_send(fd, vect[i], "#linux-bat-signal on noagendachat.net");
      } else if(!strcmp(vect[3], "stream")){
         if(strcmp(n, vect[3])){
            sprintf_send2(fd, vect[i], n, " http://live.noagendamix.com:8000/"
                  "listen.pls");
            sprintf_send2(fd, vect[i], n, " Back-up: http://www.noagendastream"
                  ".com");
         } else {
            sprintf_send(fd, vect[i], "http://live.noagendamix.com:8000/listen"
                  ".pls");
            sprintf_send(fd, vect[i], "Back-up: http://www.noagendastream.com");
         }
      } else if(!strcmp(vect[3], "google") && strcmp(vect[3], n)){
         replace_spaces(&n, "%20");
         sprintf_send2(fd, vect[i], "http://lmgtfy.com?q=", n);
      } else if(!strcmp(vect[3], "wiki") && strcmp(vect[3], n)){
         replace_spaces(&n, "_");
         sprintf_send2(fd, vect[i], "https://secure.wikimedia.org/wikipedia/"
               "en/wiki/", n);
      } else if(!strcmp(vect[3], "opencongress") && strcmp(vect[3], n)){
         replace_spaces(&n, "%20");
         sprintf(tmp, "PRIVMSG %s http://www.opencongress.org/search/res"
               "ult?q=%s&search_bills=1\r\n", vect[i], n);
         wrap_send(fd, tmp);
      } else if(!strcmp(vect[3], "help")){
         print_help(fd, vect[0]);
      } else if(!strcmp(vect[3], "twitter") && strcmp(vect[3], n)){
         if(!count(n, ' ')){
            sprintf_send2(fd, vect[i], "https://twitter.com/", n);
         }
      } else if(!strcmp(vect[3], "donate")){
         if(strcmp(vect[3], n)){
            sprintf_send2(fd, vect[i], n, " http://dvorak.org/na/");
            sprintf_send2(fd, vect[i], n, " http://noagenda.squarespace.com/"
                  "donations/");
            sprintf_send2(fd, vect[i], n, " http://www.noagendanation.com/"
                  "donate");
         } else {
            sprintf_send(fd, vect[i], "http://dvorak.org/na/");
            sprintf_send(fd, vect[i], "http://noagenda.squaresapce.com/"
                  "donations/");
            sprintf_send(fd, vect[i], "http://www.noagendanation.com/donate");
         }
      } else if(l && !strcmp(vect[3], "add") && strcmp(vect[3], n)){
         slice(n, ' ');
         if(!count(n, ' ')){
            new_head(n, &(se->user_h));
            sprintf_send2(fd, vect[0], n, " was added to the authorized users list.");
         }
      } else if(l && !strcmp(vect[3], "del") && strcmp(vect[3], n)){
         if(!count(n, ' ')){
            if(se->user_h){
               if(strcmp(se->user_h->name, n)){
                  for(l = se->user_h; l->next && strcmp(l->next->name, n); l = l->next) 
                     ;
                  remove_head(&(l->next));
               } else {
                  remove_head(&(se->user_h));
               }
            }
         }
      } else if(l && !strcmp(vect[3], "list_ausers")){
         for(l = se->user_h; l; l = l->next)
            sprintf_send(fd, vect[0], l->name);
      } else if(l && !strcmp(vect[3], "mkrc")){
         make_rc(se);
      }
   }
}
/* vim: set ts=3 sw=3 et: */
