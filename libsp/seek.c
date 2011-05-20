#include "libsp.h"

off_t sp_lseek(int fd, off_t offset, int whence)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SET_REGS(&regs, __NR_lseek, fd, offset, whence, 0, 0);
	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
