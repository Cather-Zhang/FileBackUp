#!/bin/bash

make

echo "Makefile compiling failed"

if [ $? -eq 0 ]; then
    echo "Makefile compiling failed"
    ./backup testFolder
else
    echo "Makefile compiling failed"
fi
