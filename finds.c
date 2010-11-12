#include "./main.h"

/* find_char::Returns first position of c */
int find_char(char *str, char c){
	int i;

	if(str){
		i = 0;
		while(*str){
			if(*(str++) == c)
				return i;
			i++;
		}
	}
	return -1;
}

/* rfind_char::Returns position of last instance of c */
int rfind_char(char *str, char c){
	int max;
	int i;

	if(str){
		max = i = 0;
		while(*str){
			if(*(str++) == c)
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
	int pos;
	int i;
	char *p;

	if(str && find){
		pos = 0;
		flen = strlen(find);
		slen = strlen(str);
		if(flen < slen){
			p = str + (flen - 1);
			pos += flen - 1;
			while(pos < slen){
				if(*p == *(find + flen - 1)){
					i = flen - 1;
					while(i > 0){
						if(*(p--) != *(find + i))
							break;
						i--;
					}
					if(i == 0 && *p == *find)
						return pos;
				} else {
						/* Shift pattern */
						i = rfind_char(find, *p);
						if(i < 0){
							p = str + pos + flen;
							pos += flen;
						} else {
							p = str + pos + (flen - i - 1);
							pos += (flen - i - 1);
						}
				}
			}
			if(pos <= slen)
				return pos;
		}
	}
	return -1;
}
