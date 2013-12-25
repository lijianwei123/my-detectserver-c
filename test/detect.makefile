#detect
#todo --prefix
#use mysql
#yum install -y mysql
#yum install -y mysql-devel
#-O2 
CC = gcc
C_INCLUDE_PATH = /usr/local/mysql/include/mysql:/usr/include/mysql
export C_INCLUDE_PATH
LIBRARY_PATH = /usr/local/mysql/lib/mysql:/usr/lib/mysql
export LIBRARY_PATH
detect:detect.c config.c common.c
	$(CC) -Wall -g  detect.c config.c common.c cJSON.c clockin.model.c -lpthread  -lmysqlclient -o detect
clean:
	rm -rf detect
install:detect
	touch detect.log && sudo chmod 777 detect.log && ./detect -c detect.conf 2>&1>detect.log &
