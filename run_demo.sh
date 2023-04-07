#!/bin/bash
curdir=$(pwd)

cd build/demo-izk/bin
mkdir -p data
cp ${curdir}/install/lib/lib* ./

killall test_demo
sleep 1
./test_demo 1 &
./test_demo 2 &
wait
killall test_demo

echo "Run Done!"
