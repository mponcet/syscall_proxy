all: samples shellcode

samples: libsp
	make -C samples/ shell

libsp:
	make -C libsp/

shellcode:
	make -C shellcodes/

clean:
	make -C libsp/ clean
	make -C samples/ clean
	make -C shellcodes/ clean
	rm -f *.o

.PHONY: clean libsp samples
