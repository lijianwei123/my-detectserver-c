#cJSON.makefile
cJSON.test:test/cJSON.test.c cJSON.c
	gcc -Wall -g -lm test/cJSON.test.c cJSON.c -o cJSON.test
clean:
	rm -rf cJSON.test
