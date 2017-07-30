#!/bin/sh

ROOTDIR=$(dirname $0)/..

ls $ROOTDIR/bin/* | tr ' ' '\n'   \
                  | grep -v ".sh" \
                  | while read binary
                    do
                        echo "Running $(basename $binary)"
                        eval $binary
                    done