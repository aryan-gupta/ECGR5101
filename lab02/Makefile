NEWLIB=/home/aryan/Projects/ECGR5101/lab02/newlib-2.5.0
LIBGCC=/usr/lib/gcc/arm-none-eabi/10.2.0/libgcc.a

mem_alloc.bin: mem_alloc
	arm-none-eabi-objcopy -O binary mem_alloc mem_alloc.bin

mem_alloc: mem_alloc.ld mem_alloc.o startup.o syscalls.o
	arm-none-eabi-gcc -nostdlib -T mem_alloc.ld mem_alloc.o startup.o syscalls.o "${NEWLIB}/arm-none-eabi/newlib/libc.a" "${LIBGCC}" -o mem_alloc

syscalls.o: syscalls.c
	arm-none-eabi-gcc -mcpu=cortex-a8 -I "${NEWLIB}/newlib/libc/include" -c -o syscalls.o syscalls.c

startup.o: syscalls.c
	arm-none-eabi-as -mcpu=cortex-a8 -o startup.o startup.s

mem_alloc.o: mem_alloc.c
	arm-none-eabi-gcc -mcpu=cortex-a8 -I "${NEWLIB}/newlib/libc/include" -c -o mem_alloc.o mem_alloc.c

clean:
	rm *.o mem_alloc mem_alloc.bin
