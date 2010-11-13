#include "./main.h"
#define MAX_SIZE 2048

t_tweet *parse(char *file){
		  t_tweet *parsed;
		  t_stat buf;
		  FILE *stream;
		  char *s;
		  char *p;
		  char tmp[MAX_SIZE];
		  int i, j, k , t;

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
										  j = find_char(s + i, ']');
										  if((i + 1) == j){
													 /*{"results":[]*/
													 /* We're reading bat_sig2 and 
													  * there is nothing to see here */
													 parsed = NULL;
										  } else {
													 parsed = (t_tweet *)xmalloc(sizeof(t_tweet));
													 memset(tmp, '\0', MAX_SIZE);
													 p = s + i + 1;
													 p = strncpy(tmp, p, j - i);

													 /* Grab the date */
													 i = find_str(tmp, "\"created_at\":\"");
													 k = find_char(tmp + i + 1, '"');
													 parsed->date = (char *)xmalloc((k + 1) * sizeof(char));
													 memset(parsed->date, '\0', k + 1);
													 parsed->date = strncpy(parsed->date, tmp + i + 1, k);

													 /* Grab the id_str to create the URL for the user */
													 i += find_str(tmp + i, "\"id_str\":\"");
													 k = find_char(tmp + i + 1, '"');
													 t = strlen("https://twitter.com/adamcurry/statuses/");
													 p = (char *)xmalloc((t + k + 1) * sizeof(char));
													 memset(p, '\0', t + k + 1);
													 p = strcpy(p, "https://twitter.com/adamcurry/statuses/");
													 parsed->tweet_url = p;
													 p = strncpy(p + t, tmp + i + 1, k);

													 /* Now grab the actual tweet */
													 i += find_str(tmp + i, "\"text\":\"");
										  }
								}
								free(s);
								fclose(stream);
					 }
		  }

		  return parsed;
}
