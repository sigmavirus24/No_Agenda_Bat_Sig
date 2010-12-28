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
#define MAX_ITER 700

int main(int argc, char **argv){
	CURLcode res;
	GtkWidget *win;
	t_tweet *info;
	t_tweet *refr;
	char *def; /* Default URL */
	char *get;
	char host[100];
	char *mem;
	char *path_to_jingle;
	char *browser;
	char count;
	char gtk_on;
	char use_ssl;
	char browser_set;
	char jingle_set;
	char set_from_rc[2];
	int sockfd;
#if 0
	int r;
#endif
	struct addrinfo hints;
	pid_t pid;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	jingle_set = browser_set = gtk_on = use_ssl = 0;
	path_to_jingle = browser = get = (char *)0xDEADBEEF;

	if(argc == 1){
		/* Read from rc file */
		set_from_rc[0] = 0;
		set_from_rc[1] = 0;

		bat_sig_rc(&path_to_jingle, &browser, &gtk_on, &use_ssl);
		if(path_to_jingle)
			jingle_set = set_from_rc[0] = 1;
		if(browser)
			browser_set = set_from_rc[1] = 1;
		printf("%s\n%s\n%i\n%i", path_to_jingle, browser, (int)gtk_on, (int)use_ssl);
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
				help();
				return 0;
			}

			if(!strcmp(argv[(int)count], "--ssl"))
				use_ssl = 1;

			if(!strcmp(argv[(int)count], "--browser")){
				browser_set = 1;
				count++;
				browser = argv[(int)count];
			}

			if(!strcmp(argv[(int)count], "--jingle")){
				jingle_set = 1;
				count++;
				path_to_jingle = argv[(int)count];
			}

			if(!strcmp(argv[(int)count], "-v") ||
					!strcmp(argv[(int)count], "--help")){
				printf("nabatsignal-VERSION (C) 2010 SigmaVirus24, see LICENSE for details\n");
				return 0;
			}
		}
	}

	path_to_jingle = "jingles/douchebag.mp3";
	if(!browser_set)
		browser = "/usr/bin/firefox";

	if(use_ssl){
#ifndef SIGMANATEST
		def = "https://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1";
#else
		def = "https://search.twitter.com/search.json?from=sigmanatest&rpp=1";
#endif
	}
	else {
		def = "search.twitter.com";
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

		sockfd = sockets_connect(&hints, def);

		if(sockfd < 0){
			printf("Cannot establish connection.\n");
			return 1;
		} else {
#ifndef SIGMANATEST
			get = "GET /search.json?q=@pocketnoagenda&from=adamcurry&rpp=1 HTTP/1.1\n";
#else
			get = "GET /search.json?q=@pocketnoagenda&from=sigmanatest&rpp=1 HTTP/1.1\n";
#endif
			sockets_request(sockfd, get, host, &mem, MAX_SIZE);
			my_close(&sockfd);
			get = (char *)xmalloc(MAX_SIZE * sizeof(char));
			/* Twitter sends the Connection: close header so by RFC2616 we must close our
			 * connection and then reconnect later.
			 */
		}
	}

	info = parse_mem(mem);
	if(info){
		/* Now to grab the "refresh_url" string from the data and attach it to
		 * a new URL buffer and have it watch that. 
		 * For testing, I'm using the one as of Tue Nov 9
		 */
		if(!use_ssl)
			make_get(get, info->refresh);

		for(count = 0; count < MAX_ITER; count++){
			sockfd = sockets_connect(&hints, def);

			if(sockfd > 0){
				memset(mem, '\0', MAX_SIZE); /* Clear out the memory so we do not get false information */
				if(use_ssl)
					res = my_curl_easier(mem, info->refresh);
				else 
					sockets_request(sockfd, get, host, &mem, MAX_SIZE);
				my_close(&sockfd);

				/* Returns "{\"results\":[]" if there is nothing. 
				 * Can run a while loop until this does not match. */
				refr = parse_mem(mem);

				if(refr)
					break;

				(void)sleep(SLEEP_SECONDS);
			} else {
				printf("Cannot reestablish connection.\n");
				return 1;
			}
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
					win = make_window(refr, browser);
					gtk_widget_show(win);
					gtk_main();
				}
				(void)wait(NULL);
			}
			free_t_tweet(refr);
		}
		free_t_tweet(info);
	}

	if(get != (char *)0xDEADBEEF)
		free(get);
	free(mem);
	if(set_from_rc[0])
		free(path_to_jingle); 
	if(set_from_rc[1])
		free(browser);
	return 0;
}

void help(void){
	printf("usage:\n   ./nabatsignal [--gtk | --ssl | --browser ... | --jingle ...]\n\t\t [-h | --help]\n\t\t [--license]\n\t\t[-v | --version]\n");
	printf("\t--gtk enables a GTK pop-up box.\n");
	printf("\t--ssl uses SSL/TLS to connect to Twitter.\n");
	printf("\t--browser followed by absolute path to\n\t\tthe executabe. i.e. /usr/bin/firefox\n");
	printf("\t--jingle folowed by absolute path to\n\t\tthe mp3. i.e. ~/jingles/douchebag.mp3\n");
	printf("\t-h and --help both print this message.\n");
	printf("\t--license prints the license information.\n");
	printf("\t-v and --version both print the version number.\n");
}
