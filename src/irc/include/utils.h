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
#ifndef __UTILS_H__
#define __UTILS_H__
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void exit(int);
int close(int);

#define MAXLEN 2048
#define CRLF "\r\n"

#include "t_setting.h"
int dial(char *, char *);
int find(char *, char);
int count(char *, char);
char *slice(char *, char);
char **srvr_to_vect(char *);
void parse_srvr(char *, t_setting *, int);
void identify(int, t_setting *);
void join_chans(int, t_setting *);
void wrap_send(int, char *);
void *xmalloc(int);
void clean_up(t_setting *);
void replace_spaces(char **, char *);
void print_help(int, char *);

#endif
/* vim: set ts=3 sw=3 et: */
