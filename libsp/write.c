#include <stdio.h>
#include <stdlib.h>
#include "libsp.h"

ssize_t sp_write(int fd, void *buf, size_t count)
{
	struct syscall_regs regs;
	unsigned long esp;
	
	SEND_LEN(sizeof(regs) + count);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_write, fd, esp, count, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(buf, count);
	RECV_REGS(&regs);
	RECV_DATA(NULL, count);

	return regs.eax;
}
