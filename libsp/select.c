#include "libsp.h"

int sp_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	unsigned long esp, rfds, wfds, efds, tmout;
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs) + 3 * sizeof(fd_set) + sizeof(*timeout));
	GET_ESP(&esp);

	rfds = readfds ? esp : 0;
	wfds = writefds ? esp + sizeof(fd_set) : 0;
	efds = exceptfds ? esp + 2 * sizeof(fd_set) : 0;
	tmout = timeout ? esp + 3 * sizeof(fd_set) : 0;

	SET_REGS(&regs, __NR__newselect, nfds, rfds, wfds, efds, tmout);
	SEND_REGS(&regs);
	SEND_DATA(readfds, sizeof(*readfds));
	SEND_DATA(writefds, sizeof(*writefds));
	SEND_DATA(exceptfds, sizeof(*exceptfds));
	SEND_DATA(timeout, sizeof(*timeout));
	RECV_REGS(&regs);
	RECV_DATA(readfds, sizeof(*readfds));
	RECV_DATA(writefds, sizeof(*writefds));
	RECV_DATA(exceptfds, sizeof(*exceptfds));
	RECV_DATA(timeout, sizeof(*timeout));

	return regs.eax;
}
