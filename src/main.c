#include "./include/main.h"

int main(int argc, char **argv){
	CURLcode res;
	FILE *out;
	t_stat buf;
	/*t_tweet *first_pass; */
	t_tweet *info;
	char *def; /* Default URL */
	char *refresh; /* Refresh URL */
	int s;

	if(argc == 1){
		/* Read from rc file */
	} else {
		/* Parse arguments */
	}

	if(0 > (s = stat("/tmp/bat_sig.txt", &buf))){
#ifdef DEBUG
		perror("stat");
#endif
		if(s == ENOENT){
			/* First running of program or old file was deleted.
			 * Either way, we need an initial pull
			 */
		} else {
			/* Something is wrong, should probably exit */
			fprintf(stderr, "Something went wrong somewhere...\n");
			/*return 1;*/
		}
	}

	if(NULL == (out = fopen("/tmp/bat_sig.txt", "w+"))){
		fprintf(stderr, "Cannot open /tmp/bat_sig.txt\n");
		return 1;
	}

	def = strdup("https://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1");

	curl_global_init(CURL_GLOBAL_SSL);

	res = my_curl_easy(out, fwrite, def);
	/* Should assume user did not invoke this to use the last copy of
	 * the bat-signal output so we need to check if this was posted today.
	 * If so, we're done!
	 */

	/* Now to grab the "refresh_url" string from the data and attach it to
	 * a new URL buffer and have it watch that. 
	 * For testing, I'm using the one as of Tue Nov 9
	 */
	if(NULL == (out = fopen("/tmp/bat_sig2.txt", "w+"))){
		fprintf(stderr, "Cannot open second file\n");
		return 1;
	}
	info = parse("/tmp/bat_sig.txt");
	if(info){
		printf("Date: %s\nTweetURL: %s\nTweet: %s\nRefresh: %s\n", info->date, info->tweet_url, info->text, info->refresh);

		refresh = strdup(info->refresh);

		res = my_curl_easy(out, fwrite, refresh);
		/* Returns "{\"results\":[]" if there is nothing. 
		 * Can run a while loop until this does not match. */

		curl_global_cleanup();

		free_t_tweet(info);
	}
	return 0;
}
