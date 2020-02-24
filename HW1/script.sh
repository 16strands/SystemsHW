#!/bin/bash

for i in {10..27}
do
    SIZE=$((2**$i))
    if [ $i -le 12 ]
    then
        ITERS=1000000
    elif [ $i -le 16 ]
    then
        ITERS=100000
    elif [ $i -le 20 ]
    then
        ITERS=10000
    elif [ $i -le 24 ]
    then
        ITERS=1000
    elif [ $i -le 28 ]
    then
        ITERS=100
    fi
    ./prog $SIZE $ITERS >> output.log
done