#!/bin/bash

n="./src/"$1"*"
p="prog"$1

#$2 stores options, e.g. -pg for profiling
for f in `ls $n`; do
    echo "Compile: "$f
    gcc -Wall -g -DLOCAL_DEV_ENV=1 -DNUM_TESTS=$2 $f -o $p
done