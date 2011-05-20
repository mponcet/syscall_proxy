#include <string.h>
#include <stdlib.h>
#include "libsp.h"

int sp_link(const char *oldpath, const char *newpath)
{
	struct syscall_regs regs;
	unsigned long esp;
	unsigned long oldpath_len = strlen(oldpath) + 1;
	unsigned long newpath_len = strlen(newpath) + 1;

	SEND_LEN(sizeof(regs) + oldpath_len + newpath_len);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_link, esp, esp + oldpath_len, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(oldpath, oldpath_len);
	SEND_DATA(newpath, newpath_len);
	RECV_REGS(&regs);
	RECV_DATA(NULL, oldpath_len);
	RECV_DATA(NULL, newpath_len);

	return regs.eax;
}

int sp_symlink(const char *oldpath, const char *newpath)
{
	struct syscall_regs regs;
	unsigned long esp;
	unsigned long oldpath_len = strlen(oldpath) + 1;
	unsigned long newpath_len = strlen(newpath) + 1;

	SEND_LEN(sizeof(regs) + oldpath_len + newpath_len);
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_symlink, esp, esp + oldpath_len, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(oldpath, oldpath_len);
	SEND_DATA(newpath, newpath_len);
	RECV_REGS(&regs);
	RECV_DATA(NULL, oldpath_len);
	RECV_DATA(NULL, newpath_len);

	return regs.eax;
}
