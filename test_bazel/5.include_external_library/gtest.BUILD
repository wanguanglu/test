cc_library (
    name = "main",
    srcs = glob(
        ["googletest-release-1.7.0/src/*.cc"],
        exclude = ["googletest-release-1.7.0/src/gtest-all.cc"]
    ),
    
    hdrs = glob([
        "googletest-release-1.7.0/include/**/*.h",
        "googletest-release-1.7.0/src/*.h"
    ]),
    
    copts = [
        "-Iexternal/gtest/googletest-release-1.7.0/include"
    ],
    
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)
