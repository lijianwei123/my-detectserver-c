/*
 * constant.h
 *
 *  Created on: 2012-12-16
 *      Author: root
 */

#ifndef CONSTANT_H_
#define CONSTANT_H_

typedef enum {RETURN_SUCCESS = 0, RETURN_FAILURE = -1} return_sign;
typedef enum {
	SOCKET_ANONY = 0, //匿名
	SOCKET_CLOCKIN = 1, //打卡
	SOCKET_CLOCKIN_RETURN = 2, //打卡结束
	SOCKET_SERVERINFO = 3 //服务器状态
} command;

#define SOCKET_MSG_SPLIT " "
#define SOCKET_SERVER "SERVER"
//通讯最大数据包 1K
#define MAX_PACKET 1024
#define MAX_BODY_PACKET 1000


#endif /* CONSTANT_H_ */
