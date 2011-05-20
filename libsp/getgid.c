#include "libsp.h"

gid_t sp_getgid(void)
{
	struct syscall_regs regs;
	
	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);
	
	SET_REGS(&regs, __NR_getgid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}

gid_t sp_getegid(void)
{
	struct syscall_regs regs;
	
	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);
	
	SET_REGS(&regs, __NR_getegid, 0, 0, 0, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
