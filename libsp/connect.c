#include "libsp.h"

int sp_connect(int sockfd, struct sockaddr *addr, socklen_t addrlen)
{
	struct syscall_regs regs;
	unsigned long esp, addr_ptr;
	
	SEND_LEN(sizeof(regs) + sizeof(sockfd) + sizeof(addr_ptr) + sizeof(*addr) + sizeof(addrlen));
	GET_ESP(&esp);

	SET_REGS(&regs, __NR_socketcall, 3, esp, 0, 0, 0);
	SEND_REGS(&regs);

	SEND_DATA(&sockfd, sizeof(sockfd));
	addr_ptr = esp + sizeof(sockfd) + sizeof(addr_ptr) + sizeof(addrlen);
	SEND_DATA(&addr_ptr, sizeof(addr_ptr));
	SEND_DATA(&addrlen, sizeof(addrlen));
	SEND_DATA(addr, sizeof(*addr));

	RECV_REGS(&regs);
	RECV_DATA(NULL, sizeof(sockfd) + sizeof(addr_ptr) + sizeof(*addr) + sizeof(addrlen));

	return regs.eax;
}
