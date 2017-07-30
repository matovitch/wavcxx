#!/bin/bash

ROOTDIR=$(dirname $0)/..

ls $ROOTDIR/example/*.cpp | tr ' ' '\n' \
                          | while read cppFile
                            do
                                echo "Compiling $(basename $cppFile)"
                                g++ -std=c++11 -I . $cppFile -o ./bin/$(basename $cppFile | cut -d '.' -f 1)
                            done

