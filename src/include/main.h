/* No Agenda Bat Signal
 * Copyright (C) 2010
 * See LICENSE file for license details.
 */
#ifndef _MAIN_H_
#define _MAIN_H_

#include <errno.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#define MAX_SIZE 2048

typedef struct stat t_stat;

typedef struct s_tweet {
	char *date; /* For ourselves */
	char *text; /* Might be nice to display for user */
	char *tweet_url; /* In case the user is a denier */
	char *refresh; /* Refresh URL starting with '?' */
} t_tweet;

CURLcode my_curl_easy(FILE *, 
		size_t(*fn)(const void *, size_t, size_t, FILE *),
		char *);
CURLcode my_curl_easier(char *, char *);

t_tweet *parse_mem(char *);
int find_char(char *, char);
int rfind_char(char *, char);
int find_str(char *, char *);
void free_t_tweet(t_tweet *);
void *xmalloc(int);
#endif
