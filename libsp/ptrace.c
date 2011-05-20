#include <sys/ptrace.h>
#include <sys/user.h>
#include "libsp.h"

long sp_ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data)
{
	struct syscall_regs regs;
	unsigned long esp;
	struct user_regs_struct ptrace_regs;

	if (request == PTRACE_TRACEME) {
		SEND_LEN(sizeof(regs));
		GET_ESP(NULL);
		SET_REGS(&regs, __NR_ptrace, request, 0, 0, 0, 0);

		SEND_REGS(&regs);
		RECV_REGS(&regs);
	} else if (request == PTRACE_GETREGS || request == PTRACE_SETREGS) {
		SEND_LEN(sizeof(regs) + sizeof(ptrace_regs));
		GET_ESP(&esp);
		SET_REGS(&regs, __NR_ptrace, request, pid, (unsigned long)addr, esp, 0);

		SEND_REGS(&regs);
		SEND_DATA(&ptrace_regs, sizeof(ptrace_regs));
		RECV_REGS(&regs);
		RECV_DATA(&ptrace_regs, sizeof(ptrace_regs));
	} else {
		SEND_LEN(sizeof(regs));
		GET_ESP(NULL);
		SET_REGS(&regs, __NR_ptrace, request, pid, (unsigned long)addr, (unsigned long)data, 0);

		SEND_REGS(&regs);
		RECV_REGS(&regs);
	}

	return regs.eax;
}
