#include "libsp.h"

struct mmap_args
{
	void *addr;
	size_t length;
	int prot;
	int flags;
	int fd;
	off_t offset;
};

void *sp_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	unsigned long esp;
	struct syscall_regs regs;
	struct mmap_args args = { addr, length, prot, flags, fd, offset };

	SEND_LEN(sizeof(regs) + sizeof(args));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_mmap, esp, 0, 0, 0, 0);
	SEND_REGS(&regs);
	SEND_DATA(&args, sizeof(regs));
	RECV_REGS(&regs);
	RECV_DATA(&args, sizeof(args));

	return (void *)regs.eax;
}

int sp_munmap(void *addr, size_t length)
{
	struct syscall_regs regs;

	SEND_LEN(sizeof(regs));
	GET_ESP(NULL);

	SEND_REGS(&regs);
	RECV_REGS(&regs);

	return regs.eax;
}
