/* No Agenda Bat Signal
 * Copyright (C) 2010
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of this License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * See LICENSE file for license details.
 */

#include "include/finds.h"

/* find_char::Returns first position of c */
int find_char(char *str, char c){
	int i;

	if(str){
      for(i = 0; *str && *str != c; str++, i++)
         ;
      if(!(*str))
         i = -1;
	}
	return i;
}

/* rfind_char::Returns position of last instance of c */
int rfind_char(char *str, char c){
	int max;
	int i;

	if(str){
      for(max = -1, i = 0; *str; str++, i++){
			if(*str == c)
				max = (i > max) ? i : max;
		}
	}
	return max;
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
#if 0
               for(i = flen - 1; i > 0 && *p != *(find + i);
                     p--, i--)
                  ;
#endif
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
#if 0
               /* No longer needed */
					if(i < 0){
						p = str + pos + flen;
						pos += flen;
					} else {
						p = str + pos + (flen - i - 1);
						pos += (flen - i - 1);
					}
#endif
               pos += (i < 0) ? flen : flen - i - 1;
               p = str + pos;
				}
			}
			if(pos <= slen)
				return pos;
		}
	}
	return -1;
}
/* vim: set sw=3 ts=3 et: */
