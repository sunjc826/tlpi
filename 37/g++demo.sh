# What this does is basically
# instead of linking libtlpi.so and all the object files in out/app into an executable
# I link them into a shared library called libtlpi_cpy.so
# Additionally, I set rpath of this shared library to SO_DIR because
# libtlpi_cpy.so depends on libtlpi.so
# We can verify this by running
# ldd "${SO_DIR}/libtlpi_cpy.so"
# Finally, I convert this shared library into an executable a-dynamic.out
# Again, I need to set the rpath of a-dynamic.out to SO_DIR
# because a-dynamic.out depends on libtlpi_cpy.so
# Note that if rpath is not set, I can also specify LD_LIBRARY_PATH as an environment var.

# Obviously, I can just combine libtlpi.so and the object files into an executable directly.
# This is a contrived example to understand linking better.

set -x
SO_DIR=$(realpath out/so)
# Note that the order of arguments mostly don't matter except for
# -llibrary
# the manpages for gcc/g++ also mention this
g++ -g -shared "-L${SO_DIR}" out/app/*.o -ltlpi "-Wl,-rpath,${SO_DIR}" -o out/so/libtlpi_cpy.so
g++ -g "-L${SO_DIR}" -ltlpi_cpy "-Wl,-rpath,${SO_DIR}" -o a-dynamic.out
a-dynamic.out
set +x