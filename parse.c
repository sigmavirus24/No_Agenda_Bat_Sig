#include "./main.h"
#define MAX_SIZE 2048

t_tweet *parse(char *file){
		  t_tweet *parsed;
		  t_stat buf;
		  FILE *f;
		  char *s, *p;

		  if(0 == stat(file, &buf)){
					 if(NULL != (f = fopen(file, "r"))){
								s = (char *)xmalloc(MAX_SIZE * sizeof(char));
								parsed = (t_tweet *)xmalloc(sizeof(t_tweet));

								(void)fgets(s, MAX_SIZE - 1, f);
								/* parse into parsed */
								free(s);
					 }
		  }

		  return parsed;
}
