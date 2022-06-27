#include "str.h"
#include "assert.h"
#include "fmt.h"
#include "mem.h"

#include <limits.h>
#include <string.h>

#include <stdio.h>

// position
// 1   2   3   4   5
//   w   o   r   d
// -4  -3  -2  -1  0


// index
// 0  1  2  3
// w  o  r  d


// convert position to index
#define idx(i, len) ((i)<=0 ? (i) + (len) : (i)-1)

// convert position to index
#define convert(s, i, j)	do {  \
		int len;                  \
		assert(s);                \
		len = strlen(s);          \
		i = idx(i, len);          \
		j = idx(j, len);          \
		if(i > j){                \
			int t = i;            \
			i = j;                \
			j = t;                \
		}                         \
		assert(i >= 0 && j <= len);\
	} while(0)
		

char* Str_sub(const char *s, int i, int j){
	convert(s, i, j);
	char *p, *str;
	p = str = ALLOC(j-i+1);
	while(i < j)
		*p++ = s[i++];
	*p = '\0';
	return str;
}

char* Str_dup(const char *s, int i, int j, int n){
	convert(s, i, j);
	char *p, *str;
	int k;
	p = str = ALLOC(n*(j-i)+1);
	if(j - i > 0){		
		while(n-->0){
			k = i;
			while(k < j){
				*p++ = s[k++];
			}
		}
	}
	*p = '\0';
	return str;
}

char* Str_cat(const char *s1, int i1, int j1,
const char *s2, int i2, int j2){
	convert(s1, i1, j1);
	convert(s2, i2, j2);
	char *p, *str;
	p = str = ALLOC(j1-i1+j2-i2+1);
	if(j1 > i1){
		for(; i1 < j1; i1++){
			*p++ = s1[i1];
		}
	}
	if(j2 > i2){
		for(; i2 < j2; i2++){
			*p++ = s2[i2];
		}
	}
	*p = '\0';
	return str;
}

char* Str_catv(const char* s, ...){
	va_list ap;
	int i, j;
	char *p, *str;
	const char *save = s;
	int len = 1;
	
	va_start(ap, s);
	do{
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		convert(s, i, j);
		len += j - i;
	}while((s = va_arg(ap, const char*)));
	va_end(ap);
	
	p = str = ALLOC(len);
	
	s = save;
	va_start(ap, s);
	do{
		i = va_arg(ap, int);
		j = va_arg(ap, int);
		convert(s, i, j);
		while(i < j){
			*p++ = s[i++];
		}
	}while((s = va_arg(ap, const char*)));
	va_end(ap);
	
	*p = '\0';
	
	return str;
}

char* Str_reverse(const char* s, int i, int j){
	convert(s, i, j);
	char *p, *str;
	p = str = ALLOC(j-i+1);
	while(i < j){
		*p++ = s[--j];
	}
	*p = '\0';
	return str;
}

char* Str_map(const char *s, int i, int j, const char *from, const char *to){
	convert(s, i, j);
	char map[256] = {0};
	if(from && to){
		for(unsigned c = 0; c < sizeof map; c++){
			map[c] = c;
		}
		while(*from && *to){
			map[(unsigned char)*from++] = *to++;
		}
		assert(*from == 0 && *to == 0);
	}
	else{
		assert(from == NULL && to == NULL && s == NULL);
		assert(map['a']);	// checked runtime error
	}
	
	if(s){		
		char *p, *str;
		p = str = ALLOC(j - i + 1);
		while(i < j){
			*p++ = map[s[i++]];
		}
		*p = '\0';
		return str;
	}
	else{
		return NULL;
	}
}

int Str_pos(const char *s, int i){
	assert(s);
	int len = strlen(s);
	i = idx(i, len);
	assert(i>=0 && i <= len);
	return i+1;
}

int Str_len(const char *s, int i, int j){
	convert(s, i, j);
	return j-i;
}

int Str_cmp(const char *s1,int i1, int j1,
const char *s2, int i2, int j2){
	convert(s1, i1, j1);
	convert(s2, i2, j2);
	s1 = s1 + i1;
	s2 = s2 + i2;
	int cond;
	if(j1 - i1 < j2 - i2){
		cond = strncmp(s1, s2, j1-i1);
		return cond == 0 ? -1 : cond;
	} else if(j1 - i1 > j2 - i2){
		cond = strncmp(s1, s2, j2 - i2);
		return cond == 0 ? 1 : cond;
	} else{
		return strncmp(s1, s2, j1-i1);
	}
}

int Str_chr(const char *s, int i, int j, int c){
	convert(s, i, j);
	for(; i < j; i++){
		if (s[i] == c){
			return i+1;
		}
	}
	return 0;
}

int Str_rchr(const char* s, int i, int j, int c){
	convert(s, i, j);
	while(i < j){
		if(s[--j] == c){
			return j+1;
		}
	}
	return 0;
}

int Str_upto(const char *s, int i, int j, const char *set){
	convert(s, i, j);
	assert(set);
	char map[256];
	memset(map, '\0', sizeof map);
	while(*set){
		map[*set++]++;
	}
	assert(*set == '\0');
	
	for(; i < j; i++){
		if(map[s[i]]){
			return i + 1;
		}
	}
	return 0;
}

int Str_rupto(const char *s, int i, int j, const char *set){
	convert(s, i, j);
	assert(set);
	char map[256];
	memset(map, '\0', sizeof map);
	while(*set){
		map[*set++]++;
	}
	assert(*set == '\0');
	
	for(j--; i <= j; j--){
		if(map[s[j]]){
			return j + 1;
		}
	}
	return 0;
}

int Str_find(const char *s, int i, int j, const char *str){
	convert(s, i, j);
	assert(str);
	int len = strlen(str);
	if (len == 0){
		return i+1;
	} else if(len == 1){
		for( ; i < j; i++){
			if(s[i] == *str){
				return i+1;
			}
		}
	} else{
		for(; i+len <= j; i++){
			if(strncmp(s+i, str, len) == 0){
				return i+1;
			}
		}
	}
	return 0;
}

int Str_rfind(const char *s, int i, int j, const char *str){
	convert(s, i, j);
	assert(str);
	int len = strlen(str);
	if(len == 0){
		return j+1;
	} else if(len == 1){
		for(j--; i <= j; j--){
			if(s[j] == *str){
				return j + 1;
			}
		}
	} else{
		for( ; i <= j-len; j--){
			if(strncmp(s+(j-len), str, len) == 0){
				return j-len+1;
			}
		}
	}
	return 0;
}

// Step over if the first char is in set
int Str_any(const char *s, int i, const char *set){
	assert(s && set);
	int len = strlen(s);
	i = idx(i, len);
	assert(i >= 0 && i < len);
	if(i < len && strchr(set, s[i])){
		return i + 2;
	}
	return 0;
}

int Str_many(const char *s, int i, int j, const char *set){
	assert(s && set);
	convert(s, i, j);
	if(i < j && strchr(set, s[i])){		
		for(; i < j; i++){
			if(!strchr(set, s[i])){
				return i + 1;
			}
		}
	}
	return 0;
}

int Str_rmany(const char *s, int i, int j, const char *set){
	assert(s && set);
	convert(s, i, j);
	if(i < j && strchr(set, s[j-1])){
		for(j--; i <= j; j--){
			if (!strchr(set, s[j])){
				return j + 2;
			}
		}
	}
	return 0;
}

int Str_match(const char *s, int i, int j, const char *str){
	assert(s && str);
	convert(s, i, j);
	int len = strlen(str);
	if(len == 0){
		return i + 1;
	} else if(len == 1){
		if(i < j && s[i] == *str){
			return i + 2;
		}
	} else if(i <= j-len && strncmp(s+i, str, len) == 0){
		return i + len + 1;
	}
	return 0;
}

int Str_rmatch(const char *s, int i, int j, const char *str){
	assert(s && str);
	convert(s, i, j);
	int len = strlen(str);
	if (len == 0){
		return j + 1;
	} else if(len == 1){
		if(s[j-1] == *str){
			return j;
		}
	} else if(i <= j-len && strncmp(s+(j-len), str, len) == 0){
		return j - len + 1;
	}
	return 0;
}

void Str_fmt(int code, va_list *app,
int put(int c, void *cl), void *cl,
unsigned char flags[], int width, int precision){
	assert(app && flags);
	char *str = va_arg(*app, char*);
	int i = va_arg(*app, int);
	int j = va_arg(*app, int);
	convert(str, i, j);
	Fmt_puts(str+i, j-i, put, cl,  flags, width, precision);
}