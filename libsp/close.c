#include "libsp.h"

int sp_close(int fd)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_close, fd, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
	
