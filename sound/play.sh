#!/bin/sh

ROOTDIR=$(dirname $0)/..

ls $ROOTDIR/sound/*.wav | tr ' ' '\n' \
                        | while read wavFile
                          do
                             echo "Playing $(basename $wavFile)"
                             cvlc --play-and-exit $wavFile > /dev/null 2>&1
                          done
