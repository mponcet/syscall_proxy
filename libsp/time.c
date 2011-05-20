#include "libsp.h"

time_t time(time_t *t)
{
	struct syscall_regs regs;
	unsigned long esp;

	SEND_LEN(sizeof(regs) + sizeof(*t));
	GET_ESP(&esp);
	if (t)
		SET_REGS(&regs, __NR_time, esp, 0, 0, 0, 0);
	else
		SET_REGS(&regs, __NR_time, 0, 0, 0, 0, 0);

	SEND_REGS(&regs);
	SEND_DATA(t, sizeof(*t));
	RECV_REGS(&regs);
	RECV_DATA(t, sizeof(*t));

	return regs.eax;
}
