PHP_ARG_ENABLE(utils,
    [Whether to enable the "utils" extension],
    [  --enable-utils      Enable "utils" extension support])

if test $PHP_UTILS != "no"; then
    CXX_FLAGS="-std=c++0x"
    PHP_REQUIRE_CXX()
    PHP_SUBST(UTILS_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, UTILS_SHARED_LIBADD)
    PHP_NEW_EXTENSION(utils, utils.cc uuid.cc, $ext_shared)
fi

