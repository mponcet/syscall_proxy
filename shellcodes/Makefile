CC = gcc
CFLAGS = -Wall -m32

SRCS = syscall_srv.S reverse_tcp.S
OBJS = $(SRCS:.c=.o)

gen_sc: gen_sc.o syscall_srv.o reverse_tcp.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o gen_sc

.PHONY: clean
