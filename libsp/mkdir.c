#include "libsp.h"

int sp_mkdir(const char *path, mode_t mode)
{
	struct syscall_regs regs;
	unsigned long esp, path_len = strlen(path) + 1;

	SEND_LEN(sizeof(regs) + path_len);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_mkdir, esp, mode, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(path, path_len);
	RECV_REGS(&regs);
	RECV_DATA(NULL, path_len);

	return regs.eax;
}
