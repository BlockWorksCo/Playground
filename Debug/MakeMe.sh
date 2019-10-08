#!/bin/sh


# Compile executable.
gcc -g -o StringSection StringSection.c

# Copy .logdebug section out of executable.
objcopy --dump-section .logdebug=StringSection.logdebug StringSection

# Remove .logdebug section from executable.
cp StringSection StringSection.original
objcopy --remove-section .logdebug StringSection



