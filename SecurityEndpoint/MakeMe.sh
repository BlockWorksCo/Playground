#!/bin/sh


cd libcoap
./autogen.sh
./configure --disable-shared
make
cd ..


cd mbedtls-2.16.1
CFLAGS="-I$PWD/configs -DMBEDTLS_CONFIG_FILE='<config-ccm-ecdhe-tls1_2.h>'" make clean all
cd ..

gcc -o SecurityEndpoint SecurityEndpoint.c  -Imbedtls-2.16.1/include  -Lmbedtls-2.16.1/library -lmbedtls -lmbedcrypto -lmbedx509

