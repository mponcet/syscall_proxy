#include "libsp.h"

int sp_stat(const char *path, struct stat *st)
{
	struct syscall_regs regs;
	unsigned long path_len, esp;

	path_len = strlen(path) + 1;

	SEND_LEN(sizeof(regs) + path_len + sizeof(*st));
	GET_ESP(&esp);
	
	SET_REGS(&regs, __NR_stat64, esp, esp + path_len, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(path, path_len);
	SEND_DATA(st, sizeof(*st));
	RECV_REGS(&regs);
	RECV_DATA(NULL, path_len);
	RECV_DATA(st, sizeof(*st));
	
	return regs.eax;
}

int sp_fstat(int fd, struct stat *st)
{
	struct syscall_regs regs;
	unsigned long esp;
	
	SEND_LEN(sizeof(regs) + sizeof(*st));
	GET_ESP(&esp);
	
	SET_REGS(&regs, __NR_fstat64, fd, esp, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(st, sizeof(*st));
	RECV_REGS(&regs);
	RECV_DATA(st, sizeof(*st));
	
	return regs.eax;
}

int sp_lstat(const char *path, struct stat *st)
{
	struct syscall_regs regs;
	unsigned long path_len, esp;

	path_len = strlen(path) + 1;

	SEND_LEN(sizeof(regs) + path_len + sizeof(*st));
	GET_ESP(&esp);
	
	SET_REGS(&regs, __NR_lstat64, esp, esp + path_len, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(path, path_len);
	SEND_DATA(st, sizeof(*st));
	RECV_REGS(&regs);
	RECV_DATA(NULL, path_len);
	RECV_DATA(st, sizeof(*st));
	
	return regs.eax;
}
