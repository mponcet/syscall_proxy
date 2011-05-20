#include "libsp.h"

int sp_dup(int oldfd)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_dup, oldfd, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}

int sp_dup2(int oldfd, int newfd)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_dup2, oldfd, newfd, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
