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
#include "./include/curl_functions.h"

static size_t write_to_mem(void *p, size_t size, size_t nmemb, void *data){
	size_t size_new = 0;

	if(data && p){
		size_new = size * nmemb;
		if(size_new < MAX_SIZE)
			if(!memcpy(data, p, size_new)){
				fprintf(stderr, "memcpy error!");
				return -1;
			}
	}
	return size_new;
}

CURLcode my_curl_easy(FILE *out, 
		size_t(*fn)(const void *, size_t, size_t, FILE *),
		char *url){
	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fn);
		ret = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}

	return ret;
}

CURLcode my_curl_easier(char *mem, char *url){
	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)mem);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_mem);
		ret = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
	}

	return ret;
}
/* vim: set sw=8 ts=8: */
