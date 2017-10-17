#!/usr/bin/python



import binascii


if __name__ == '__main__':
    for line in sys.stdin:
        print(binascii.unhexlify(line))

