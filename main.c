#include "./main.h"
#define MAX_SIZE 4098

int main(int argc, char **argv){
	CURL *curl;
	CURLcode res;
	FILE *out;

	if(argc == 1) {
		/* Read from rc file */
	} else {
		/* Parse arguments */
	}

	if(NULL == (out = fopen("/tmp/bat_sig.txt", "w+"))){
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
	/* putchar('\n'); */

	curl_global_cleanup();

	return 0;
}
