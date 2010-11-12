#include "./main.h"
#define MAX_SIZE 2048

t_tweet *parse(char *file){
		  t_tweet *parsed;
		  t_stat buf;
		  FILE *stream;
		  char *s;
		  char *p;
		  int i;
		  int j;

		  if(0 == stat(file, &buf)){
					 if(NULL != (stream = fopen(file, "r"))){
								s = (char *)xmalloc(MAX_SIZE * sizeof(char));

								p = fgets(s, MAX_SIZE, stream);
								if(p != s || !p)
										  fprintf(stderr, "fgets error");
								if(s){
										  /* parse into parsed */
#ifdef DEBUG
										  printf("%i\n", find_str(s, "\"results\":["));
#endif
										  i = find_str(s, "{\"results\":[");
										  j = find_char(s+i, ']');
										  if((i + 1) == j){
													 /*{"results":[]*/
													 /* We're reading bat_sig2 and 
													  * there is nothing to see here */
													 parsed = NULL;
										  } else {
													 parsed = (t_tweet *)xmalloc(sizeof(t_tweet));
										  }
								}
								free(s);
								fclose(stream);
					 }
		  }

		  return parsed;
}
