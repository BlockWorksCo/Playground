#!/bin/sh


gcc -fsanitize=address  -g -O0 -o TraceTest TraceTest.c -lasan


