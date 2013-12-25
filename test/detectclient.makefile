#detectclient
CC = gcc
detectclient:detectclient.c
	$(CC) -g -Wall detectclient.c -o detectclient
clean:
	rm -rf detectclient
install:detectclient
	install -m 755 -o root detectclient ./
