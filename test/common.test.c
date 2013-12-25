/*
 * test common.c
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../common.h"


int main(int argc, char **argv, char **envp) {
	MEM_INFO m;
	get_mem_info(&m);
	printf("%s%lu", m.name, m.total);
	return 0;
}
