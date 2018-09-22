#!/bin/sh
umount $1
mount -t tmpfs -o size=32G tmpfs $1
cp out-s*/libleveldb* /usr/local/lib
rm db_bench
make
cp out-static/db_bench .
./db_bench $1 $2
./db_bench $1 $3
