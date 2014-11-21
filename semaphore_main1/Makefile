obj-m += my_char_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

main:main.c
	cc -g -o main main.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
