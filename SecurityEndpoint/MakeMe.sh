#!/bin/sh


cd mbedtls-2.16.1
CFLAGS="-I$PWD/configs -DMBEDTLS_CONFIG_FILE='<config-ccm-ecdhe-tls1_2.h>'" make
cd ..


