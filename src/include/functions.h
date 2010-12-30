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
#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "t_tweet.h"
#include "t_setting.h"

#ifdef __CURL_CURL_H
CURLcode my_curl_easy(FILE *, 
		size_t(*fn)(const void *, size_t, size_t, FILE *),
		char *);
CURLcode my_curl_easier(char *, char *);
#endif

t_tweet *parse_mem(char *);
void make_get(char *, char *);
int find_char(char *, char);
int rfind_char(char *, char);
int find_str(char *, char *);
void free_t_tweet(t_tweet *);
void *xmalloc(int);
#ifdef __GTK_H__
GtkWidget *make_window(t_tweet *, char *);
#endif
#ifdef _SYS_SOCKET_H
int sockets_connect(struct addrinfo *, char *);
#endif
void sockets_request(int, char *, char *, char **, int);
void my_close(int *);
void bat_sig_rc(t_setting *);

#endif
/* vim: set sw=8 ts=8: */
