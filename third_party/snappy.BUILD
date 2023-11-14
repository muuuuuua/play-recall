licenses(["notice"])

# Modified from https://github.com/mzhaom/trunk/blob/master/third_party/snappy/BUILD
#               https://github.com/smyte/smyte-db/blob/master/third_party/snappy.BUILD

genrule(
    name = "snappy_stubs_public_h",
    srcs = [
        "snappy-stubs-public.h.in",
    ],
    outs = [
        "snappy-stubs-public.h",
    ],
    cmd = "sed 's/$${HAVE_SYS_UIO_H_01}/1/g' $(<) | " +
        "sed 's/$${PROJECT_VERSION_MAJOR}/1/g' | " +
        "sed 's/$${PROJECT_VERSION_MINOR}/1/g' | " +
        "sed 's/$${PROJECT_VERSION_PATCH}/9/g' >$(@)",
)

genrule(
    name = "config_h",
    srcs = [
        "cmake/config.h.in",
    ],
    outs = [
        "config.h",
    ],
    cmd = "sed 's/cmakedefine/define/g' $(<) | " +
        "sed 's/HAVE_WINDOWS_H 1/HAVE_WINDOWS_H 0/g' | " +
        "sed 's/SNAPPY_IS_BIG_ENDIAN 1/SNAPPY_IS_BIG_ENDIAN 0/g' | " +
        "sed '/HAVE_LIBLZO2/s/^/\\/\\//' | " +
        "sed '/HAVE_LIBZ/s/^/\\/\\//' | " +
        "sed '/HAVE_LIBLZ4/s/^/\\/\\//' | " +
        "sed '/SNAPPY_HAVE_SSSE3/s/^/\\/\\//' | " +
        "sed '/SNAPPY_HAVE_BMI2/s/^/\\/\\//' " +
        " >$(@)",
)

cc_library(
    name = "snappy",
    srcs = [
        "snappy-c.cc",
        "snappy-internal.h",
        "snappy-sinksource.cc",
        "snappy-stubs-internal.cc",
        "snappy-stubs-internal.h",
        "snappy.cc",
        ":config_h",
    ],
    hdrs = [
        ":snappy_stubs_public_h",
        "snappy.h",
        "snappy-c.h",
        "snappy-sinksource.h",
    ],
    includes = [
        "."
    ],
    copts = [
        "-DHAVE_CONFIG_H",
        "-Wno-sign-compare",
    ],
    deps = [
    ],
    visibility = ["//visibility:public"],
)
