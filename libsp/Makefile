TARGET = libsp.a
CC = gcc
AR = ar
CFLAGS = -Wall -m32

SRCS = chdir.c chroot.c creat.c execve.c getpid.c libsp.c mkdir.c open.c rename.c select.c socket.c	\
	uname.c write.c chmod.c close.c exit.c getuid.c link.c mknod.c ptrace.c readdir.c rmdir.c	\
	setgid.c stat.c	unlink.c chown.c connect.c dup.c getgid.c kill.c mmap.c read.c seek.c setuid.c	\
	time.c wait.c

OBJS = $(SRCS:.c=.o)

libsp: $(OBJS)
	$(AR) -rcs $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.a

.PHONY: clean
