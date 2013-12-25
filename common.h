/*
 * common.h
 *
 *  Created on: 2012-11-19
 *      Author: root
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdarg.h>
#include <stdlib.h>
#include <mysql.h>

#include "cJSON.h"


typedef struct
{
	char name[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
} CPU_INFO;

typedef struct
{
	char name[20];
	unsigned long total;
	char name2[20];
	unsigned long free;
} MEM_INFO;


typedef struct  
{
	int command;
	char *fromTo;
	char *sendTo;
	char *msgBody;
} Msg;


typedef struct{
	int *clientSocket;
	char *clientIp;
	unsigned short *clientPort;

} Client_Socket_Param;

extern void show_help(void);
extern void get_mem_info(MEM_INFO *);
extern void get_cpu_info(CPU_INFO *);
extern char *itoa(int);
extern void Str2Msg(char *, Msg *);
extern void Msg2Str(Msg *, char *);
extern void mult_strcat(char *, char *,...);
extern int getLength(const int);

extern void init_mysql(MYSQL *);
extern MYSQL mysql;

extern void free_part_Msg(Msg *msg);
extern void free_Client_Socket_Param(Client_Socket_Param *param);
#endif /* COMMON_H_ */
