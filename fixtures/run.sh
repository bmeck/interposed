OS=$(uname -s)
if [ "$OS" == "Darwin" ]; then
  DYLD_INSERT_LIBRARIES=nix.dylib ./a.out
else
  LD_PRELOAD=nix.so ./a.out
fi
