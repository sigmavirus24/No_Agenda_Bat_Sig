#ifndef __T_TWEET_H__
#define __T_TWEET_H__

typedef struct s_tweet {
	char *date; /* For ourselves */
	char *text; /* Might be nice to display for user */
	char *tweet_url; /* In case the user is a denier */
	char *refresh; /* Refresh URL starting with '?' */
} t_tweet;

#endif
