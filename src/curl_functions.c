#include "./include/main.h"

CURLcode my_curl_easy(FILE *out, 
		size_t(*fn)(const void *, size_t, size_t, FILE *),
		char *url){
	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fn);
		ret = curl_easy_perform(curl);

		fclose(out);
		curl_easy_cleanup(curl);
	}

	return ret;
}
