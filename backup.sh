#!/bin/bash

make -f MakeFile

if [ $? -eq 0 ]; then
    ./backup
else
    echo "Makefile compiling failed"
fi
