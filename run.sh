#!/usr/bin/env bash



# Ideally, `js` and `wasm` files should be emitted to some build folder,
# but it doesn't work ¯\_(ツ)_/¯

emcc --bind -Oz Warp.cc -o Warp.js \
-s WASM=1 -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['addOnPostRun']" \
|| exit 1

tsembind ./Warp.js > Warp.d.ts