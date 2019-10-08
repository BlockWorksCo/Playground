#!/bin/sh


# Compile executable using a .ld file that adds a marker to the base address.
gcc -g -o StringSection StringSection.c -Wl,-T StringSection.ld

# Copy .logdebug section out of executable.
objcopy --dump-section .logdebug=StringSection.logdebug StringSection

# Remove .logdebug section from executable.
cp StringSection StringSection.original
objcopy --remove-section .logdebug StringSection

# Generate the gperf source.

# Generate the lookup-code from gperf source.
gperf StringSection.gperf -t --slot-name=input --duplicates > lut.c
gcc -o lut lut.c

# example.
./lut


