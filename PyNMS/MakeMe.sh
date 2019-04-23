#!/bin/sh

cc -std=c99 -pedantic -Wall -g -Wextra -Wno-unused-parameter -Wno-unused-function -Wno-overlength-strings -o SecurityEndpoint SecurityEndpoint.c  -lssl -lcrypto -pthread -lm -ldl


