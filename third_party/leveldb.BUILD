package(default_visibility = ["//visibility:public"])

config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
    visibility = ["//visibility:public"],
)

cc_library(
    name = "leveldb",
    srcs = glob(
        [
            "db/**/*.cc",
            "db/**/*.h",
            "helpers/**/*.cc",
            "helpers/**/*.h",
            "port/**/*.cc",
            "port/**/*.h",
            "table/**/*.cc",
            "table/**/*.h",
            "util/**/*.cc",
            "util/**/*.h",
        ],
        exclude = [
            "**/*_test.cc",
            "**/testutil.*",
            "**/*_bench.cc",
            "**/*_windows*",
            "db/leveldbutil.cc",
        ],
    ),
    hdrs = glob(
        [
            "helpers/memenv/*.h",
            "util/*.h",
            "port/*.h",
            "port/win/*.h",
            "table/*.h",
            "db/*.h",
            "include/leveldb/*.h",
        ],
        exclude = [
            "**/*test.*",
        ],
    ),
    copts = [
        "-fno-builtin-memcmp",
        "-DLEVELDB_PLATFORM_POSIX=1",
        "-DLEVELDB_ATOMIC_PRESENT",
    ],
    defines = [
        "LEVELDB_PLATFORM_POSIX",
    ] + select({
        ":darwin": ["OS_MACOSX"],
        "//conditions:default": [],
    }),
    includes = [
        "include/",
    ],
)
