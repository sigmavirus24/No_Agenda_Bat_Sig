#include "./main.h"

int find_char(char *str, char c);

/* find_str::Utilizes Boyer-Moore Fast String Search Algorithm:
 * http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/index.html
 */
int find_str(char *str, char *find){
	int flen;
	int slen;
	int len;
	int i;
	char *p;

	if(str && find){
		len = 0;
		flen = strlen(find);
		slen = strlen(str);
		if(slen < flen)
			return NULL;
		while(len < slen){
			p = str + flen;
			len += flen;
			if(*p == *(find + flen)){
				i = flen;
				while(--i > 0)
					if(*(find + i) != *(str + i))
						break;
				if(i == 0 && *find == *str)
					return find;
				else
					/* Shift pattern */
			}
		}
	}
