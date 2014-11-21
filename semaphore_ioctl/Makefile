obj-m += ioctl.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

test:test.c
	cc -g -o test test.c

test1:test1.c
	cc -g -o test1 test1.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
