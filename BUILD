load("@com_google_protobuf//:protobuf.bzl", "cc_proto_library")

#cc_proto_library(
#    name = "proto",
#    srcs = glob(["proto/*.proto"]),
#    strip_include_prefix = "proto",
#    visibility = ["//visibility:public"],
#)

cc_library(
    name = "query",
    srcs = ["src/query.cpp"],
    hdrs = ["src/query.h"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
    deps = [
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "main",
    srcs = [
        "src/index.h",
        "src/main.cpp",
    ],
    copts = [
        "-g",
        "--std=c++17",
    ],
    #    linkopts = ["-lunwind"],
    deps = [
        ":query",
        "@com_github_brpc_brpc//:butil",
        "@com_github_croaring//:roaring",
        "@com_github_croaring//:roaring-cpp",
        "@com_github_google_glog//:glog",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
    ],
)
