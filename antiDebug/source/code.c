#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void sus() {
	char *buf, num1[13], num2[13];
	buf = malloc(300);
	memset(num1, 0, sizeof(num1));
	memset(num2, 0, sizeof(num2));

	FILE *sth = fopen("/proc/self/maps", "r");
	while (strstr(buf, "[st") == NULL) {
		memset(buf, 0, sizeof(buf));
		fgets(buf, 150, sth);
	}
	fclose(sth);

	strncpy(num1, buf, 12);
	strncpy(num2, buf + 13, 12);

	unsigned long start = strtoll(num1, num1, 16);
	unsigned long finish = strtoll(num2, num2, 16);

	FILE *mem_stream = fopen("/proc/self/mem", "r");
	fseek(mem_stream, start, 0);
	char *dump_buffer = malloc(finish - start + 1);
	fread(dump_buffer, finish - start + 1, 1, mem_stream);
	fclose(mem_stream);
	
	FILE *output = fopen("./dump", "w");
	fwrite(dump_buffer, finish - start + 1, 1, output);
	fclose(output);
}

int main() {
	FILE *text = fopen("./somethingSecret.txt", "r");
	char *something;
	something = malloc(5715);
	fread(something, 5715, 1, text);
	fclose(text);

	printf("Oh no, the stupid author leaked something:\n");

	char key[5715], store[5715];
	memset(key, 0, sizeof(key));
	memset(store, 0, sizeof(store));

	srand(time(NULL));

	for (int i=0; i<5715; i++)
		key[i] = (rand() & 0xff);

	for (int i=0; i<5715; i++)
		store[i] = (((something[i] ^ key[i]) + key[i]) & 0xff);

	for (int i=0; i<3; i++)
		printf("%02X ", (store[i] & 0xff));
	putchar(10);

	sus();
	return 0;
}