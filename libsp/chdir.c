#include "libsp.h"

int sp_chdir(const char *path)
{
	struct syscall_regs regs;
	unsigned long esp, path_len = strlen(path) + 1;

	SEND_LEN(sizeof(regs) + path_len);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_chdir, esp, 0, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(path, path_len);
	RECV_REGS(&regs);
	RECV_DATA(NULL, path_len);

	return regs.eax;
}


int sp_fchdir(int fd)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);
	SET_REGS(&regs, __NR_fchdir, fd, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
