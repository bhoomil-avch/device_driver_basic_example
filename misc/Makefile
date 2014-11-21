obj-m = misc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test_first:test_first.c
	
	cc -o test test_first.c
