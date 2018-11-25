#!/bin/sh


#gcc -fsanitize=address  -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld -lasan
gcc -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld 
arm-linux-gnueabi-gcc -g -O0 -o TraceTest.arm TraceTest.c -Wl,-T TraceTest.ld 


objcopy -S -O binary TraceTest TraceTest.bin
arm-linux-gnueabi-objcopy -S -O binary TraceTest.arm TraceTest.arm.bin

objdump -h -w TraceTest | grep -E "^\s+" | awk -F'\\s+' '{print $3,$5;}'


