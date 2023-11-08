load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

#cc_proto_library(
#    name = "proto",
#    srcs = glob(["proto/*.proto"]),
#    strip_include_prefix = "proto",
#    visibility = ["//visibility:public"],
#)

cc_binary(
    name = "main",
    srcs = glob([
        "src/*.cpp",
        "src/*.h",
    ]),
    copts = [
        "-g",
        "--std=c++17",
    ],
    linkopts = ["-lunwind"],
    deps = [
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)
