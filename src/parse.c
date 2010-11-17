/* No Agenda Bat Signal
 * Copyright (C) 2010
 * See LICENSE file for license details.
 */
#include "./include/main.h"

t_tweet *parse_mem(char *mem){
	t_tweet *parsed;
	char *s;
	char *p;
	char tmp[MAX_SIZE];
	int i, j, k, t;

	if(mem){
		s = mem;
		/* commence the parsing */
		i = find_str(s, "{\"results\":[");
		j = find_char(s + i, ']');
		if((i + 1) == j){
			/*{"results":[]*/
			/* We're reading bat_sig2 and 
			 * there is nothing to see here */
			parsed = NULL;
		} 
		else {
			parsed = (t_tweet *)xmalloc(sizeof(t_tweet));
			memset(tmp, '\0', MAX_SIZE);
			p = s + i + 1;
			p = strncpy(tmp, p, j - i);

			/* Grab the date */
			i = find_str(tmp, "\"created_at\":\"");
			k = find_char(tmp + i + 1, '"');
			parsed->date = (char *)xmalloc((k + 1) * sizeof(char));
			memset(parsed->date, '\0', k + 1);
			parsed->date = strncpy(parsed->date, tmp + i + 1, k);

			/* Grab the id_str to create the URL for the user */
			i += find_str(tmp + i, "\"id_str\":\"");
			k = find_char(tmp + i + 1, '"');
			t = strlen("https://twitter.com/adamcurry/statuses/");
			p = (char *)xmalloc((t + k + 1) * sizeof(char));
			memset(p, '\0', t + k + 1);
			p = strcpy(p, "https://twitter.com/adamcurry/statuses/");
			parsed->tweet_url = p;
			p = strncpy(p + t, tmp + i + 1, k);
			p = parsed->tweet_url;
			parsed->tweet_url = strdup(p);
			free(p);

			/* Now grab the actual tweet */
			i += find_str(tmp + i, "\"text\":\"");
			k = find_char(tmp + i + 1, '"');
			parsed->text = (char *)xmalloc((k + 1) * sizeof(char));
			memset(parsed->text, '\0', k + 1);
			parsed->text = strncpy(parsed->text, tmp + i + 1, k);

			/* Finally grab the refresh url */
			i = find_str(s + j, "\"refresh_url\":\"");
			t = find_char(s + j + i + 1, '"');
			k = strlen("https://search.twitter.com/search.json");
			p = (char *)xmalloc((k + t + 1) * sizeof(char));
			memset(p, '\0', k + t + 1);
			p = strcpy(p, "https://search.twitter.com/search.json");
			parsed->refresh = p;
			p = strncpy(p + k, s + j + i + 1, t);
			p = parsed->refresh;
			parsed->refresh = strdup(p);
			free(p);
		}
	}
	return parsed;
}
