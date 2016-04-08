# 第一章. Boost.Log v2
Andrey Semashev（万广鲁翻译）


目录

* [动机](#motivation)
* [如何阅读本文档](#how-to-read-this-document)
* [安装和兼容性](#install)
	* [支持的编译器和平台](#supported-compilers-and-platforms)
	* [配置和构建本程序库](#configuring-and-building-the-library)
* [定义](#definition)
* [设计概要](#design-overview)
* [教程](#tutorial)
	* [Trivial logging](#trivial-logging)
	* [包含过滤器的Trivial logging](#trivial-logging-with-filters)
	* [建立sink](#set-up-sink)


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

&emsp;&emsp;在教程之外，[详细特征介绍](#detailed-feature-description)章节，介绍了本程序库提供的其它工具，这一部分最好按照案例来进行阅读。

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
|BOOST_LOG_USE_COMPILER_TLS|只影响程序库的编译。此宏使对于编译器线程本地存储内联函数生效。在一些使用限制可以接受的情况下，定义此宏可能会提升Boost.Log的性能。在下面可以看到更详细的解释|
|BOOST_LOG_USE_STD_REGEX,<br> BOOST_LOG_USE_BOOST_REGEX or <br> BOOST_LOG_USE_BOOST_XPRESSIVE|只影响程序库的编译。通过定义这些宏可以指示Boost.Log使用std::regex，[Boost.Regex][boost_log]，[Boost.Xpressive][boost_xpressive]来进行字符串匹配过滤解析。如果上述都没有定义，Boost.Log会默认使用[Boost.Regex][boost_regex]，使用std::regex或者[Boost.Regex][boost_regex]会使得生成的可执行程序更小，[Boost.Regex][boost_regex]在运行时更快，使用[Boost.Xpressive][boost_xpressive]可以避免对[Boost.Regex][boost_regex]的编译依赖。这些宏不影响用户创建的[过滤器表达式](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/log/detailed/expressions.html#log.detailed.expressions.predicates.advanced_string_matching)。|

[boost_regex]: http://www.boost.org/doc/libs/release/libs/regex/index.html
[boost_xpressive]: http://www.boost.org/doc/libs/release/doc/html/xpressive.html
[boost_thread]: http://www.boost.org/doc/libs/release/doc/html/thread.html
[boost_asio]: http://www.boost.org/doc/libs/release/doc/html/boost_asio.html
[boost_phoenix]: http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/index.html

&emsp;&emsp;你可以在bjam命令行中定义配置宏，像下面这样：
``` sh
bjam --with-log variant=release define=BOOST_LOG_WITHOUT_EVENT_LOG define=BOOST_USE_WINAPI_VERSION=0x0600 stage
```

&emsp;&emsp;在头文件"/boost/config/user.hpp"文件中定义配置宏会更加方便一些，这样会同时影响程序库和用户的工程。如果这些所有选项都没有定义，程序库会只是最全面的安装，支持目标平台所有可用的的字节类型和特征。

&emsp;&emsp;本日志程序库使用了几个其他的Boost库，这些也需要进行构建，包括[Boost.Filesystem](http://www.boost.org/doc/libs/release/libs/filesystem/doc/index.htm)，[Boost.System](http://www.boost.org/doc/libs/release/libs/system/doc/index.html)，[Boost.DateTime](http://www.boost.org/doc/libs/release/doc/html/date_time.html)，[Boost.Thread][boost_thread]以及[Boost.Regex][boost_regex]。通过参考他们的文档可以了解在构建过程中更细节的指令。

&emsp;&emsp;在最后需要说的是，此程序库需要实时类型信息（run-time type information, RTTI)，这样会同时影响本程序库以及用户程序的编译。正常来说，你不需要做什么，除非在你的工程中RTTI未被禁止。

#### *编译器提供的支持TLS内联函数须知*
&emsp;&emsp;许多广泛应用的编译器支持内敛函数来管理线程本地存储（Thread Local Storage，TLS，可以参考[链接](http://www.cppblog.com/Tim/archive/2012/07/04/181018.html)），这些被应用在本程序库的许多地方。这个特性也包含在C++11中作为标准。通常来说，这些内联函数比任何代理实现方式对内存更有效的管理。比如[Boost.Thread][boost_thread]甚至本地操作系统API。然而使用TLS需要注意以下几点

* 当TLS定义在动态库，在应用程序运行时动态加载时，使用这些内联函数，在一些操作系统中并不支持。这些操作系统包括Linux，Vista之前Windows的版本。Windows Vista以及以后的版本不存在这个问题。
* TLS可能不能被全局的构造和析构可靠地访问。至少在Windows环境中MSVC 8.0存在此问题。

&emsp;&emsp;本程序库提供```BOOST_LOG_USE_COMPILER_TLS```配置宏，运行使用此特征，来提升程序库的性能。但是必须在以下前提下

* 此应用程序必须静态链接Boost.Log库，不能是运行时动态加载。
* 此应用程序必须不能在全局的构造和析构中使用logging。

&emsp;&emsp;需要注意的是，```BOOST_LOG_USE_COMPILER_TLS```宏仅仅控制Boost.Log中TLS的应用，但无法控制Boost.Log使用的其他的程序库。比如，[BOOST.ASIO][boost_asio]默认使用编译器提供的TLS。为了完全自由地构建Boost.Log程序库，不使用编译器提供的TLS，在其他程序库中也将此特性的设置为关闭。（在[BOOST.ASIO][boost_asio]中，可以通过在构建过程中定义BOOST_ASIO_DISABLE_THREAD_KEYWORD_EXTENSION宏来达到此目的）。

#### *使用本地wchar_t支持须知*
&emsp;&emsp;一些编译器，尤其是MSVC，有一些选项来关闭本地wchar_t类型，通过类型定义一个整型来模仿wchar_t。这种做法并不太遵守从C++语言的一些规范，但是可以兼容一些古老的难以更新的代码。

&emsp;&emsp;默认情况下，Boost（以及Boost.Log作为其中一部分）在构建中将wchar_t设置为打开状态。在编写本文档时，用户必须更改Boost.Build来开启模拟模式。可以在此模式下构建Boost.Log。但是有一些注意事项必须牢记

* 编译之后的Boost.Log会根据构建时的配置输出一些符号，用户代码必须使用同样的设置，否则会残生链接错误。
* 在模拟模式下，wchar_t和整型是无法区分的。此程序库的一些部分在正常模式和模拟模式下会有不同的表现。特别是宽字节文字会被拒绝或者格式化成另外的形式。
* 模拟模式没有进行测试，有可能会有意想不到的错误。

&emsp;&emsp;因此，使用模拟模式是不被孤立的，而且应该避免。在未来的发布版本中，对于模拟模式的支持有可能会被完全删除掉。

### <a name=definition></a>定义
&emsp;&emsp;此处一些术语的定义，这些定义在此文档中会被广泛应用

**日志记录 Log record**

&emsp;&emsp;从用户的应用程序中收集的一个信息簇，是被输出到日志的候选信息。在简单的情况下，一个日志记录经过日志程序库处理之后表示为日志文件中的一行文本。

**属性 attribute**

&emsp;&emsp;一个属性是一条元数据，用于表示一个日志记录。在Boost.Log中，用一些有具体接口的函数对象来表示属性，在调用的时候返回真实的属性值。

**属性值**

&emsp;&emsp;属性值是从属性中获取的真实数据。这些数据依附于一条特定的日志记录，程序库会进行处理这些属性值。属性值有可能有不同的类型（整型，字符串类型或者更加复杂的类型，包括用户自定义类型）。一些属性值的示例包括：当前时间戳、文件名、行号、当前范围名称等等。属性值包装在一个包装器中，其真实类型在接口中是不可见的。值的真实类型有时被称作存储类型。

**(属性)值访问**

&emsp;&emsp;一种处理属性值的方式。这种方法调用一个应用于属性值的函数对象(访问者）。为了处理此属性值，这个访问者需要知道属性值的存储类型。

**(属性)值提取**

&emsp;&emsp;当调用者试图得到一个存储值的引用时，处理属性值的方式。为了提取属性值，调用者应当知道属性值的存储类型。

**Log sink**

&emsp;&emsp;将所有的用户应用程序收集到的日志记录输出到的目标。sink定义了这些日志记录被怎么处理以及被存储到哪儿。

**日志源Log source**

&emsp;&emsp;用户应用程序的日志记录的输入点。在一个简单的示例中，一个日志对象保持一套属性，用户根据需要将这些属性组成一个日志记录。当然，用户可以创建一个source，从其他的事件中获取信息，并产生日志记录。例如，通过截流和解析其他应用程序的显示器输出。

**日志过滤器Log filter**

&emsp;&emsp;用于判断一个日志记录是否应该通过或者被丢弃。日志过滤器通常需要根据日志的属性值类决策是否需要通过或丢弃。

**日志格式化工具Log formatter**

&emsp;&emsp;一个生成日志记录最终文本输出格式函数对象。一些sink，例如二进制日志sink也许不需要它。但是大部分基于文本的sink会需要一个格式化工具来组合它的输出。

**日志核心Logging core**

&emsp;&emsp;一个全局实体，保持了源和sink之间的连接，同时对记录提供过滤器。主要在日志程序库初始化的时候使用。

**国际化 i18n**

&emsp;&emsp;国际化，操纵宽字节的能力。

**线程本地存储 TLS**

&emsp;&emsp;线程本地存储，对于不同的线程访问一个变量时，其值是相互独立的。

**实时类型信息RTTI**

&emsp;&emsp;实时类型信息。这个是一个C++支持的数据结构。需要通过```dynamic_cast```和```typeid```来正常工作。

## <a name="design-overview"></a>设计概要

&emsp;&emsp;Boost.Log设计得非常模块化而且易于扩展。同时支持窄字节和宽字节日志。宽字节和窄字节日志提供相似的功能，因此在此文档大部分的情况下，仅仅介绍窄字节的接口。本程序库包括三个主要的层：

* 数据收集层
* 数据处理层
* 中央枢纽层，将上面两层连接起来。

&emsp;&emsp;整体设计如下图所示

![boost-log-architechture](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/images/log/Design.png)

&emsp;&emsp;箭头表示了日志信息流的方向。从用户的应用程序端，到最终的存储端。存储是可选的，因为有的时候日志处理结果可能包含一些动作并没有真正存储这些信息。例如，如果你的应用程序处于临界状态，它可能产生特殊的日志记录。这些日志通过处理，通过系统通知抖动以及声音来提醒用户。此程序库一个非常重要的特性是，收集、处理以及日志数据有哪些信息都是相互独立的。这样将允许使用此程序库不仅仅用于传统的日志。还包括指示重要的事件给应用程序用户，以及积累统计数据。

#### 日志源
&emsp;&emsp;回到这张图。在左侧，应用程序输出日志记录。通过一些logger对象，提供流来格式化信息，这些信息最终被传输的日志中。本程序库提供了一些不同的logger类型，同时你也可以精心设计其他的logger类型，来扩展现有类型。logger被设计成一些不同特征的组合。你可以开发自己的特征，然后将其添加到池子中。你可以像使用函数式编程一样使用结构化的logger。通常这样比使用一个全局的logger更加方便。

&emsp;&emsp;总体来说，此程序库不需要logger来写日志。通用术语“日志源”指向一个实体，这个实体通过创建一个日志记录来启动日志过程。其他的日志源可能抓取子程序的屏幕输出或者从网络上获取的数据。然而logger是最通用的日志源。

#### 属性和属性值
&emsp;&emsp;为了启动日志，一个日志源必须将所有与此日志记录有关的数据传输到日志核心logging core。这些数据，更准确来说，是这些数据的获取方式是通过一些属性来描述的。总的来说，每一个属性是一个函数，函数的结果是“属性值”，这些数据在未来阶段会被真正处理。举一个例子，返回当前时间就可以作为一个属性。其返回结果就是确切的当前时间。

&emsp;&emsp;有三种属性类型

* 全局属性
* 线程相关属性
* 源相关属性

&emsp;&emsp;你可以从这张图中看到，全局属性和线程相关属性，通过logging core来保持，因此不需要在初始化时通过日志源来传输。全局属性会依附于所有的日志记录。很显然，线程相关属性是线程产生的，依附在当前线程的日志记录上。源相关的属性，是在启动日志过程时，由源产生的，这些属性值依附于特定源产生的日志记录。

&emsp;&emsp;当一个源启动日志过程，从以上三个属性集合中获取属性值。然后，将这些属性值组成一个属性集合，在后续进行处理。你可以往属性集合中增加更多的属性值，新增的属性值只依附于特定的日志记录。并且于日志源和日志核心没有关联。在不同的属性结合中可能会出现同名的属性，这些冲突的优先级处理准则是：全局属性有最低的优先级，源相关的属性有最高的优先级。当出现冲突的时候，低优先级属性将被丢弃。

#### 日志核心和过滤
&emsp;&emsp;当组合属性值集合之后，日志核心判断这条日志记录是否继续被sink处理。这就被称为过滤。其中包含两层过滤器：

* 全局过滤器&emsp;首先在日志核心中实施全局过滤器，允许快速擦去不需要的日志记录。
* sink相关的过滤器&emsp;其次实施sink过滤器，sink过滤器允许日志记录到特定的sink。需要注意的是，日志记录是通过哪个源产生的并不重要，过滤器仅仅通过依附于日志记录上属性值集合来进行过滤。

&emsp;&emsp;需要提到的是，对于一个指定的日志记录，过滤仅仅实施一次。显然，只有在过滤之前产生的属性可以参与过滤。一些属性值，比如日志记录的message信息，一般在过滤之后才依附到日志记录中。这些属性值在过滤时是不会被用到的，你只能在sink和格式化是使用。

#### sink和格式化
&emsp;&emsp;如果一个日志记录通过了过滤，至少一个sink是可以处理此日志记录的。如果此sink支持结构化输出，在当前点会发生结构化的操作。结构化之后的信息以及属性值组合被传输到接收此日志记录的sink。需要注意的是结构化是发生在pre-sink阶段，因此每个sink可以将日志记录按照特定格式输出。

&emsp;&emsp;sink包含两部分，前端和后端。之所以如此区分，是为了提取sink的通用功能。比如过滤、格式化以及线程同步到独立的实体（前端）中。本程序库提供sink前端，用户基本上不用重新实现他们。后端，则相反，是程序库中最可能进行扩展的地方。sink后端是真正进行处理日志记录的地方。可能有一个sink将一条日志记录存储到文件中，有一个sink将日志记录通过网络发送到远端处理节点，也可能有sink，像之前提到的，将记录信息发送到桌面通知。大部分通用的sink在程序库中已经提供。

&emsp;&emsp;除了一些上述的基础设施，本程序库还提供各种各样的辅助工具，例如属性、格式化工具以及过滤器，表现为lambda表达式。甚至一些程序库初始化基本帮助。你会在[详细特征描述](#detailed-feature-description)小节学习到这些知识。不过对于初学者来说，推荐从[教程](#tutorial)小节开始学习。

## <a name="tutorial"></a>教程

* [Trivial logging](#trivial-logging)
* [包含过滤器的Trivial logging](#trivial-logging-with-filters)
* [建立sink](#set-up-sink)

&emsp;&emsp;在本小结中，我们会把一些基本的步骤走一遍，来熟悉此程序库。在阅读完本节信息之后，你应该能够开始使用此程序库，并在自己的应用程序中打印日志。本教程中的示例代码都可以在```libs/log/examples```文件夹中获取，可以随意地编译并查看运行结果。

### <a name="trivial-logging"></a>Trivial logging
&emsp;&emsp;对于不想阅读复杂的使用手册，仅仅需要一些简单的日志功能，以下代码就可以实现
```csharp
#include <boost/log/trivial.hpp>

int main(int, char*[])
{
    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return 0;
}
```
[查看完成代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_trivial.cpp)

&emsp;&emsp;BOOST_LOG_TRIVIAL宏接收严重程度参数，会产生一个类似于流的对象，支持插入操作符。上述代码的结果，会让日志信息打印到屏幕上。此程序库的使用方式和std::cout的使用方式非常类似。然而，此程序库提供了一些优势：

1. 除去记录信息之外，每个输出的日志记录包含一个时间戳，当前线程的id以及日志的严重程度。
2. 不同的线程同时写日志是安全的，日志信息不会错误。
3. 后面会讲到，可以添加过滤器。

&emsp;&emsp;必须要讲的是，此宏和本程序库中提供的其他宏类似，不是仅有的使用此程序库的接口方式。可以不使用任何宏就能够完成日志的记录。

### <a name="trivial-logging-with-filters"></a>包含过滤器的Trivial logging
&emsp;&emsp; 可以通过日志的严重程度来区分信息，我们会希望使用一个过滤器来仅输出有意义的记录并忽略其他的记录。通过在程序库核心中设置一个全局的过滤器可以非常方便的实现此功能。就像这样：
```csharp
void init()
{
    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}

int main(int, char*[])
{
    init();

    BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
    BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
    BOOST_LOG_TRIVIAL(info) << "An informational severity message";
    BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
    BOOST_LOG_TRIVIAL(error) << "An error severity message";
    BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";

    return 0;
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_trivial_flt.cpp)

&emsp;&emsp;现在，如果你运行上述示例程序，前两行日志记录会被忽略，剩下的四条记录会输出到显示器上。

![important](http://www.boost.org/doc/libs/1_60_0/doc/src/images/important.png)**重要**

	需要记住流表达式只有在记录通过过滤器之后才会执行。不要在流表达式中指定关键业务调用。因为当记录被过滤掉之后，这些调用可能没有被执行。

&emsp;&emsp;过滤器设置形式需要再说一下。在我们设置一个全局过滤器时，我们需要获取一个日志核心实例。这个就是```logging::core::get()```所做的事情，它会返回一个指向核心单例的指针。日志核心的```set_filter```函数设置了全局过滤函数。

&emsp;&emsp;在本示例中的过滤器像[Boost.Phoenix][boost_phoenix]lambda表达式一样编写。在我们的例子中，表达式包含一个单独的谓词逻辑。谓词左边参数描述一个等待被校验的属性，谓词的右边是需要被校验的值。severity是此程序库提供的关键词，此关键词标识日志的严重程度属性。其名称为"Severity"，类型为[severity_level](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/log/reference.html#boost.log.trivial.severity_level)。在trivial logging中自动提供此属性。用户只需在声明中提供属性值。关键词以及比较操作符创建一个函数对象，此函数对象会被日志核心调用，来过滤日志记录。结果是，只有日志严重等级不低于给定值的日志记录会通过此过滤器，并输出到屏幕上。

&emsp;&emsp;也可以构建更加复杂的过滤器。组合像这样的多个谓词逻辑，或者定义自己的函数(包括C++11 lambda函数)来作为一个过滤器。我们会在后续的章节中继续回到过滤器。


### <a name="set-up-sink"></a> 建立sink ###

## <a name="detailed-feature-description"></a>详细特征描述
