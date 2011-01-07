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
      for(pid = 0; pid < 2; pid++){
         fgets(recvd, MAXLEN << 1, server);
#ifdef DEBUG
         printf("<<< %s", recvd);
#endif
         parse_srvr(recvd, &se, fd);
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
#if 0
   if(0 > (pid = fork())){
      printf("No Agenda IRC Bot Version "VERSION": unable to background.\n");
      close(fd);
      clean_up(&se);
      return 0;
   }
   if(0 == pid){
#endif
      /* Done with pid, can use now for recv()'s and send()'s */
      memset(recvd, '\0', MAXLEN << 1);
      while(fgets(recvd, MAXLEN << 1, server)){
#ifdef DEBUG
         printf("<<< %s", recvd);
#endif
         parse_srvr(recvd, &se, fd);
         memset(recvd, '\0', MAXLEN << 1);
      }
#if 0
   }
#endif
   return 0;
}

void borked_pipe(int signo){
   printf("Connection with network has broken.");
   exit(0);
}

char **parse(char *str, char ch){
   int cnt, i;
   char **tmp;
   char *p;

   i = count(str, ch);
   if(i > 0){
      tmp = (char **)xmalloc((i + 1) * sizeof(char *));
      p = str;
      for(cnt = i = 0; 0 < (i = find(p, ',')); p += i + 1, cnt++)
         *(tmp + cnt) = strndup(p, i);
      *(tmp + cnt++) = strdup(p);
      *(tmp + cnt) = NULL;
   } else {
      tmp = (char **)xmalloc(2 * sizeof(char *));
      *tmp = strdup(str);
      *(tmp + 1) = NULL;
   }
   return tmp;
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
      }
      fclose(rc);

      /* Parse out channels and authorized users */
      se->chans = parse(se->_chans, ',');
      se->ausers = parse(se->_ausers, ',');
   }
}
/* vim: set ts=3 sw=3 et: */
