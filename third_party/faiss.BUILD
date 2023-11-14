package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # MIT

cc_library(
    name = "faiss",
    srcs = [
        "faiss/AutoTune.cpp",
        "faiss/Clustering.cpp",
        "faiss/IVFlib.cpp",
        "faiss/Index.cpp",
        "faiss/Index2Layer.cpp",
        "faiss/IndexAdditiveQuantizer.cpp",
        "faiss/IndexAdditiveQuantizerFastScan.cpp",
        "faiss/IndexBinary.cpp",
        "faiss/IndexBinaryFlat.cpp",
        "faiss/IndexBinaryFromFloat.cpp",
        "faiss/IndexBinaryHNSW.cpp",
        "faiss/IndexBinaryHash.cpp",
        "faiss/IndexBinaryIVF.cpp",
        "faiss/IndexFastScan.cpp",
        "faiss/IndexFlat.cpp",
        "faiss/IndexFlatCodes.cpp",
        "faiss/IndexHNSW.cpp",
        "faiss/IndexIDMap.cpp",
        "faiss/IndexIVF.cpp",
        "faiss/IndexIVFAdditiveQuantizer.cpp",
        "faiss/IndexIVFAdditiveQuantizerFastScan.cpp",
        "faiss/IndexIVFFastScan.cpp",
        "faiss/IndexIVFFlat.cpp",
        "faiss/IndexIVFPQ.cpp",
        "faiss/IndexIVFPQFastScan.cpp",
        "faiss/IndexIVFPQR.cpp",
        "faiss/IndexIVFSpectralHash.cpp",
        "faiss/IndexLSH.cpp",
        "faiss/IndexLattice.cpp",
        "faiss/IndexNNDescent.cpp",
        "faiss/IndexNSG.cpp",
        "faiss/IndexPQ.cpp",
        "faiss/IndexPQFastScan.cpp",
        "faiss/IndexPreTransform.cpp",
        "faiss/IndexRefine.cpp",
        "faiss/IndexReplicas.cpp",
        "faiss/IndexRowwiseMinMax.cpp",
        "faiss/IndexScalarQuantizer.cpp",
        "faiss/IndexShards.cpp",
        "faiss/MatrixStats.cpp",
        "faiss/MetaIndexes.cpp",
        "faiss/VectorTransform.cpp",
        "faiss/clone_index.cpp",
        "faiss/impl/AdditiveQuantizer.cpp",
        "faiss/impl/AuxIndexStructures.cpp",
        "faiss/impl/FaissException.cpp",
        "faiss/impl/HNSW.cpp",
        "faiss/impl/IDSelector.cpp",
        "faiss/impl/LocalSearchQuantizer.cpp",
        "faiss/impl/NNDescent.cpp",
        "faiss/impl/NSG.cpp",
        "faiss/impl/PolysemousTraining.cpp",
        "faiss/impl/ProductAdditiveQuantizer.cpp",
        "faiss/impl/ProductQuantizer.cpp",
        "faiss/impl/ResidualQuantizer.cpp",
        "faiss/impl/ScalarQuantizer.cpp",
        "faiss/impl/index_read.cpp",
        "faiss/impl/index_write.cpp",
        "faiss/impl/io.cpp",
        "faiss/impl/kmeans1d.cpp",
        "faiss/impl/lattice_Zn.cpp",
        "faiss/impl/pq4_fast_scan.cpp",
        "faiss/impl/pq4_fast_scan_search_1.cpp",
        "faiss/impl/pq4_fast_scan_search_qbs.cpp",
        "faiss/index_factory.cpp",
        "faiss/invlists/BlockInvertedLists.cpp",
        "faiss/invlists/DirectMap.cpp",
        "faiss/invlists/InvertedLists.cpp",
        "faiss/invlists/InvertedListsIOHook.cpp",
        "faiss/invlists/OnDiskInvertedLists.cpp",
        "faiss/utils/Heap.cpp",
        "faiss/utils/WorkerThread.cpp",
        "faiss/utils/distances.cpp",
        "faiss/utils/distances_simd.cpp",
        "faiss/utils/extra_distances.cpp",
        "faiss/utils/hamming.cpp",
        "faiss/utils/partitioning.cpp",
        "faiss/utils/quantize_lut.cpp",
        "faiss/utils/random.cpp",
        "faiss/utils/utils.cpp",
    ],
    hdrs = glob([
        "faiss/*.h",
        "faiss/**/*.h",
    ]),
    copts = [
        "-DFINTEGER=int",
        "-fopenmp",
    ],
    includes = ["."],
    linkopts = [
        "-fopenmp",
    ],
)

#load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")
#
#package(default_visibility = ["//visibility:public"])
#
#licenses(["notice"])  # MIT
#
#exports_files(["LICENSE"])
#
#filegroup(
#    name = "all_srcs",
#    srcs = glob(["**"]),
#)
#
#cmake(
#    name = "faiss",
#    generate_args = [
#        "-G Ninja",
#        "-DFAISS_ENABLE_GPU=OFF",
#        "-DFAISS_ENABLE_PYTHON=OFF",
#        "-DFAISS_ENABLE_C_API=OFF",
#        "-DBUILD_TESTING=OFF",
#        "-DCMAKE_BUILD_TYPE=Release",
#        "-DFAISS_OPT_LEVEL=general",
#    ],
#    lib_source = ":all_srcs",
#    out_static_libs = [
#        "libfaiss.a",
#    ],
#    targets = [
#        "faiss",
#    ],
#)
