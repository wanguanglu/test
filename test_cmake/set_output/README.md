# 换个地方保存目标二进制
不论是`SUBDIRS`还是`ADD_SUBDIRECTORY`指令(不论是否指定编译输出目录)，我们都可
以通过 SET 指令重新定义`EXECUTABLE_OUTPUT_PATH`和`LIBRARY_OUTPUT_PATH`变量
来指定最终的目标二进制的位置(指最终生成的 hello 或者最终的共享库，不包含编译生成
的中间文件)
```
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
```
在第一节我们提到了`<projectname>_BINARY_DIR`和`PROJECT_BINARY_DIR`变量，他
们指的编译发生的当前目录，如果是内部编译，就相当于`PROJECT_SOURCE_DIR`也就是
工程代码所在目录，如果是外部编译，指的是外部编译所在目录，也就是本例中的 build
目录。

所以，上面两个指令分别定义了：
可执行二进制的输出路径为 build/bin 和库的输出路径为 build/lib.

本节我们没有提到共享库和静态库的构建，所以，你可以不考虑第二条指令。
问题是，我应该把这两条指令写在工程的 CMakeLists.txt 还是 src 目录下的
CMakeLists.txt，把握一个简单的原则，在哪里`ADD_EXECUTABLE` 或`ADD_LIBRARY`，
如果需要改变目标存放路径，就在哪里加入上述的定义。
