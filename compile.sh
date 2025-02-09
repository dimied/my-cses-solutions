#!/bin/bash

n="./src/"$1"*"
p="prog"$1

for f in `ls $n`; do
    echo "Compile: "$f
    gcc -Wall -DLOCAL_DEV_ENV=1 $f -o $p
done