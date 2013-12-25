#include <stdio.h>
#include <stdarg.h>

char buffer[80] = "30 90.0 abc";

int vssf(char *fmt, ...){
	va_list arg;
	int cnt;
	fflush(stdin);
	va_start(arg, fmt);
	cnt = vsscanf(buffer, fmt, arg);
	va_end(arg);
	return cnt;
}
int main(){
	int inumber;
	float fnumber;
	char string[80];
	vssf("%d %f %s", &inumber, &fnumber, string);
	printf("%d %f %s\n", inumber, fnumber, string);
	return 0;
}