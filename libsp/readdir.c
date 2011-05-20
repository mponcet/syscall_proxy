#include "libsp.h"

ssize_t sp_readdir(unsigned int fd, struct dirent *dirp, unsigned int count)
{
	unsigned long esp;
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs) + sizeof(*dirp));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_readdir, fd, esp, count, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(NULL, sizeof(*dirp));
	RECV_REGS(&regs);
	RECV_DATA(dirp, sizeof(*dirp));

	return regs.eax;
}
