#ifndef __LIBC_SP_H
#define __LIBC_SP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <asm/unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/ptrace.h>
#include <sys/mman.h>


extern int __sock;

void wait_connect_back(unsigned short port);
void send_sp_shellcode(char *shellcode, size_t size);
ssize_t safe_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t safe_recv(int sockfd, void *buf, size_t len, int flags);

struct syscall_regs {
	unsigned long eax, ebx, ecx, edx, esi, edi;
};

static inline void SET_REGS(struct syscall_regs *regs,
			unsigned long eax, unsigned long ebx, unsigned long ecx,
			unsigned long edx, unsigned long esi, unsigned long edi)
{
	regs->eax = eax;
	regs->ebx = ebx;
	regs->ecx = ecx;
	regs->edx = edx;
	regs->esi = esi;
	regs->edi = edi;
}

static inline void GET_ESP(unsigned long *esp)
{
	unsigned long tmp;

	safe_recv(__sock, &tmp, sizeof(tmp), 0);

	if (esp)
		*esp = tmp + sizeof(struct syscall_regs);
}

static inline void SEND_LEN(unsigned long len)
{
	safe_send(__sock, &len, sizeof(len), 0);
}

static inline void SEND_REGS(struct syscall_regs *regs)
{
	safe_send(__sock, regs, sizeof(*regs), 0);
}

static inline void RECV_REGS(struct syscall_regs *regs)
{
	safe_recv(__sock, regs, sizeof(*regs), 0);
}

static inline void SEND_DATA(const void *data, unsigned long len)
{
	if (!data) {
		char __data[len];
		safe_send(__sock, __data, len, 0);
	} else {
		safe_send(__sock, data, len, 0);
	}
}

static inline void RECV_DATA(void *data, unsigned long len)
{
	
	if (!data) {
		char __data[len];
		safe_recv(__sock, __data, len, 0);
	} else {
		safe_recv(__sock, data, len, 0);
	}
}

/*
 * implemented syscalls
 */

void sp_exit(int);
ssize_t sp_read(int , void *, size_t);
ssize_t sp_write(int, void *, size_t);
int sp_open(const char *, int);
int sp_openat(int dirfd, const char *path, int flags);
int sp_close(int);
off_t sp_lseek(int fd, off_t offset, int whence);
int sp_creat(const char *pathname, mode_t mode);
int sp_unlink(const char *path);
int sp_rename(const char *oldpath, const char *newpath);
int sp_link(const char *, const char *);
ssize_t sp_readlink(const char *path, char *buf, size_t bufsiz);
int sp_mknod(const char *path, mode_t mode, dev_t dev);
int sp_chmod(const char *path, mode_t mode);
int sp_fchmod(int fd, mode_t mode);
int sp_chown(const char *path, uid_t owner, gid_t group);
int sp_lchown(const char *path, uid_t owner, gid_t group);
int sp_fchown(int fd, uid_t owner, gid_t group);
int sp_stat(const char *path, struct stat *buf);
int sp_lstat(const char *path, struct stat *buf);
int sp_fstat(int fd, struct stat *buf);
int sp_chdir(const char *path);
int sp_fchdir(int fd);

#define NAME_MAX 255
struct dirent {
	long  d_ino;			/* inode number */
	off_t d_off;			/* offset to this old_linux_dirent */
	unsigned short d_reclen;	/* length of this d_name */
	char  d_name[NAME_MAX+1];	/* filename (null-terminated) */
};
ssize_t sp_readdir(unsigned int fd, struct dirent *dirp, unsigned int count);
int sp_mkdir(const char *path, mode_t mode);
int sp_rmdir(const char *path);
int sp_dup(int oldfd);
int sp_dup2(int oldfd, int newfd);
int sp_chroot(const char *path);

int sp_execve(const char *, char * const [], char * const []);
pid_t sp_getpid(void);
pid_t sp_getppid(void);
int sp_kill(pid_t pid, int sig);
pid_t sp_wait(int *status);
pid_t sp_waitpid(pid_t pid, int *status, int options);
void *sp_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

uid_t sp_getuid(void);
uid_t sp_geteuid(void);
gid_t sp_getgid(void);
gid_t sp_getegid(void);
int sp_setuid(uid_t uid);
int sp_setgid(gid_t gid);

int sp_socket(int, int, int);
int sp_connect(int, struct sockaddr *, socklen_t);

time_t sp_time(time_t *t);
int sp_uname(struct utsname *buf);
long sp_ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);

#endif
