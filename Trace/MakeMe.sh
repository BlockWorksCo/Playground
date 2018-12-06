#!/bin/sh


gcc -fsanitize=address  -g -O0 -o TraceTest TraceTest.c crc32.c TraceEncode.c TraceDecode.c Hash.c TracePacket.c -Wl,-T TraceTest.ld  -lasan
gcc -fsanitize=address  -g -O0 -o TraceServer TraceServer.c TraceDecode.c Hash.c crc32.c TracePacket.c  -lasan
arm-linux-gnueabi-gcc -g -O0 -o TraceTest.arm TraceTest.c crc32.c TraceEncode.c Hash.c -Wl,-T TraceTest.ld 


objcopy -S -O binary TraceTest TraceTest.bin
arm-linux-gnueabi-objcopy -S -O binary TraceTest.arm TraceTest.arm.bin

#OBJDUMP=arm-none-eabi-objdump
#ELFFILE=TraceTest.arm
OBJDUMP=objdump
ELFFILE=TraceTest

startOffset=$($OBJDUMP -h -w $ELFFILE | grep -E "^\s+" | awk -F'\\s+' '{offset=strtonum("0x" $5);if(offset>0) printf("%s %d\n", $3,offset);}' | sort -n -k 2,2 | head -1 | cut -d" " -f2)
rodataOffset=$($OBJDUMP -h -w $ELFFILE | grep -E "^\s+" | awk -F'\\s+' '{offset=strtonum("0x" $5);if(offset>0) printf("%s %d\n", $3,offset);}' | sort -n -k 2,2 | grep rodata | cut -d" " -f2)
echo $startOffset
echo $rodataOffset

fileOffset=$(expr $rodataOffset - $startOffset)
echo $fileOffset


#
# Small demo of how to use it.
# - TraceTest output hex trace packet.
# - TraceDecode decodes the packet with appropriate parameters to output trace text.
#
./TraceTest > out.hex
./TraceServer TraceTest.bin $fileOffset < out.hex 
