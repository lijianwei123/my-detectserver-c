#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "common.h"

void show_help(void){
	char *help = "only -h -c\n"
			"-h show help\n"
			"-c  config file path\n";
	fprintf(stderr, help, strlen(help));
}

void get_mem_info(MEM_INFO *mem){
	FILE *fd;
	char buff[256];

	fd = fopen("/proc/meminfo", "r");
	fgets(buff, sizeof(buff), fd);
	sscanf(buff, "%s %lu", mem->name, &mem->total);
	fgets(buff, sizeof(buff), fd);
	sscanf(buff, "%s %lu", mem->name2, &mem->free);
	
	fclose(fd);
}

void get_cpu_info(CPU_INFO *cpu){
	FILE *fd;
	char buff[256];
	
	fd = fopen("/proc/stat", "r");
	fgets(buff, sizeof(buff), fd);
	sscanf(buff, "%s %u %u %u %u", cpu->name, &cpu->user, &cpu->nice, &cpu->system, &cpu->idle);
	
	fclose(fd);
}
void reverse(char *s)
{
  char *c;
  int i;
  c = s + strlen(s) - 1;
  while(s < c) {
    i = *s;
    *s++ = *c;
    *c-- = i;
  }
}

char *itoa(int n)
{
  int sign;
  char *ptr = (char *)malloc(getLength(n)+1);
  char *return_ptr = ptr;
  if ((sign = n) < 0) n = -n;
  do {
    *ptr++ = n % 10 + '0';
  } while ((n = n / 10) > 0);
  if (sign < 0) *ptr++ = '-';
  *ptr = '\0';
  reverse(ptr);
  return return_ptr;
}
//str转message
void Str2Msg(char *str, Msg *msg_ptr){
	cJSON *json;

	json = cJSON_Parse(str);
	if(!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());exit(1);}
	if(json->type == cJSON_Object){
		msg_ptr->command = json->child->valueint;

		msg_ptr->fromTo = (char *)malloc(sizeof(json->child->next->valuestring));
		memcpy(msg_ptr->fromTo, json->child->next->valuestring, sizeof(msg_ptr->fromTo));

		msg_ptr->sendTo = (char *)malloc(sizeof(json->child->next->next->valuestring));
		memcpy(msg_ptr->sendTo, json->child->next->next->valuestring, sizeof(msg_ptr->sendTo));

		msg_ptr->msgBody = (char *)malloc(sizeof(json->child->next->next->next->valuestring));
		memcpy(msg_ptr->msgBody, json->child->next->next->next->valuestring, sizeof(msg_ptr->msgBody));

		cJSON_Delete(json);
	}
}
//msg to str
void Msg2Str(Msg *msg_ptr, char *str){
	cJSON *tmp = cJSON_CreateObject();
	char *out;
	cJSON_AddNumberToObject(tmp, "command", msg_ptr->command);
	cJSON_AddStringToObject(tmp, "fromTo", msg_ptr->fromTo);
	cJSON_AddStringToObject(tmp, "sendTo", msg_ptr->sendTo);
	cJSON_AddStringToObject(tmp, "msgBody", msg_ptr->msgBody);
	out = cJSON_PrintUnformatted(tmp);
	memcpy(str, out, strlen(out) + 1);
	cJSON_Delete(tmp);
}
//多个字符连接
void mult_strcat(char *buffer, char *format,...){
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
}
//计算整型的位数
int getLength(const int tmp){
	char str[16];
	memset(str, 0, sizeof(str));
	sprintf(str, "%d", tmp);
	return strlen(str);
}

//初始化mysql连接
void init_mysql(MYSQL *mysql_ptr){
	static MYSQL *static_mysql;
	if(static_mysql != NULL){
		mysql_ptr = static_mysql;
		return;
	}
	mysql_init(static_mysql);
	if(!mysql_real_connect(static_mysql, "sql.w108.vhostgo.com", "lijianwei", "ljw123456", "lijianwei", 0, NULL, 0)){
		fprintf(stderr, "Error connecting to database: %s\n", mysql_error(static_mysql));
		exit(EXIT_FAILURE);
	}
	mysql_query(static_mysql,"set names utf8");

	mysql_ptr = static_mysql;
	return;
}

void free_part_Msg(Msg *msg){
	free(msg->fromTo);
	free(msg->sendTo);
	free(msg->msgBody);
}

void free_Client_Socket_Param(Client_Socket_Param *param){
	close(*(param->clientSocket));
	free(param->clientSocket);
	free(param->clientIp);
	free(param->clientPort);
	free(param);
}


