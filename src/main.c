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

int main(int argc, char **argv){
	CURLcode res;
	t_tweet *info;
	t_tweet *refr;
	char *def; /* Default URL */
	char *mem;

	if(argc == 1){
		/* Read from rc file */
	} else {
		/* Parse arguments */
		if(!strcmp(argv[1], "--license")){
			printf("\"No Agenda Bat Signal\" Copyright (C) 2010\nThis program comes with ABSOLUTELY NO WARRANTY; for details visit\n<http://www.gnu.org/licenses>. This is free software, and you are\nwelcome to redistribute it under certain conditions.\n");
			return 0;
		}
	}

	/*
	   if(0 > (s = stat("/tmp/bat_sig.txt", &buf))){
#ifdef DEBUG
perror("stat");
#endif
if(s == ENOENT){
*/
/* First running of program or old file was deleted.
 * Either way, we need an initial pull
 */
/*
   } else {
   */
/* Something is wrong, should probably exit */
/*fprintf(stderr, "Something went wrong somewhere...\n");*/
/*return 1;*/
/*}
  }

  if(NULL == (out = fopen("/tmp/bat_sig.txt", "w+"))){
  fprintf(stderr, "Cannot open /tmp/bat_sig.txt\n");
  return 1;
  }*/

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

		while(1){
			res = my_curl_easier(mem, info->refresh);
			/* Returns "{\"results\":[]" if there is nothing. 
			 * Can run a while loop until this does not match. */

			refr = parse_mem(mem);

			if(refr)
				break;

			(void)sleep(SLEEP_SECONDS);
		}

		printf("Time: %s\nTweetURL: %s\nTweet: %s\nRefresh: %s\n", refr->date, refr->tweet_url, refr->text, refr->refresh);

		/* fork() and exec Jingle */

		free_t_tweet(info);
		free_t_tweet(refr);
		/* free(refresh); */
	}

	free(def);
	free(mem);
	return 0;
}
