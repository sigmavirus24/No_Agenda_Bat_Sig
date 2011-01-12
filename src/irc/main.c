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
   FILE *server;
   int fd;
   int localfd;
   int pid;
   char rc[128];
   char recvd[4096];
   t_setting se;

   if(argc > 1){
      printf("No Agenda IRC Bot Version "VERSION": no arguments allowed. See ~/.nabotrc.\n");
      return 0;
   }
   setbuf(stdout, NULL);
   setbuf(stderr, NULL);

   memset(rc, '\0', 128);
   if(NULL == strcpy(rc, getenv("HOME"))){
      printf("No Agenda IRC Bot Version "VERSION": unable to get $HOME.\n");
      return 0;
   }

   strcat(rc, "/.nabotrc");
   se.rcfilepath = rc;
   read_rc(rc, &se);

#ifdef DEBUG
   printf("SETTINGS %s %s %s %s %s %s %s\n", se.pass, se.nick, se.realname, se.serv, se.port, se._chans, se._ausers);
#endif

   fd = dial(se.serv, se.port);
   if(fd > 0){
      if(SIG_ERR == signal(SIGPIPE, borked_pipe)){
         printf("Cannot set up signal handler.\n");
         close(fd);
         clean_up(&se);
         return 0;
      }

      if(NULL == (server = fdopen(fd, "r+"))){
         printf("Cannot make file pointer.\n");
         clean_up(&se);
         close(fd);
         return 0;
      }

      memset(recvd, '\0', MAXLEN << 1);
      for(pid = 0; pid < 2 && fgets(recvd, MAXLEN << 1, server); pid++){
#ifdef DEBUG
         printf("<<< %s", recvd);
#endif
         parse_srvr(recvd, &se, fileno(server));
         memset(recvd, '\0', MAXLEN << 1);
      }
   } else {
      printf("No Agenda IRC Bot Version "VERSION": not able to connect to %s at %s.\n", se.serv, se.port);
      clean_up(&se);
      return 0;
   }

   identify(fileno(server), &se);
   sleep(1);
   join_chans(fileno(server), &se);

   if(0 > (pid = fork())){
      printf("No Agenda IRC Bot Version "VERSION": unable to background.\n");
      fclose(server);
      clean_up(&se);
      return 0;
   }

   if(0 == pid){
      int bsfd;
      socklen_t saddrlen;
      t_list *p;
      char tmp[1028];
      struct sockaddr_in batsig;

      sleep(2);
      localfd = dial(NULL, "33333");

      saddrlen = sizeof(struct sockaddr_in);

      while(1){
         if(0 > (bsfd = accept(localfd, (struct sockaddr*)&batsig,
                     &saddrlen))){
            printf("Error accepting batsignal connection.\n");
            clean_up(&se);
            return -1;
         }

         if(0 > (pid = recv(bsfd, recvd, MAXLEN << 1, 0))){
            printf("Error receiving batsignal.\n");
         }
         *(recvd + pid) = '\0';

         memset(tmp, '\0', 1028);
         for(p = se.chan_h ; p; p = p->next){
            if(strcmp(p->name, "#NoAgenda"))
               sprintf(tmp, "NOTICE %s :%s\r\n", p->name, recvd);
            else {
               sprintf(tmp, "PRIVMSG %s Batsignal RECEIVED! No Agenda is going"
                    " live! (%s)\r\n", p->name, recvd);
            }
            wrap_send(fileno(server), tmp);
            memset(tmp, '\0', 1028);
         }
         close(bsfd);
      }
   } else {
      se.listening_pid = pid;
      if(!fork()){/* Handles accept()'s from bat signal */
         /* Done with pid, can use now for recv()'s and send()'s */
         while(fgets(recvd, MAXLEN << 1, server)){
#ifdef DEBUG
            printf("<<< %s", recvd);
#endif
            parse_srvr(recvd, &se, fd);
            memset(recvd, '\0', MAXLEN << 1);
         }
      }
   }
   return 0;
}

void borked_pipe(int signo){
   printf("Connection with network has broken.");
   exit(0);
}

t_list *parse(char *str, char ch){
   int cnt, i;
   char *p;
   t_list *ptr, *t;

   i = count(str, ch);
   if(i > 0){
      ptr = (t_list *)xmalloc(sizeof(t_list));
      t = ptr;
      p = str;
      for(cnt = i = 0; 0 < (i = find(p, ',')); p += i + 1, cnt++){
         t->name = strndup(p, i);
         t->next = (t_list *)xmalloc(sizeof(t_list));
         t = t->next;
      }
      t->name = strdup(p);
      t->next = NULL;
   } else {
      ptr = (t_list *)xmalloc(sizeof(t_list));
      ptr->name = strdup(str);
      ptr->next = NULL;
   }
   return ptr;
}

void read_rc(char *file, t_setting *se){
   FILE *rc;
   char in[1024];
   int i;

   if(NULL != (rc = fopen(file, "r"))){
      for(memset(in, '\0', 1024); fgets(in, 1024, rc); memset(in, '\0', 1024)){
         i = strlen(in) - 1;
         *(in + i) = '\0';
         i = find(in, ' ');
         if(!strncmp(in, "password", i))
            se->pass = strdup(in + i + 1);
         if(!strncmp(in, "nick", i))
            se->nick = strdup(in + i + 1);
         if(!strncmp(in, "realname", i))
            se->realname = strdup(in + i + 1);
         if(!strncmp(in, "server", i))
            se->serv = strdup(in + i + 1);
         if(!strncmp(in, "port", i))
            se->port = strdup(in + i + 1);
         if(!strncmp(in, "channels", i))
            se->_chans = strdup(in + i + 1);
         if(!strncmp(in, "auth_users", i))
            se->_ausers = strdup(in + i + 1);
         if(!strncmp(in, "nogreet_chans", i))
            se->_nogreet = strdup(in + i + 1);
      }
      fclose(rc);

      /* Parse out channels and authorized users */
      se->chan_h = parse(se->_chans, ',');
      se->num_chans = count(se->_chans, ',');
      se->user_h = parse(se->_ausers, ',');
      se->nogreet_h = parse(se->_nogreet, ',');
   }
}
/* vim: set ts=3 sw=3 et: */
