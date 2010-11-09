#include "./main.h"
#define MAX_SIZE 4098

int main(int argc, char **argv){
	CURL *curl;
	CURLcode res;
	FILE *out;
	t_stat buf;

	if(argc == 1) {
		/* Read from rc file */
	} else {
		/* Parse arguments */
	}

	if(NULL == (out = fopen("/tmp/bat_sig.txt", "w+"))) {
		fprintf(stderr, "Cannot open /tmp/bat_sig.txt\n");
		return 1;
	}

	curl_global_init(CURL_GLOBAL_SSL);

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
		res = curl_easy_perform(curl);

		if(res != 0)
			printf("Error!\n");

		curl_easy_cleanup(curl);
	}

	if(0 > stat("/tmp/bat_sig.txt", &buf))
#ifdef DEBUG
		perror("stat");
#endif

	/*
	fprintf(stdout, "%i = ", (int)buf.st_ctime);
	printf("%i\n", (int)time(NULL));
	*/
	/* Now to grab the "refresh_url" string from the data and attach it to
	 * a new URL buffer and have it watch that. 
	 * For testing, I'm using the one as of Tue Nov 9*/
	if(NULL == (out = fopen("/tmp/bat_sig2.txt", "w+"))) {
		fprintf(stderr, "Cannot open second file\n");
		return 1;
	}

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://search.twitter.com/search.json?since_id=2072899217264640&q=%23%40pocketnoagenda+from%3Aadamcurry");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
		res = curl_easy_perform(curl);

		if(res != 0)
			printf("Error!\n");

		curl_easy_cleanup(curl);
	}
	/* Returns "{\"results\":[]" if there is nothing. 
	 * Can run a while loop until this does not match. */

	curl_global_cleanup();

	return 0;
}
