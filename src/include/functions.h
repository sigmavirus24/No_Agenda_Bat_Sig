#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "t_tweet.h"

CURLcode my_curl_easy(FILE *, 
		size_t(*fn)(const void *, size_t, size_t, FILE *),
		char *);
CURLcode my_curl_easier(char *, char *);

t_tweet *parse_mem(char *);
void make_get(char *, char *);
int find_char(char *, char);
int rfind_char(char *, char);
int find_str(char *, char *);
void free_t_tweet(t_tweet *);
void *xmalloc(int);
GtkWidget *make_window(t_tweet *);

#endif
