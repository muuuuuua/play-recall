package(default_visibility = ["//visibility:public"])

cc_library(
    name = "roaring",
    srcs = [
        "src/array_util.c",
        "src/bitset.c",
        "src/bitset_util.c",
        "src/containers/array.c",
        "src/containers/bitset.c",
        "src/containers/containers.c",
        "src/containers/convert.c",
        "src/containers/mixed_andnot.c",
        "src/containers/mixed_equal.c",
        "src/containers/mixed_intersection.c",
        "src/containers/mixed_negation.c",
        "src/containers/mixed_subset.c",
        "src/containers/mixed_union.c",
        "src/containers/mixed_xor.c",
        "src/containers/run.c",
        "src/isadetection.c",
        "src/memory.c",
        "src/roaring.c",
        "src/roaring_array.c",
        "src/roaring_priority_queue.c",
    ],
    hdrs = glob(
        [
            "include/**/*.h",
        ],
    ),
    strip_include_prefix = "include",
)

cc_library(
    name = "roaring-cpp",
    hdrs = glob(["cpp/*.hh"]),
    strip_include_prefix = "cpp",
)
