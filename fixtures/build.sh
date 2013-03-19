OS=$(uname -s)
if [ "$OS" == "Darwin" ]; then
  gcc test.c --no-builtin-memset -flat_namespace
  gcc -dynamiclib -o nix.dylib -flat_namespace nix.c
elif [ "$OS" == "SunOS" ]; then
  gcc test.c --no-builtin-memset -lsocket  -lnsl
  gcc -shared -G -fPIC -lc -o nix.so nix.c
fi
