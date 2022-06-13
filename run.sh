#!/bin/sh
f(){
    echo "arrival lambda = " $1;
    ./qkd/qkd --keyrate 0.005 --arrival $1 --service 0.05 --step 10 --time 86400000 --capacity 10 --lifetime 10000;
    echo;
}

for i in 0.0001 0.0002 0.0005 0.0008 0.001 0.002 0.005 0.008 0.01 0.02 0.05 0.08;
do
    f $i &
done
