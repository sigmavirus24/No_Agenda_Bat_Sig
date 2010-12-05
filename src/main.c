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
	GtkWidget *win;
	t_tweet *info;
	t_tweet *refr;
	char *def; /* Default URL */
	char host[100];
	char *mem;
	char *path_to_jingle;
	char count;
	char gtk_on;
	char use_ssl;
	int sockfd;
	int r;
	struct addrinfo hints, *ret, *p;
	pid_t pid;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	gtk_on = use_ssl = 0;

	if(argc == 1){
		/* Read from rc file */
	} else {
		/* Parse arguments */
		if(!strcmp(argv[1], "--license")){
			printf("\"No Agenda Bat Signal\" Copyright (C) 2010\n\nThis program comes with ABSOLUTELY NO WARRANTY.\nFor details check LICENSE which can be found in the source, visit\n<http://www.gnu.org/licenses>. This is free software, and you are\nwelcome to redistribute it under certain conditions.\n");
			return 0;
		}

		for(count = 1; count < argc; count++){
			if(!strcmp(argv[(int)count], "--gtk")){
				gtk_on = 1;
				gtk_init(&argc, &argv);
			}

			if(!strcmp(argv[(int)count], "-h") || 
					!strcmp(argv[(int)count], "--help")){
				printf("usage:\n      ./nabatsignal [--gtk | -h | --help | --license | --ssl]\n");
			   return 0;
			}

			if(!strcmp(argv[(int)count], "--ssl"))
				use_ssl = 1;
		}
	}


	/* Using a local directory, hopefully git will accept the mp3 file.
	path_to_jingle = (char *)xmalloc(100 * sizeof(char));
	if(NULL == strcpy(path_to_jingle, getenv("HOME"))){
		fprintf(stderr, "getenv\n");
		return 1;
	}
	if(NULL == strcat(path_to_jingle, "/NA_Jingles/douchebag.mp3")){
		fprintf(stderr, "strcat\n");
		return 1;
	}
	*/
	path_to_jingle = "jingles/douchebag.mp3";

	if(use_ssl){
#ifndef SIGMANATEST
		def = strdup("https://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1");
#else
		def = strdup("https://search.twitter.com/search.json?from=sigmanatest&rpp=1");
#endif
	}
	else {
		def = strdup("search.twitter.com");
		memset(host, '\0', 100);
		sprintf(host, "Host: %s\n\n", def);
	}


	mem = (char *)xmalloc(MAX_SIZE * sizeof(char));
	if(use_ssl)
		res = my_curl_easier(mem, def);
	else {
		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;

		/* Not the actual sockfd yet, just using it until I get the sockfd */
		if(0 != (sockfd = getaddrinfo(def, "80", &hints, &ret))){
			printf("Getaddrinfo error.\n");
			return 1;
		}

		for(p = ret; p; p = p->ai_next){
			 if(0 > (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
				 continue;
			 if(0 > connect(sockfd, p->ai_addr, p->ai_addrlen)){
				 close(sockfd);
				 sockfd = -1;
			 } else
				 break;
		}

		if(sockfd < 0){
				  printf("Cannot establish connection.\n");
				  return 1;
		} else {
#ifndef SIGMANATEST
			def = "GET /search.json?q=@pocketnoagenda&from=adamcurry&rpp=1 HTTP/1.1\n";
#else
			def = "GET /search.json?q=@pocketnoagenda&from=sigmanatest&rpp=1 HTTP/1.1\n";
#endif
			write(sockfd, def, strlen(def));
			write(sockfd, host, strlen(host));

			if(0 > (r = read(sockfd, mem, MAX_SIZE - 1))){
				printf("Nothing to see here!\n");
				free(mem);
				close(sockfd);
				return 1;
			}
			*(mem + r) = '\0';
			def = (char *)xmalloc(MAX_SIZE * sizeof(char));
		}
	}

	info = parse_mem(mem);
	if(info){
		/* Now to grab the "refresh_url" string from the data and attach it to
		 * a new URL buffer and have it watch that. 
		 * For testing, I'm using the one as of Tue Nov 9
		 */
		make_get(def, info->refresh);

		for(count = 0; count < MAX_ITER; count++){
		   memset(mem, '\0', MAX_SIZE);
			if(use_ssl)
				res = my_curl_easier(mem, info->refresh);
			else {
				write(sockfd, def, strlen(def));
				write(sockfd, host, strlen(host));

				if(0 > (r = read(sockfd, mem, MAX_SIZE - 1))){
					printf("Something bizarre happened\n");
					free(def);
					free(mem);
					close(sockfd);
					return 1;
				}
			}
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
			/* fork() and exec Jingle */
			if(0 > (pid = fork())){
				fprintf(stderr, "Error forking.\n");
				return 1;
			}
			if(pid == 0){
				char *args[] = {"/usr/bin/mpg123", path_to_jingle, NULL};
				execvp(*args, args);
				exit(EXIT_FAILURE);
			} else {
				if(!gtk_on)
					printf("Time: %s\nTweetURL: %s\nTweet: %s\n", refr->date, refr->tweet_url, refr->text);
				else {
					win = make_window(refr);
					gtk_widget_show(win);
					gtk_main();
				}
				(void)wait(NULL);
			}
			free_t_tweet(refr);
		}
		free_t_tweet(info);
	}

	close(sockfd);
	free(def);
	free(mem);
/*	free(path_to_jingle); */
	return 0;
}
