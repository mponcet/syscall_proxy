CC = gcc
CFLAGS = -m32 -Wall -I..
LDFLAGS = -L../libsp -lsp

all: shell stage1

shell: shell.o libsp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

stage1: stage1.o
	$(CC) $(CFLAGS) -o $@ $^

libsp:
	make -C ../libsp/

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.S
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o stage1 shell

.PHONY: clean
