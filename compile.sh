#!/bin/bash

n="./src/"$1"*"
p="prog"$1

for f in `ls $n`; do
    echo "Compile: "$f
    gcc -Wall $f -o $p
done