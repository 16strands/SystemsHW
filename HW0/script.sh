#!/bin/bash

for i in {8..24}
do
    SIZE=$((2**$i))
    if [ $i -le 10 ]
    then
        ITERS=1000000
    elif [ $i -le 12 ]
    then
        ITERS=100000
    elif [ $i -le 16 ]
    then
        ITERS=10000
    elif [ $i -le 20 ]
    then
        ITERS=1000
    elif [ $i -le 26 ]
    then
        ITERS=100
    fi
    ./prog $SIZE $ITERS >> output.log
    ./prog $SIZE $ITERS >> output.log
    ./prog $SIZE $ITERS >> output.log
done