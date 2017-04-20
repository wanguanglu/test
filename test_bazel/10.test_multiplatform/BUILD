cc_binary (
    name = "multiplatform_bin",
    srcs = select({
        ":darwin_mode":  ["darwin.cc"],
        ":armeabi_mode": ["armeabi.cc"]
    })
)

config_setting(
    name = "darwin_mode",
    values = { "cpu": "darwin" }
)

config_setting(
    name = "armeabi_mode",
    values = { "cpu": "armeabi-v7a" }
)
