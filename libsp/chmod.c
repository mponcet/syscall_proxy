#include "libsp.h"

int sp_chmod(const char *path, mode_t mode)
{
	struct syscall_regs regs;
	unsigned long esp, path_len = strlen(path) + 1;

	SEND_LEN(sizeof(regs) + path_len);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_chmod, esp, mode, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(path, path_len);
	RECV_REGS(&regs);
	RECV_DATA(NULL, path_len);

	return regs.eax;
}


int sp_fchmod(int fd, mode_t mode)
{
	struct syscall_regs regs;
	unsigned long esp;

	SEND_LEN(sizeof(regs));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_fchmod, fd, mode, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);
 
	return regs.eax;
}

