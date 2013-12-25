#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//计算整型的位数
int getLength(const int tmp){
	char str[16];
	memset(str, 0, sizeof(str));
	sprintf(str, "%d", tmp);
	int a = strlen(str);
	return a;
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
  char *ptr = (char *)malloc(getLength(n) + 1);
  char *return_ptr;
  return_ptr = ptr;
  if ((sign = n) < 0) n = -n;
  do {
    *ptr++ = n % 10 + '0';
  } while ((n = n / 10) > 0);
  if (sign < 0) *ptr++ = '-';
  *ptr = '\0';
  reverse(ptr);

  return return_ptr;
}

void main(){
	char *p;
	p = itoa(123456);
	printf("%s", p);
	free(p);
}
