#include "libsp.h"

int sp_uname(struct utsname *buf)
{
	struct syscall_regs regs;
	unsigned long esp;

	SEND_LEN(sizeof(regs) + sizeof(*buf));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_uname, esp, 0, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(buf, sizeof(*buf));
	RECV_REGS(&regs);
	RECV_DATA(buf, sizeof(*buf));

	return regs.eax;
}
