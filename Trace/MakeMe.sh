#!/bin/sh


#gcc -fsanitize=address  -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld -lasan
gcc -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld 
arm-linux-gnueabi-gcc -g -O0 -o TraceTest.arm TraceTest.c -Wl,-T TraceTest.ld 


objcopy -S -O binary TraceTest TraceTest.bin
arm-linux-gnueabi-objcopy -S -O binary TraceTest.arm TraceTest.arm.bin

#objdump -h -w TraceTest | grep -E "^\s+" | awk -F'\\s+' '{print $3,$5;}'
#objdump -h -w TraceTest | grep -E "^\s+" | awk -F'\\s+' '{printf("%s %d\n", $3,strtonum("0x" $5));}' | sort -n -k 2,2

startOffset=$(objdump -h -w TraceTest | grep -E "^\s+" | awk -F'\\s+' '{offset=strtonum("0x" $5);if(offset>0) printf("%s %d\n", $3,offset);}' | sort -n -k 2,2 | head -1 | cut -d" " -f2)
rodataOffset=$(objdump -h -w TraceTest | grep -E "^\s+" | awk -F'\\s+' '{offset=strtonum("0x" $5);if(offset>0) printf("%s %d\n", $3,offset);}' | sort -n -k 2,2 | grep rodata | cut -d" " -f2)
echo $startOffset
echo $rodataOffset

fileOffset=$(expr $rodataOffset - $startOffset)
echo $fileOffset

