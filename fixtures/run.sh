OS=$(uname -s)
if [ "$OS" == "Darwin" ]; then
  DYLD_INSERT_LIBRARIES=`pwd`/nix.dylib ./a.out
elif [ "$OS" == "SunOS" ]; then
  LD_PRELOAD=`pwd`/nix.so ./a.out
fi
