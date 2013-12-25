#socketClient socketServer
#command start  tab
#@ don't display command self
#target:dependent 

CC=gcc
all:socketServer socketClient
socketClient:../socketClient.c
	$(CC) -Wall -g ../socketClient.c -o socketClient
	@echo ""
	@echo "socketClient build complete."
	@echo ""
socketServer:../socketServer.c
	$(CC) -Wall -g ../socketServer.c -o socketServer
	@echo ""
	@echo "socketServer build complete."
	@echo ""
clean:
	rm -rf socketClient socketServer
install:socketClient socketServer
	./socketServer ./socketClient 
