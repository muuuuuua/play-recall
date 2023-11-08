workspace(name = "lianxi")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "bazel_skylib",
    strip_prefix = "bazel-skylib-1.3.0",
    urls = ["https://github.com/bazelbuild/bazel-skylib/archive/1.3.0.tar.gz"],
)

http_archive(
    name = "rules_foreign_cc",
    sha256 = "1df78c7d7eed2dc21b8b325a2853c31933a81e7b780f9a59a5d078be9008b13a",
    strip_prefix = "rules_foreign_cc-0.7.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.7.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

http_archive(
    name = "rules_perl",  # 2021-09-23T03:21:58Z
    sha256 = "55fbe071971772758ad669615fc9aac9b126db6ae45909f0f36de499f6201dd3",
    strip_prefix = "rules_perl-2f4f36f454375e678e81e5ca465d4d497c5c02da",
    urls = [
        "https://github.com/bazelbuild/rules_perl/archive/2f4f36f454375e678e81e5ca465d4d497c5c02da.tar.gz",
    ],
)

load("@rules_perl//perl:deps.bzl", "perl_register_toolchains")

perl_register_toolchains()

git_repository(
    name = "com_google_protobuf",
    remote = "https://github.com/protocolbuffers/protobuf.git",
    tag = "v3.21.12",
)

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")

protobuf_deps()

git_repository(
    name = "com_github_mkl",
    commit = "40fc9dcd9c0aaf54b21f1ab01215ce85a6fafa85",
    remote = "https://github.com/muuuuuua/mkl.git",
)

http_archive(
    name = "com_github_facebookresearch_faiss",
    build_file = "//third_party:faiss.BUILD",
    sha256 = "a6a6f331be9e7bfea08ce1654519411904c656847a00a52de0d54d8e9895b629",
    strip_prefix = "faiss-1.7.3",
    url = "https://github.com/facebookresearch/faiss/archive/v1.7.3.zip",
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = [
        "https://github.com/gflags/gflags/archive/v2.2.2.tar.gz",
    ],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "8a83bf982f37bb70825df71a9709fa90ea9f4447fb3c099e1d720a439d88bad6",
    strip_prefix = "glog-0.6.0",
    url = "https://github.com/google/glog/archive/refs/tags/v0.6.0.tar.gz",
)

http_archive(
    name = "com_github_google_leveldb",
    build_file = "//third_party:leveldb.BUILD",
    sha256 = "9a37f8a6174f09bd622bc723b55881dc541cd50747cbd08831c2a82d620f6d76",
    strip_prefix = "leveldb-1.23",
    urls = [
        "https://github.com/google/leveldb/archive/refs/tags/1.23.tar.gz",
    ],
)

http_archive(
    name = "com_google_googletest",
    sha256 = "ad7fdba11ea011c1d925b3289cf4af2c66a352e18d4c7264392fead75e919363",
    strip_prefix = "googletest-1.13.0",
    url = "https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz",
)

http_archive(
    name = "com_github_google_snappy",
    build_file = "//third_party:snappy.BUILD",
    sha256 = "75c1fbb3d618dd3a0483bff0e26d0a92b495bbe5059c8b4f1c962b478b6e06e7",
    strip_prefix = "snappy-1.1.9",
    url = "https://github.com/google/snappy/archive/refs/tags/1.1.9.tar.gz",
)

http_archive(
    name = "com_github_facebook_rocksdb",
    build_file = "//third_party:rocksdb.BUILD",
    sha256 = "886378093098a1b2521b824782db7f7dd86224c232cf9652fcaf88222420b292",
    strip_prefix = "rocksdb-7.9.2",
    urls = [
        "https://github.com/facebook/rocksdb/archive/refs/tags/v7.9.2.tar.gz",
    ],
)

# bRPC cannot use boringssl. Build openssl.
http_archive(
    name = "openssl",  # 2021-12-14T15:45:01Z
    build_file = "//third_party:openssl.BUILD",
    sha256 = "f89199be8b23ca45fc7cb9f1d8d3ee67312318286ad030f5316aca6462db6c96",
    strip_prefix = "openssl-1.1.1m",
    urls = [
        "https://www.openssl.org/source/openssl-1.1.1m.tar.gz",
        "https://github.com/openssl/openssl/archive/OpenSSL_1_1_1m.tar.gz",
    ],
)

http_archive(
    name = "com_github_madler_zlib",  # 2017-01-15T17:57:23Z
    build_file = "//third_party:zlib.BUILD",
    sha256 = "c3e5e9fdd5004dcb542feda5ee4f0ff0744628baf8ed2dd5d66f8ca1197cb1a1",
    strip_prefix = "zlib-1.2.11",
    urls = [
        "https://downloads.sourceforge.net/project/libpng/zlib/1.2.11/zlib-1.2.11.tar.gz",
        "https://zlib.net/fossils/zlib-1.2.11.tar.gz",
    ],
)

git_repository(
    name = "com_github_brpc_brpc",
    remote = "https://github.com/apache/incubator-brpc.git",
    tag = "1.7.0",
)

git_repository(
    name = "com_github_brpc_braft",
    remote = "https://github.com/baidu/braft.git",
    tag = "v1.1.2",
)

git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp.git",
    tag = "20230125.3",
)
