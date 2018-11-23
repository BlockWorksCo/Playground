#!/bin/sh


#gcc -fsanitize=address  -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld -lasan
gcc -g -O0 -o TraceTest TraceTest.c -Wl,-T TraceTest.ld 


