/*
 * config.c
 *
 *  Created on: 2012-11-19
 *  Author: root
 */
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


char *strtrimr(char *pstr){
	int i;
	i = strlen(pstr) -1;
	while((i >=0) && isspace(pstr[i])){
		pstr[i--] = '\0';
	}
	return pstr;
}

char *strtriml(char *pstr){
	int i=0,j;
	j = strlen(pstr) - 1;
	while(isspace(pstr[i]) && (i <= j))
		i++;
	if(0 < i)
		strcpy(pstr, &pstr[i]);
	return pstr;
}

char *strtrim(char *pstr){
	char *p;
	p = strtrimr(pstr);
	return strtriml(p);
}

typedef struct item_t{
	char *key;
	char *value;
} ITEM;


ITEM *read_from_config_file(char *filename){
	static ITEM items[20];

	if(items[0].key != NULL){
		return items;
	}

	FILE *fp;
	fp = fopen(filename, "r");
	char line[1024];
	char *pos;

	if(fp == NULL){
		fprintf(stderr, "cann't open file:%s\n", filename);
		exit(1);
	}
	int i = 0;
	while(fgets(line, 1024, fp) != NULL ){
		pos = strchr(line, '=');
		*pos++ = '\0';

		items[i].key = (char *)malloc(strlen(line) + 1);
		items[i].value = (char *)malloc(strlen(pos) + 1);

		strcpy(items[i].key, line);
		strcpy(items[i].value, pos);
		i++;
	}

	fclose(fp);

	return items;
}

char * get_item_by_key(char *key, char *filename){
	ITEM *items;
	items = read_from_config_file(filename);
	while(items->key != NULL){
		if(!strcmp(items->key, key)){
			return items->value;
		}
		items++;
	}
	return NULL;
}


