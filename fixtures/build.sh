gcc test.c --no-builtin-memset -flat_namespace
gcc -dynamiclib -o mac.dylib -flat_namespace mac.c
