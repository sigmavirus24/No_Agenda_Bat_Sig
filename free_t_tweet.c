#include "./main.h"

void free_t_tweet(t_tweet *t){
				if(t){
								free(t->date);
								free(t->text);
								free(t->tweet_url);
								free(t->refresh);
								free(t);
				}
}
