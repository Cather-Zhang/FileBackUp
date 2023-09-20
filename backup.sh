#!/bin/bash

make -f MakeFile

echo "Makefile compiling failed"

if [ $? -eq 0 ]; then
    echo "Makefile compiling failed"
    ./backup
else
    echo "Makefile compiling failed"
fi
