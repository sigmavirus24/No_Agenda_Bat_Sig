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

t_tweet *parse(char *);
int find_char(char *, char);
int rfind_char(char *, char);
int find_str(char *, char *);
void *xmalloc(int);
#endif
