#!/usr/bin/env bash
emcc --bind -Oz Warp.cc -o dist/Warp.js \
-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','addOnPostRun'] \
-s WASM=1 -s NO_EXIT_RUNTIME=1 \
-s MODULARIZE=1 \
# -s ENVIRONMENT='node,web,worker' \
# -s SINGLE_FILE=1 \