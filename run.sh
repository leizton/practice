#! /bin/bash
clear

if [ ! -d 'build' ]; then
    mkdir build
    cd build
    cmake -DMODULE=$1 ..
else
    cd build
fi
make
if [ $? -eq 0 ]; then
    clear
    ./practice
fi