#include "libsp.h"

pid_t sp_waitpid(pid_t pid, int *status, int options)
{
	struct syscall_regs regs;
	unsigned long esp;

	SEND_LEN(sizeof(regs) + sizeof(*status));
	GET_ESP(&esp);
	SET_REGS(&regs, __NR_waitpid, pid, esp, options, 0, 0);

	SEND_REGS(&regs);
	SEND_DATA(status, sizeof(*status));
	RECV_REGS(&regs);
	RECV_DATA(status, sizeof(*status));

	return regs.eax;
}

pid_t sp_wait(int *status)
{
	return sp_waitpid(-1, status, 0);
}
