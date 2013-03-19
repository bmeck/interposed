OS=$(uname -s)
if [ "$OS" == "Darwin" ]; then
  echo "BUILDING FOR MAC"
  gcc test.c --no-builtin-memset -flat_namespace
  gcc -dynamiclib -o nix.dylib -flat_namespace nix.c
else
  gcc test.c --no-builtin-memset
  gcc -shared -o nix.so -flat_namespace nix.c
  echo 1
fi
