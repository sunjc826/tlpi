# We can check the glibc version this way
eval sudo $(ldd $(which ls) | grep libc | awk '{ printf $3 }')