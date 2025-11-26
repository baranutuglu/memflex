obj-m += mymemory.o
mymemory-objs := src/kernel.o src/memory.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

run: all
	sudo insmod mymemory.ko
	sudo dmesg | tail -n 30
	sudo rmmod mymemory
