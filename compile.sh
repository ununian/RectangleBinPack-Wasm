#!/usr/bin/env bash
emcc --bind -Oz Warp.cc -o dist/Warp.js \
-s WASM=1 \
-s MODULARIZE=1 \
# -s ENVIRONMENT='node,web,worker' \
# -s SINGLE_FILE=1 \