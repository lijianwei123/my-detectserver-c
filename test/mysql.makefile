all:mysql
mysql:../mysql.c
	gcc -Wall -g ../mysql.c -L/usr/local/mysql/lib/mysql -lmysqlclient -o mysql
