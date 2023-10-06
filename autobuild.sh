#! /bin/bash

rm -rf `pwd`/build/*
mkdir `pwd`/bin
cd `pwd`/build &&
cmake .. &&
make
