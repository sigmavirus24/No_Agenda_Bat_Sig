#include "./main.h"

void *xmalloc(int n){
	void *p;

	p = malloc(n);
	if(p == 0){
		fprintf(stderr, "Virtual memory exhausted.\n");
		exit(1);
	}
	return p;
}
