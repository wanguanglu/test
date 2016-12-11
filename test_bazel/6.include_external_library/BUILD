genrule(
  name = "hello",
  outs = ["hello_world.txt"],
  cmd  = "echo Hello World > $@",
)

cc_binary(
  name = "demo",
  srcs = ["demo.cc"],
  deps = ["@gtest//:main"],
)
