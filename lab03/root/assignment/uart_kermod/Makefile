# Cross compilation Makefile for ARM
# Change KERN_SRC appropriately
KERN_SRC=../linux-5.8.16
obj-m = memory.o
 
all: 
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) modules
	arm-linux-gnueabi-gcc -Wall --static -O2 -mcpu=arm926ej-s mem_test.c -o mem_test

clean:
	make -C $(KERN_SRC) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-  M=$(PWD) clean