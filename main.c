#include "./main.h"

int main(int argc, char **argv){
		  int sockfd;
		  struct addrinfo hints, *ret;
		  char *t_url = "http://search.twitter.com/search.atom?q=%23@pocketnoagenda&from=adamcurry&rpp=1";
		  char *t_portno = "80";

		  if(argc == 1) {
					 /* Read from rc file */
		  }
		  else {
					 /* Parse arguments */
		  }

		  memset(&hints, 0, sizeof(struct addrinfo));
		  hints.ai_family = AF_UNSPEC;
		  hints.ai_socktype = SOCK_STREAM;

		  getaddrinfo(t_url, t_portno, &hints, &ret);

		  /* ret will likely have more than one choice (linked-list) */

		  /* find one with a good socket, connect and read */
}
