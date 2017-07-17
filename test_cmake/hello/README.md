# Hello
这个项目是一个基本的cmake项目。项目编译方式:
```sh
cd cmake
cmake ../
```

## 详细介绍
* CMakeLists.txt
```
project (HELLO)
```
项目名称为HELLO

```
add_subdirectory (Hello)
```
添加`Hello`子目录。

* Hello/CMakeLists.txt
```
add_library (Hello hello.cpp)
```
添加`Hello` lib库，依赖于`hello.cpp`。

* Demo/CMakeLists.txt
```
add_executable (helloDemo demo.cpp)
```
添加`Demo`可执行程序，源代码文件为`demo.cpp`。
