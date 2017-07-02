#!/bin/bash

#set -xe

rm -f /tmp/Hash

pushd $1
find . | sort > /tmp/Hash
cat /tmp/Hash | xargs  stat -c "%a %D %F %g %u %n"   > /tmp/Hash2
cat /tmp/Hash2 >> /tmp/Hash
md5sum /tmp/Hash
popd

