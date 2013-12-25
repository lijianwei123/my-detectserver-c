#include <stdio.h>
#include <string.h>
#include "/usr/local/mysql/include/mysql/mysql.h"
#include <stdlib.h>

int main(){
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	char *query;
	int t;
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "172.16.1.18", "root", "root", "anbo", 0, NULL, 0)){
		printf("Error connecting to database: %s\n", mysql_error(&mysql));
		return 0;
	}else{
		printf("Connected mysql successfully!\n");
	}
	query = "select * from admin";
	t = mysql_real_query(&mysql, query, (unsigned int)strlen(query));

	if(t){
		printf("Error making query:%s\n", mysql_error(&mysql));
	}else{
		printf("[%s] made...\n", query);
		res = mysql_store_result(&mysql);
		while(row = mysql_fetch_row(res)){
			for(t = 0; t < mysql_num_fields(res); t++){
				printf("%s ", row[t]);
			}
			printf("\n");
		}
	}
	mysql_close(&mysql);
	return 0;
}
