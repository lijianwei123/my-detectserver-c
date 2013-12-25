#multhreadclient multhreadserver
CC = gcc
all:multhreadclient multhreadserver
multhreadclient:../multhreadclient.c
	$(CC) -Wall -g ../multhreadserver.c -lpthread -o multhreadclient
multhreadserver:../multhreadserver.c
	$(CC) -Wall -g ../multhreadserver.c -lpthread -o multhreadserver
clean:multhreadclient multhreadserver
	rm -rf multhreadclient multhreadserver