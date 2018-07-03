#!/bin/sh
rm -f a.out
g++ -std=c++14 main.cpp
if [ -f a.out ]; then
  ./a.out
fi
