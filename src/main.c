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
#define SLEEP_SECONDS 11
#define MAX_ITER 350

int main(int argc, char **argv){
	CURLcode res;
	t_tweet *info;
	t_tweet *refr;
	char *def; /* Default URL */
	char *mem;
	char *path_to_jingle;
	char count;
	pid_t pid;

	if(argc == 1){
		/* Read from rc file */
	} else {
		/* Parse arguments */
		if(!strcmp(argv[1], "--license")){
			printf("\"No Agenda Bat Signal\" Copyright (C) 2010\n\nThis program comes with ABSOLUTELY NO WARRANTY.\nFor details check LICENSE which can be found in the source, visit\n<http://www.gnu.org/licenses>. This is free software, and you are\nwelcome to redistribute it under certain conditions.\n");
			return 0;
		}
	}

	path_to_jingle = (char *)xmalloc(100 * sizeof(char));
	if(NULL == strcpy(path_to_jingle, getenv("HOME"))){
		fprintf(stderr, "getenv\n");
		return 1;
	}
	if(NULL == strcat(path_to_jingle, "/NA_Jingles/douchebag.mp3")){
		fprintf(stderr, "strcat\n");
		return 1;
	}

	def = strdup("https://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1");


	/* Should assume user did not invoke this to use the last copy of
	 * the bat-signal output so we need to check if this was posted today.
	 * If so, we're done!
	 */
	mem = (char *)xmalloc(MAX_SIZE * sizeof(char));
	res = my_curl_easier(mem, def);

	info = parse_mem(mem);
	if(info){
		/* Now to grab the "refresh_url" string from the data and attach it to
		 * a new URL buffer and have it watch that. 
		 * For testing, I'm using the one as of Tue Nov 9
		 */
		/* refresh = strdup(info->refresh); */

		for(count = 0; count < MAX_ITER; count++){
			res = my_curl_easier(mem, info->refresh);
			/* Returns "{\"results\":[]" if there is nothing. 
			 * Can run a while loop until this does not match. */

			refr = parse_mem(mem);

			if(refr)
				break;

			(void)sleep(SLEEP_SECONDS);
		}

		if(!refr)
			printf("No bat signal was sent.\n");
		else{
			printf("Time: %s\nTweetURL: %s\nTweet: %s\n", refr->date, refr->tweet_url, refr->text);

			/* fork() and exec Jingle */
			if(0 > (pid = fork())){
				fprintf(stderr, "Error forking.\n");
				return 1;
			}
			if(pid == 0){
				char *args[] = {"/usr/bin/mpg123", path_to_jingle, NULL};
				execvp(*args, args);
			} else 
				(void)wait(NULL);
			
			free_t_tweet(refr);
		}

		free_t_tweet(info);
	}

	free(def);
	free(mem);
	free(path_to_jingle);
	return 0;
}
