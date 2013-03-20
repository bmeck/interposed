OS=$(uname -s)
export NODE_CHANNEL_FD=1
if [ "$OS" == "Darwin" ]; then
  export DYLD_INSERT_LIBRARIES=`pwd`/nix.dylib
elif [ "$OS" == "SunOS" ]; then
  export LD_PRELOAD=`pwd`/nix.so ./a.out
fi
./a.out