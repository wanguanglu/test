cc_library (
    name = "main",
    srcs = glob(
        ["src/*.cc"],
        exclude = ["src/gtest-all.cc"]
    ),
    
    hdrs = glob([
        "include/**/*.h",
        "src/*.h"
    ]),
    
    copts = [
        "-Iexternal/gtest/include",
        "-Iexternal/gtest/"
    ],
    
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
