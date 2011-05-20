#include "libsp.h"

int sp_socket(int domain, int type, int protocol)
{
	struct syscall_regs regs;
	unsigned long esp;

	SEND_LEN(sizeof(regs) + 3 * sizeof(int));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_socketcall, 1, esp, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(&domain, sizeof(domain));
	SEND_DATA(&type, sizeof(type));
	SEND_DATA(&protocol, sizeof(protocol));
	RECV_REGS(&regs);
	RECV_DATA(NULL, 3 * sizeof(int));

	return regs.eax;
}
