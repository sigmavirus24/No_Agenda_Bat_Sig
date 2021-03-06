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
#define _GNU_SOURCE

#include <getopt.h>
#include <unistd.h>
#include "./include/main.h"

#define SLEEP_SECONDS 11
#define MAX_ITER 700
#define DBPTR 0xDEADBEEF

extern int optind, opterr, optopt;
extern char *optarg;

static const struct option long_options[] = {
	{"gtk",       no_argument,       0, 0},
	{"license",   no_argument,       0, 0},
	{"version",   no_argument,       0, 'v'},
	{"help",      no_argument,       0, 'h'},
	{"browser",   required_argument, 0, 'b'},
	{"jingle",    required_argument, 0, 'j'},
	{"mp3player", required_argument, 0, 'm'},
	{"irc",	      no_argument,	 0, 0},
	{0, 0, 0, 0}
};

int parse_cmdline_opts(int argc, char *argv[], t_setting *sets);


int main(int argc, char **argv){
	GtkWidget *win;
	t_tweet *info;
	t_tweet *refreshed;
	t_setting sets;
	char *get;
	char *mem;
	char *host;
	char count;
	int sockfd;
	int rv;
	pid_t pid;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	memset(sets.set_from_rc, 0, 3);
	sets.irc = sets.gtk_on = 0;
	sets.mp3player = sets.path_to_jingle = sets.browser = get = (char *)DBPTR;

	if(argc == 1){
		/* Read from rc file */
		bat_sig_rc(&sets);
	} else {
		/* Parse arguments */
		rv = parse_cmdline_opts(argc, argv, &sets);
		if(rv < 0) {
			return 0;
		}
		else if(rv > 0) { /* technically else not needed */
			return 10; /* bad args */
		}
	}

	if(sets.path_to_jingle == (char*)DBPTR)
		sets.path_to_jingle = "jingles/douchebag.mp3";
	if(sets.browser == (char*)DBPTR)
		sets.browser = "/usr/bin/firefox";
	if(sets.mp3player == (char*)DBPTR)
	    sets.mp3player = "/usr/bin/mpg123";

   host = "Host: search.twitter.com\n\n";

	mem = (char *)xmalloc(MAX_SIZE * sizeof(char));

   sockfd = sockets_connect();

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
      get = (char *)xmalloc(MAX_SIZE * sizeof(char));
      /* Twitter sends the Connection: close header so by RFC2616 we must close our
       * connection and then reconnect later.
       */
   }

	info = parse_mem(mem);
	if(info){
		/* Now to grab the "refresh_url" string from the data and attach it to
		 * a new URL buffer and have it watch that. 
		 * For testing, I'm using the one as of Tue Nov 9
		 */
      make_get(get, info->refresh);

		for(count = 0; count < MAX_ITER; count++){
			sockfd = sockets_connect();

			if(sockfd > 0){
				memset(mem, '\0', MAX_SIZE); /* Clear out the memory so we do not get false information */

            sockets_request(sockfd, get, host, &mem, MAX_SIZE);

				/* Returns "{\"results\":[]" if there is nothing. 
				 * Can run a while loop until this does not match. */
				refreshed = parse_mem(mem);
				if(refreshed)
					break;

				(void)sleep(SLEEP_SECONDS);
			} else {
				printf("Cannot reestablish connection.\n");
				return 1;
			}
		}

		if(!refreshed)
			printf("No bat signal was sent.\n");
		else{
         if(sets.irc)
            notice(refreshed);
			/* fork() and exec Jingle */
			if(0 > (pid = fork())){
				fprintf(stderr, "Error forking.\n");
				return 1;
			}
			if(pid == 0){
				char *args[] = {sets.mp3player, sets.path_to_jingle, NULL};
				close(STDOUT_FILENO);
				close(STDERR_FILENO);
				execvp(*args, args);
				exit(EXIT_FAILURE);
			} else {
				if(!sets.gtk_on)
					printf("Time: %s\nTweetURL: %s\nTweet: %s\n", refreshed->date, refreshed->tweet_url, refreshed->text);
				else {
					rv = 0;
					gtk_init(&rv, NULL);
					win = make_window(refreshed, sets.browser);
					gtk_widget_show(win);
					gtk_main();
				}
			}
			free_t_tweet(refreshed);
		}
		free_t_tweet(info);
	}

	if(get != (char *)DBPTR)
		free(get);
	free(mem);
	if(sets.set_from_rc[0])
		free(sets.path_to_jingle); 
	if(sets.set_from_rc[1])
		free(sets.browser);
	if(sets.set_from_rc[2])
		free(sets.mp3player);
	return 0;
}

void help(void){
	printf("usage:\n       ./nabatsignal [--gtk | --browser ... | --jingle ... | \n\t\t\t--mp3player ...]\n");
	printf("\t\t [-h | --help]\n\t\t [--license]\n\t\t [-v | --version]\n");
	printf("\t--gtk enables a GTK pop-up box.\n");
   printf("\t--irc turns on sending the signal to the No Agenda IRC Bot.\n");
	printf("\t--browser followed by the absolute path to the executabe. (i.e.\n\t\t /usr/bin/firefox)\n");
	printf("\t--jingle folowed by the absolute path to the mp3. (i.e.\n\t\t ~/jingles/douchebag.mp3)\n");
	printf("\t--mp3player followed by the absolute path.\n");
	printf("\t-h and --help both print this message.\n");
	printf("\t--license prints the license information.\n");
	printf("\t-v and --version both print the version number.\n");
}

#define MINLEN(a,b) (a < b ? a : b)

int parse_cmdline_opts(int argc, char *argv[], t_setting *sets) {

	int optindex = -1;
	int c = 0, num_bad_args = 0, len = 0;

	while((c = getopt_long(argc, argv, "+vhb:j:m:",
					long_options, &optindex)) != -1) {
		switch(c) {
			case 0:
				len = strlen(long_options[optindex].name);
				if(strncmp("gtk", long_options[optindex].name,
							MINLEN(4,len)) == 0) {
					sets->gtk_on = 1;
					len = 0; /* use temporarily */
				}
				else if(strncmp("irc", long_options[optindex].name,
							MINLEN(4,len)) == 0) {
					sets->irc = 1;
				}
				else if(strncmp("license", long_options[optindex].name,
							MINLEN(8,len)) == 0) {
					printf("\"No Agenda Bat Signal\" Copyright (C) 2010\n"
							"\n"
							"This program comes with ABSOLUTELY NO WARRANTY.\n"
							"For details check LICENSE which can be found in "
							"the source, visit\n"
							"<http://www.gnu.org/licenses>. This is free "
							"software, and you are\n"
							"welcome to redistribute it under certain "
							"conditions.\n");
					return -1; /* trigger exit */
				}
				else {
					return -1; /* error */
				}
				break;

			case 'v':
				printf("nabatsignal-"VERSION" (C) 2010 SigmaVirus24, "
						"see LICENSE for details\n");
				return -1; /* trigger exit */
				break;

			case 'h':
				printf("nabatsignal-"VERSION" (C) 2010 SigmaVirus24, "
						"see LICENSE for details\n");
				help();
				return -1; /* trigger exit */
				break;

			case 'b':
				sets->browser = strdup(optarg);
				break;

			case 'j':
				sets->path_to_jingle = strdup(optarg);
				break;

			case 'm':
				sets->mp3player = strdup(optarg);
				break;

			default:
				return -1; /* error */
				break; /* never get here */
		}
		optindex = -1; /* reset the optindex */
	}

	/* see if anything else is hanging around */
	if(optind < argc) {
		fprintf(stderr, "Unknown option(s):");
		while(optind < argc) {
			fprintf(stderr, " %s", argv[optind++]);
		}
		fprintf(stderr, "\n");
		return -1;
	}

	/* FIXME: verify argument values */

	/* report bad args */
	return num_bad_args;
}
/* vim: set sw=3 ts=3 et: */
