#include "libsp.h"

uid_t sp_getuid(void)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_getuid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);
	
	return regs.eax;
}

uid_t sp_geteuid(void)
{
	struct syscall_regs regs;
	
	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_geteuid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);
	
	return regs.eax;
}
