/*
 * test config.c
 */
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../config.h"
#include "../common.h"

#if defined NDEBUG
#define NAME "lijianwei"
#else
#define NAME "weiyanping"
#endif

int main(int argc, char **argv, char **envp) {
	char *str = "lijianwei";
	int result = strcmp(str, strtrim(str));
	assert(!result);

	int oc = 0;
	opterr = 1;
	char *filename;
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
	int bind_port = 0;
	bind_port = atoi(get_item_by_key("bind_port", filename));
	printf("bind port: %d\n", bind_port);

	int cpu = 0;
	cpu = atoi(get_item_by_key("cpu", filename));
	printf("cpu detect:%d\n", cpu);

	return 0;
}
