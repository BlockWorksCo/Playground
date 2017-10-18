#!/usr/bin/python



import binascii
import sys


if __name__ == '__main__':
    for line in sys.stdin:
        sys.stdout.write(binascii.hexlify(line))

