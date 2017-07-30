#!/bin/sh

ROOT_DIR=$(dirname $0)

#Clean binaries...
ls $ROOT_DIR/bin/* | tr ' ' '\n'   \
                   | grep -v ".sh" \
                   | while read binary
                     do
                         echo "Deleting $binary"
                         rm $binary
                     done

#Clean WAVE files...
ls $ROOT_DIR/sound/*.wav | tr ' ' '\n'   \
                         | while read wavFile
                           do
                               echo "Deleting $wavFile"
                               rm $wavFile
                           done