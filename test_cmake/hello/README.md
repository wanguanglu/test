# Hello
这个项目是一个基本的cmake项目。项目编译方式:
```sh
cd cmake
cmake ../
```

## 详细介绍
* CMakeLists.txt
```
project (Hello)
```
项目名称为Hello

```
add_subdirectory (hello)
```
添加`hello`子目录。

* hello/CMakeLists.txt
```
add_library (hello hello.cc)
```
添加`hello` lib库，依赖于`hello.cc`。

* demo/CMakeLists.txt
```
add_executable (hello_demo demo.cc)
```
添加`Demo`可执行程序，源代码文件为`demo.cc`。
