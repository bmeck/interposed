OS=$(uname -s)
if [ "$OS" == "Darwin" ]; then
  gcc test.c --no-builtin-memset -flat_namespace
  gcc -dynamiclib -O3 -o nix.dylib -flat_namespace --no-builtin-memset nix.c
elif [ "$OS" == "SunOS" ]; then
  gcc test.c --no-builtin-memset -lsocket  -lnsl
  gcc -O3 -shared -G -fPIC -lc -o nix.so --no-builtin-memset nix.c
fi
