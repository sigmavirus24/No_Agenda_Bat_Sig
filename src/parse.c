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
#include "./include/parse.h"

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
		j = find_char(s, ']');
		if((i < 0) || (i + 1) == j){
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

void make_get(char *dest, char *src){
	int i;

	if(dest && src){
		memset(dest, '\0', MAX_SIZE);
		strcpy(dest, "GET ");
		i = find_str(src, "twitter.com/");
		strcat(dest, src + i);
		strcat(dest, " HTTP/1.1\n");
	}
}
