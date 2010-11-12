#include "./main.h"
#define MAX_SIZE 2048

t_tweet *parse(char *file){
		  t_tweet *parsed;
		  t_stat buf;
		  FILE *f;
		  char *s;
			/*char *p;*/

		  if(0 == stat(file, &buf)){
					 if(NULL != (f = fopen(file, "r"))){
								parsed = (t_tweet *)xmalloc(sizeof(t_tweet));
								s = (char *)xmalloc(MAX_SIZE * sizeof(char));

								s = fgets(s, MAX_SIZE - 1, f);
								printf("%s\n\n", s);
								if(s)
									printf("%i\n", find_str(s, "\"results\":["));
								/* parse into parsed */
								free(s);
								fclose(f);
					 }
		  }

		  return parsed;
}
