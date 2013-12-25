//检测服务器状态

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

#include "config.h"
#include "common.h"
#include "constant.h"
#include "clockin.model.h"

void *processthread(void *);
void processServerInfo(Msg *send_msg_ptr, Client_Socket_Param *socket_param_ptr);
void processClockIn(Msg *recv_msg_ptr, Client_Socket_Param *socket_param_ptr, Msg *send_msg_ptr);

MYSQL mysql;

int main(int argc, char **argv, char **env) {
	char *filename;
	int oc = 0;
	opterr = 1;
	while ((oc = getopt(argc, argv, "c:h")) != -1) {
		switch (oc) {
		case 'c':
			filename = optarg;
			break;
		case 'h':
			show_help();
			exit(0);
			break;
		case '?':
			printf("%c", optopt);
			break;
		}
	}
	int bind_port = atoi(get_item_by_key("bind_port", filename));

	//tcp server socket
	int serverSocket;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("serversocket error");
		exit(1);
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(bind_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("bind error");
		exit(1);
	}

	if(listen(serverSocket, 5) < 0){
		perror("listen error");
		exit(1);
	}

	int addr_len = sizeof(client_addr);

	char *p;
	Client_Socket_Param *socket_param_ptr;

	while(1){
		//每个线程新开辟参数存储空间,及时清除内存空间。
		socket_param_ptr = (Client_Socket_Param *)malloc(sizeof(Client_Socket_Param));
		socket_param_ptr->clientSocket = (int *)malloc(sizeof(int));

		*(socket_param_ptr->clientSocket) = accept(serverSocket, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len);
		if(*(socket_param_ptr->clientSocket) < 0){
			
			free(socket_param_ptr->clientSocket);
			free(socket_param_ptr);

			perror("accept error");
			continue;
		}


		p = inet_ntoa(client_addr.sin_addr);
		socket_param_ptr->clientIp = (char *)malloc(sizeof(*p));
		strcpy(socket_param_ptr->clientIp, p);
		p = NULL;
		
		socket_param_ptr->clientPort = (unsigned short *)malloc(sizeof(unsigned short));	
		*(socket_param_ptr->clientPort) = htons(client_addr.sin_port);

		pthread_t threadid;
		int temp;
		temp = pthread_create(&threadid, NULL, processthread, (void *)socket_param_ptr);

		printf("temp:%d\n", temp);
		printf("threadid:%ld\n", threadid);
		printf("clientIp:%s\n", socket_param_ptr->clientIp);
		printf("clientPort:%hu\n", *(socket_param_ptr->clientPort));
		if(temp != 0){
			printf("can't create thread:%s\n", strerror(temp));
		}

	}
	close(serverSocket);
	mysql_close(&mysql);

	return 0;
}

void *processthread(void *para){
	Client_Socket_Param *socket_param_ptr;

	socket_param_ptr = (Client_Socket_Param *)para;

	char recvdata[MAX_PACKET]; //recv data
	char senddata[MAX_PACKET]; //send data
	int idatanum;

	int clientsocket = *(socket_param_ptr->clientSocket);

	Msg recv_msg, send_msg; //数据格式

	while(1){
		//初始化
		memset(recvdata, 0, MAX_PACKET);
		memset(senddata, 0, MAX_PACKET);
		//接受数据
		idatanum = recv(clientsocket, recvdata, MAX_PACKET, 0);
		if(idatanum < 0){
			break;
		}
		
		Str2Msg(recvdata, &recv_msg); //解析数据

		switch(recv_msg.command){
			//获取服务器状态
			case SOCKET_SERVERINFO:
				processServerInfo(&send_msg, socket_param_ptr);
			break;

			case SOCKET_CLOCKIN:
				processClockIn(&recv_msg, socket_param_ptr, &send_msg);
			break;
		}

		Msg2Str(&send_msg, senddata);
		
		free_part_Msg(&recv_msg);
		free_part_Msg(&send_msg);

		int error = send(*(socket_param_ptr->clientSocket), senddata, strlen(senddata) + 1, 0);
		if(error < 0){
			break;
		}
	}
	printf("client:%s %hu closed!\n", socket_param_ptr->clientIp, *(socket_param_ptr->clientPort));
	//清除内存资源
	free_Client_Socket_Param(socket_param_ptr);

	pthread_exit(NULL);
}

void processServerInfo(Msg *send_msg_ptr, Client_Socket_Param *socket_param_ptr){
	MEM_INFO mem;
	CPU_INFO cpu;
	char format[20] = {0};
	char *p1, *p2;
	unsigned short *p3;

	get_cpu_info(&cpu);
	get_mem_info(&mem);

	send_msg_ptr->msgBody = (char *)malloc(MAX_BODY_PACKET);
	sprintf(format, "%s%s%s", "%s", SOCKET_MSG_SPLIT, "%s");
	mult_strcat(send_msg_ptr->msgBody, format, p1 = itoa(cpu.idle), p2 = itoa(mem.free));
	free(p1); free(p2);

	send_msg_ptr->command = SOCKET_ANONY;
	send_msg_ptr->fromTo = (char *)malloc(sizeof(SOCKET_SERVER));
	strcpy(send_msg_ptr->fromTo,SOCKET_SERVER);
	send_msg_ptr->sendTo = (char *)malloc(MAX_BODY_PACKET);
	memset(format, 0, 20);
	sprintf(format, "%s%s%s", "%s", SOCKET_MSG_SPLIT, "%d");
	mult_strcat(send_msg_ptr->sendTo, format, p1 = socket_param_ptr->clientIp, p3 = socket_param_ptr->clientPort);

	free(p1);free(p3);
}

//打卡操作
void processClockIn(Msg *recv_msg_ptr, Client_Socket_Param *socket_param_ptr, Msg *send_msg_ptr){
	//解析数据
	char *fromTo, *phoneNumber, *phoneMac, *s, *datetime;
	int result;

	fromTo = recv_msg_ptr->fromTo;
	if(!strchr(fromTo, atoi(SOCKET_MSG_SPLIT))){ //如果信息没有用分隔符
		send_msg_ptr->command = SOCKET_CLOCKIN_RETURN;
		send_msg_ptr->fromTo = (char *)malloc(sizeof(SOCKET_SERVER));
		strcpy(send_msg_ptr->fromTo, SOCKET_SERVER);
		send_msg_ptr->sendTo = NULL;
		s = itoa(RETURN_FAILURE);
		send_msg_ptr->msgBody = (char *)malloc(sizeof(s));
		strcpy(send_msg_ptr->msgBody, s);
		free(s);
		return;
	}
		
	phoneNumber = strtok(fromTo, SOCKET_MSG_SPLIT);
	phoneMac = strtok(NULL, SOCKET_MSG_SPLIT);
	datetime = itoa(time(0)); // NULL  == 0  time(NULL);
	//调用mysql 插入数据
	result = insert_clockin(phoneNumber, phoneMac, datetime);
	if(result < 0){
		send_msg_ptr->command = SOCKET_CLOCKIN_RETURN;
		send_msg_ptr->fromTo = (char *)malloc(sizeof(SOCKET_SERVER));
		strcpy(send_msg_ptr->fromTo, SOCKET_SERVER);
		send_msg_ptr->sendTo = NULL;
		s = itoa(RETURN_FAILURE);
		send_msg_ptr->msgBody = (char *)malloc(sizeof(s));
		strcpy(send_msg_ptr->msgBody, s);
		free(s);
		return;
	}
	//成功
	send_msg_ptr->command = SOCKET_CLOCKIN_RETURN;
	send_msg_ptr->fromTo = (char *)malloc(sizeof(SOCKET_SERVER));
	strcpy(send_msg_ptr->fromTo, SOCKET_SERVER);
	send_msg_ptr->sendTo = NULL;
	s = itoa(RETURN_SUCCESS);
	send_msg_ptr->msgBody = (char *)malloc(sizeof(s));
	strcpy(send_msg_ptr->msgBody, s);

	free(datetime);
	return;

}
