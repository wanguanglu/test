# 第一章. Boost.Log v2
Andrey Semashev（万广鲁翻译）

	
目录
	
* [动机](#motivation)
* [如何阅读本文档](#how-to-read-this-document)
* [安装和兼容性](#install)
	* [支持的编译器和平台](#supported-compilers-and-platforms)
	* [配置和构建本程序库](#configuring-and-building-the-library)



## <a name="motivation"></a>动机
&emsp;&emsp;当前应用程序快速发展，越来越复杂，非常难以测试和调试。应用程序大部分时间运行在远端的服务器上，让开发者难以监控他们的运行情况，而且在出现问题时，难以找出运行失败的原因。即使对于本地程序，程序的运行情况严重依赖于异步的事件，调试也越来越困难。比如很多程序依赖于设备的反馈或者其他线程的运行情况。

&emsp;&emsp;这就是日志能够帮助的地方，应用程序保存运行信息到日志中，当出现问题时，可以根据日志来分析错误原因。日志还有很多用途，比如收集统计信息和一些重要的事件（比如出现了某些情况或者异常），这些对于工业界的实际应用都至关重要。

&emsp;&emsp;本程序库目标让日志输出对于开发者来说方便快捷，它提供了很多直接可用的工具，以及一些可以实现的公共接口，本程序库的主要目标是

* 简单&emsp;通过一些简单的示例代码就可以了解本程序的的基本特征
* 可扩展&emsp;用户可以通过扩展本程序库的功能来收集和存储信息到日志中
* 性能&emsp;本程序库应该尽可能少地影响到用户的应用程序

## <a name="how-to-read-this-document"></a>如何阅读本文档
&emsp;&emsp;本文档同时面向新老读者，但是我们期望用户对于常用的boost组件比较熟悉，比如```shared_ptr```,```make_shared```，这些信息可以参见[Boost.SmartPtr](http://www.boost.org/doc/libs/release/libs/smart_ptr/smart_ptr.htm)，本文档中的部分文档会参考boost中的其它程序库的文档。

&emsp;&emsp;如果你是第一次使用本程序库，推荐先阅读[设计概要](#design-overview)小节，对本程序库的功能和架构有一个基本的了解，通过阅读[安装](#install)和[教程](#tutorial)章节的内容可以实验本程序库。在教程章节中通过实际代码来介绍本程序库的功能，一些教程分为初级和高级来分别介绍，初级形式介绍最通用和简单易用的方式来进行工作，我们推荐初学者这么使用。高级版本介绍延伸的方式来实现同样的事情，但是在深度上进行扩展，同时进行了一些客户化。这种方式适合有经验的开发者来阅读，如果初级版本无法满足需要的话。

&emsp;&emsp;在教程之外，[功能细节介绍](#detailed-feature-description)章节，介绍了本程序库提供的其它工具，这一部分最好按照案例来进行阅读。

&emsp;&emsp;最后会有一个详细介绍本程序库组件的参考书。

&emsp;&emsp;为了是的本文档更加简洁，我们对一些命名空间定义了一些别名
```
namespace logging = boost::log;
namespace sinks = boost::log::sinks;	
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;
```

## <a name="install"></a>安装和兼容性
* [支持的编译器和平台](#supported-compilers-and-platforms)
* [配置和构建本程序库](#configuring-and-building-the-library)

### <a name="supported-compilers-and-platforms"></a>支持的编译器和平台
&emsp;&emsp;本程序库需要在一些可兼容的编译器下进行构建和工作，本程序库在以下的平台上进行了成功的构建和测试。
* Windows XP, Windows Vista, Windows 7. MSVC 8.0 SP1, MSVC 9.0或者更新版本
*  Linux. GCC 4.5或者更新版本，老版本可能也可以工作，但是我们没有进行测试。
* Linux. Intel C++ 13.1.0.146 Build 20130121.
* Linux. Clang 3.2或者更新版本

&emsp;&emsp;以下的编译器和平台是不支持的，可能会编译失败
*  C++11编译器，但是含有非C++11标准库(例如从GCC 4.2来的Clang libstdc++) 。在C++11模式下请使用C++11的标准库。
* MSVC 8.0(没有SP1)或者更老版本。
* GCC 4.2或者更老版本。
* Borland C++ 5.5.1(免费版)。新版本可能可以工作也可能不可以工作。
* Solaris Studio 12.3或者更老版本
* Windows 9x, ME, NT4, 2000或者更老版本是不支持的。

&emsp;&emsp;Boost.Log兼容所有Boost支持的硬件架构，但是在32位x86架构下，需要至少i586 CPU来运行。

#### *GCC用户须知*
&emsp;&emsp;自GCC 4.5开始支持链接时间优化(Link time optimazation, LTO)，大部分优化和二进制代码在链接阶段生成。这就允许更加高级的优化并生成更快的代码。不幸的是，当工程中包含在不同的编译选项时，它的效率并不高，Boost.Log就是这种工程，在工程中部分代码包含对于流行CPU的优化，不能在老版本的CPU上运行。如果开启LTO，将导致生成的二进制代码对于老版本CPU不兼容(GCC [bug](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=61043))，可能会导致运行时崩溃。这个问题在GCC 5.1中被修复，因此对于GCC 5.1之前的版本，不支持LTO选项。

&emsp;&emsp;在开启-march=native命令行选项时，可能会触发GCC [bug](http://gcc.gnu.org/bugzilla/show_bug.cgi?id=60607)导致编译失败。推荐避免使用-march=native(或instruction-set=native bjam属性)选项以及显式的指定目标CPU(例如instruction-set=sandy-bridge)。

#### *MinGW, Cygwin and Visual Studio Express Edition用户须知*
&emsp;&emsp;要在这些编译器下进行编译，需要进行一些特殊的准备。

&emsp;&emsp;首先，在MinGW或者Cygwin下，一定要保证安装了最新的GCC版本。本程序库在GCC 3.x下很可能会编译失败。

&emsp;&emsp;其次，在一些时候，本程序库需要一个消息编译工具(mc.exe)，在MinGW，Cygwin以及一些版本的MSVC Express Edition中并没有提供。通常，本程序库的构建脚本会自动检测消息编译工具是否存在。如果检测不存在的话，讲关闭事件日志相关的部分。如果需要事件日志而在系统中有没有找到消息编译工具的话，你有三种选择来解决这个问题。推荐的解决方案是获取原始的mc.exe，这个工具在Windows SDK中提供。勀有在微软的网站上免费下载Windows SDK(比如[这里](http://www.microsoft.com/downloads/details.aspx?FamilyID=71deb800-c591-4f97-a900-bea146e4fae1&displaylang=en))。这个工具在Visual Studio 2010 Express Edition中是提供的。在编译时，mc.exe需要在环境变量`PATH`指定的文件夹中能够找到。

&emsp;&emsp;另一中方案是尝试使用MinGW和Cygwin发布的windmc.exe工具，它是一个模拟原始mc.exe的工具。如果你想这样做，你需要打包在[此便签](https://svn.boost.org/trac/boost/ticket/4111)中介绍的Boost.Build文件(明确来说，是tools/build/tools/mc.jam文件)，然后你可以在bjam中使用 mc-compiler=windmc来构建本程序库。

&emsp;&emsp;在某些情况下，消息编译工具检测失败，你可以显示的取消时间日志后端的支持，通过在构建本程序库时定义`BOOST_LOG_WITHOUT_EVENT_LOG`配置宏。这样会去除对于消息编译器的需要，请查看[这一小节](#configuring-and-building-the-library)来了解更多的配置选项。

&emsp;&emsp;Windows XP的MinGW用户可能会被msvcrt.dll中的[bug](http://sourceforge.net/p/mingw-w64/bugs/307/)影响，这个动态链接库是操作系统自带的，这个bug的现象是自身在结构话日志输出时会崩溃。这个问题不仅仅在Boost.Log会出现，在一些其他的本地化以及输入输出流管理的情景下都有可能出现。

#### *Cygwin用户的额外须知*
Cygwin的支持非常初步，Cygwin中默认的GCC版本是4.5.3（编写此文档时），是无法编译此程序库的。你必须构建一个更新版本的GCC，即使这样一些Boost.Log的功能还是无法使用。特别指出，socket相关的系统日志后端是不支持的。因为它是基于Boost.ASIO的，但是Boost.ASIO在此平台上无法编译。但是本地的系统日志支持是可以工作的。

### <a name="configuring-and-building-the-library"></a>配置和构建本程序库
本程序库有一个的单独构建部分，已经在[初始引导](http://www.boost.org/doc/libs/release/more/getting_started/)中介绍。但是，有一件事情需要注意，如果你的应用包含不止一个模块（例如一个exe文件包含多个dll）使用Boost.log，本程序库必须被构建成共享对象。如果你只有一个可执行程序或者一个模块使用Boost.log，你可以将本程序库构建成静态库。

本程序库支持许多配置宏

**表格 1.1. 配置宏**

|宏名称          |影响             |
|:------------- |:----------------| 
|BOOST_LOG_DYN_LINK|如果在用户程序的定义，本程序库会假定这个二进制会被构建成一个动态加载库("dll"或者"so")，否则会假定这个程序被静态构建，这个宏必须在用户所有的应用程序中，要么都定义，要么都不定义，在自动链接的情况下该宏会有帮助|
|BOOST_ALL_DYN_LINK|和BOOST_LOG_DYN_LINK一样，但是会影响其他的Boost库|
|BOOST_USE_WINAPI_VERSION|会影响编译程序库和用户的程序，这个宏是windows环境特有的，选定一个目标的Windows版本下的Boost库，包括Boost.Log，程序会在指定的windows版本下编译有可能会在老版本的Windows环境下失败。由于使用了新版本的操作系统特性，有可能会提升性能。这个宏有一个整型值等效于[_WIN32_WINNT](https://msdn.microsoft.com/en-us/library/6sehtctf.aspx)|
|BOOST_LOG_NO_THREADS|如果定义，则取消对多线程的支持。对程序库和用户程序的编译都有影响，如果检测到不支持多线程，则此宏会被自动定义|
|BOOST_LOG_WITHOUT_CHAR|如果定义，对窄字节字符日志的支持失效，对程序库和用户程序的编译都有影响|
|BOOST_LOG_WITHOUT_WCHAR_T|如果定义，对宽字节字符日志的支持失效，对程序库和用户程序的编译都有影响|
|BOOST_LOG_NO_QUERY_PERFORMANCE_COUNTER|此宏只对Windows有效，对程序库和用户程序的编译都有影响。如果定义，则对timer属性中的QueryPerformanceCounter API支持失效。会导致时间读取准确性降低。此宏是为了解决一些早期AMD速龙CPU中可能存在的问题。在[链接1](http://support.microsoft.com/?scid=kb%3ben-us%3b895980)和[链接2](http://support.microsoft.com/?id=896256)中都有介绍。还有一些已知的芯片组失败会阻止此API运行正常。（参考[此链接](http://support.microsoft.com/kb/274323))。|
|BOOST_LOG_USE_NATIVE_SYSLOG|只影响程序库的编译，如果由于一些原因没有自动检测到本地系统日志API的支持，可以定义此宏强行使其生效。|
|BOOST_LOG_WITHOUT_DEFAULT_FACTORIES|只影响程序库的编译。如果定义，会构建一个没有初始过滤器和格式化工厂的设置解析器。用户必须在解析过滤器和格式化字符串之前注册所有属性，这样会大幅降低程序库的大小。|
|BOOST_LOG_WITHOUT_SETTINGS_PARSERS|只影响程序库的编译。如果定义，所有解析设置类的功能都不会被构建，这样会大幅降低程序库的大小。|
|BOOST_LOG_WITHOUT_DEBUG_OUTPUT|只影响程序库的编译。如果定义，对与在Windows上调试输出只是将不会被构建。|
|BOOST_LOG_WITHOUT_EVENT_LOG|只影响程序库的编译。如果定义，对于Windows时间日志支持不会被构建，定义此宏会使得信息编译工具集不再必要。|
|BOOST_LOG_WITHOUT_SYSLOG|只影响程序库的编译。如果定义，对于系统日志后端(syslog backend)的支持将失效。|
|BOOST_LOG_NO_SHORTHAND_NAMES|只影响程序库的编译。如果定义，一些过时的速记宏名称将失效。|
|BOOST_LOG_USE_COMPILER_TLS|只影响程序库的编译。此宏使对于编译器线程内存储内敛函数生效。在一些使用限制可以接受的情况下，定义此宏可能会提升Boost.Log的性能。在下面可以看到更详细的解释|
|BOOST_LOG_USE_STD_REGEX,<br> BOOST_LOG_USE_BOOST_REGEX or <br> BOOST_LOG_USE_BOOST_XPRESSIVE|只影响程序库的编译。|











## <a name="design-overview"></a>设计概要

## <a name="tutorial"></a>教程




















## <a name="detailed-feature-description"></a>功能细节介绍
