#include "./main.h"
#define MAX_SIZE 4098

int main(int argc, char **argv){
		  int sockfd;
		  /* int i; */
		  int r;
		  struct addrinfo hints, *ret, *p;
		  char *t_url;
		  /* char *refresh; */
		  char *buffer;
		  char *t_portno = "80";

		  if(argc == 1) {
				/* Read from rc file */
		  } else {
				/* Parse arguments */
		  }

		  memset(&hints, 0, sizeof(struct addrinfo));
		  hints.ai_family = AF_UNSPEC;
		  hints.ai_socktype = SOCK_STREAM;

		  t_url = strdup("http://search.twitter.com/search.json?q=%23@pocketnoagenda&from=adamcurry&rpp=1");

		  if(0 != getaddrinfo(t_url, t_portno, &hints, &ret)){
				  printf("Invalid URL.\n");
				  return 1;
		  }

		  /* ret will likely have more than one choice (linked-list) 
		   * so find one with a good socket and connect*/
		  for(p = ret; p->ai_next; p = p->ai_next){
				 if(0 > (sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
						 continue;
		  		 if(0 > connect(sockfd, p->ai_addr, p->ai_addrlen)){
						 close(sockfd);
						 sockfd = -1;
				 }
		  }

		  /* once you have a connection (if you have it), read */
		  if(sockfd < 0){
				  printf("Cannot establish connection.\n");
				  return 1;
		  } else {
				  if(NULL == (buffer = (char*)malloc(MAX_SIZE * sizeof(char)))){
						  printf("Somehow you're out of virtual memory.");
						  return 1;
				  }

				  if(0 > (r = read(sockfd, buffer, MAX_SIZE - 1))){
						  printf("Nothing to see here!\n");
						  free(buffer);
						  close(sockfd);
						  return 1;
				  }
				  *(buffer + r) = '\0';
				  printf("%s", buffer);
		  }

		  freeaddrinfo(ret);
		  return 0;
}
