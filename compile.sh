#!/usr/bin/env bash
emcc --bind -Oz Warp.cc -o dist/Warp.js \
-s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','addOnPostRun'] \
-s MODULARIZE=1 \
-s ENVIRONMENT='web,worker' \
# -s SINGLE_FILE=1 \
# || exit 1

# emcc --bind -Oz Warp.cc -o dist/Warp.js \
# -s EXPORTED_RUNTIME_METHODS=['ccall','cwrap','addOnPostRun'] \
# -s EXPORT_ES6=1 \
# -s USE_ES6_IMPORT_META=0 \
# -s ENVIRONMENT='web,worker' \
# || exit 1
