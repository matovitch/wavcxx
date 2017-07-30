#!/bin/sh

ROOT_DIR=$(dirname $0)

eval "$ROOT_DIR/bin/build.sh"
eval "$ROOT_DIR/bin/run.sh"
eval "$ROOT_DIR/sound/play.sh"