#include "libsp.h"

pid_t sp_getpid(void)
{
	struct syscall_regs regs;

	SET_REGS(&regs, __NR_getpid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}

pid_t sp_getppid(void)
{
	struct syscall_regs regs;

	SET_REGS(&regs, __NR_getppid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
