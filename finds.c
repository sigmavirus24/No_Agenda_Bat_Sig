#include "./main.h"

/* find_char::Returns position of last instance of c */
int find_char(char *str, char c){
	int max;
	int i;

	if(str){
		max = i = 0;
		while(*str){
			if(*str == c)
				max = (i > max) ? i : max;
			i++;
		}
		return max;
	}
	return -1;
}

/* find_str::Utilizes Boyer-Moore Fast String Search Algorithm:
 * http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/index.html
 * Returns index of the last character of pattern you are searching for in 
 *  the string.
 * str::string searching in
 * find::pattern trying to find.
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
			return -1;
		p = str + flen - 1;
		len += flen;
		while(len < slen){
			if(*p == *(find + flen - 1)){
				i = flen - 1;
				while(--i > 0)
					if(*(--p) != *(find + i))
						break;
				if(i == 0 && *p == *find)
					return len + 1;
				else {
					/* Shift pattern */
					i = find_char(find, *p);
					if(i < 0){
						p = str + len + flen;
						len += flen;
					} else {
						p = str + len + i;
						len += i;
					}
				}
			} else {
				p += flen;
				len += flen;
			}
		}
		if(len == slen)
			return len;
	}
	return -1;
}
