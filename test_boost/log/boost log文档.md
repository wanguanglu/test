# Boost.Log v2
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
	* [创建logger并写日志](#create-logger-and-write-log)
	* [属性](#attributes)
	* [日志记录格式化](#log-record-formatting)
	* [回顾filtering](#filtering-revisit)
	* [宽字节日志](#wide-char-logging)
* [详细特性描述](#detailed-feature-description)
	* [核心组件](#core-facilities)
	* [日志源](#detailed-logging-source)


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
* [创建logger并写日志](#create-logger-and-write-log)
* [属性](#attributes)
* [日志记录格式化](#log-record-formatting)
* [回顾filtering](#filtering-revisit)
* [宽字节日志](#wide-char-logging)

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

>![important](http://www.boost.org/doc/libs/1_60_0/doc/src/images/important.png)**重要**
需要记住流表达式只有在记录通过过滤器之后才会执行。不要在流表达式中指定关键业务调用。因为当记录被过滤掉之后，这些调用可能没有被执行。

&emsp;&emsp;过滤器设置形式需要再说一下。在我们设置一个全局过滤器时，我们需要获取一个日志核心实例。这个就是```logging::core::get()```所做的事情，它会返回一个指向核心单例的指针。日志核心的```set_filter```函数设置了全局过滤函数。

&emsp;&emsp;在本示例中的过滤器像[Boost.Phoenix][boost_phoenix]lambda表达式一样编写。在我们的例子中，表达式包含一个单独的谓词逻辑。谓词左边参数描述一个等待被校验的属性，谓词的右边是需要被校验的值。severity是此程序库提供的关键词，此关键词标识日志的严重程度属性。其名称为"Severity"，类型为[severity_level](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/log/reference.html#boost.log.trivial.severity_level)。在trivial logging中自动提供此属性。用户只需在声明中提供属性值。关键词以及比较操作符创建一个函数对象，此函数对象会被日志核心调用，来过滤日志记录。结果是，只有日志严重等级不低于给定值的日志记录会通过此过滤器，并输出到屏幕上。

&emsp;&emsp;也可以构建更加复杂的过滤器。组合像这样的多个谓词逻辑，或者定义自己的函数(包括C++11 lambda函数)来作为一个过滤器。我们会在后续的章节中继续回到过滤器。


### <a name="set-up-sink"></a> 建立sink ###
&emsp;&emsp;有时候trivial logging不能够提供足够的灵活度。例如，我们可能希望在日志处理过程中采用更加复杂的逻辑，而不是简单的打印到屏幕上。为了实现这些个性化的需求，你必须建立日志sink，并将它们注册到日志核心。这些操作只需要在你的应用程序启动时执行一次即可。

>![Note][note-image] **须知**
需要注意的是，在之前的小节中我们没有初始化任何sink，但是trivial logging也可以工作。这是因为本程序库包含了一个默认的sink。当用户没有设置任何sink时，就会使用此sink。此sink将以一个固定的格式日志记录打印到屏幕上。默认的sink可以让trivial logging在没有任何初始化的情况下也可以正确使用。一旦你往日志核心中添加了sink，默认的sink就不会被使用。但是trivial logging的宏可以继续使用。

#### *使用文件日志*
&emsp;&emsp;在开始是，你应该初始化日志输出到文件。
```csharp
void init()
{
    logging::add_file_log("sample.log");

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}
```

&emsp;&emsp;在初始化中添加了```add_file_log```函数调用。这个函数初始化一个将日志记录保存到文本文件的日志sink。这个函数也接受一些客户化的选项，比如日志滚动间隔和大小限制。例如：
```csharp
void init()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",                                        
        keywords::rotation_size = 10 * 1024 * 1024,                                   
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%]: %Message%"                                 
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}
```
1. 文件名的模式
2. 每10M滚动一次文件...
3. ...或者在凌晨0点
4. 日志记录的格式

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_file_manual.cpp)

&emsp;&emsp;可以看到，传给函数的选项是是一种命名表单的方式。这种方式在此程序库中的其他地方也有使用。你需要学会他。这些参数的意义是自解释的，在使用手册中有详细记录(点击[此链接](#log.detaild.sink_backends.text_file)可以查看关于文本文件sink的部分)。另外一种快捷的添加方法在[此小节](#log.detailed.utilities.setup.convenience)有介绍。

>![Note][note-image] **须知**
你可以注册多个sink，每一个sink会接收并处理日志记录，互相独立。

#### *深度学习sink 更多细节*
&emsp;&emsp;如果你不想了解更多细节，勀有跳过此小节，继续阅读下一节。如果你想对sink的配置有更加复杂的控制，或者除了已有的sink之外，希望使用更多sink，你可以手动注册更多的sink。

&emsp;&emsp;在最简单的情形下，调用```add_file_log```函数，和以下代码基本等价。

```csharp
void init()
{
    // Construct the sink
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    // Add a stream to write log to
    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("sample.log"));

    // Register the sink in the logging core
    logging::core::get()->add_sink(sink);
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_file_manual.cpp)

&emsp;&emsp;首先可以看到上面的sink是由两个类合成的，分别是前端和后端。前端(在上面代码中使用[synchronous_sink](#log.detailed.sink_frontends.sync)模板)负责对于sink来说的许多通用任务，比如线程同步模式、过滤、格式化等。后端(在上述代码中使用[text_ostream_backend](#log.detailed.sink_backends.text_ostream)类)实现sink特定的事情，在此示例中是写到一个文件。此程序库提供了许多直接可用的前端和后端。

&emsp;&emsp;[synchronous_sink](#log.detailed.sink_frontends.sync)类模板表示此sink是同步的。它允许许多个线程同时写日志，当出现冲突时会阻塞。这就意味着后端类[text_ostream_backend](#log.detailed.sink_backends.text_ostream)完全不用担心多线程的问题。本程序库还提供了很多sink前端，你可以点击[此链接](#sink_frontends)来查看他们。

&emsp;&emsp;[text_ostream_backend](#log.detailed.sink_backends.text_ostream)类写格式化的日志记录到STL流中。在上面我们使用文件流，其实我们可以使用任何形式的流。例如，我们可以像下面代码所表示的，输出到屏幕中。

```csharp
#include <boost/core/null_deleter.hpp>

// We have to provide an empty deleter to avoid destroying the global stream object
boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());
sink->locked_backend()->add_stream(stream);
```

&emsp;&emsp;[text_ostream_backend](#log.detailed.sink_backends.text_ostream)类支持添加多个流，在这种情况下它的输出会被复制到所有添加的流中。这对需要同时将日志打印到文件和屏幕是非常有用的。这样的话，对于每个日志记录，过滤、格式化以及一些其他的操作只需要执行一次。

>![Note][note-image] **须知**
需要注意注册多个sink，以及注册一个包含多个目标流的sink之间的差别。因为对于前者来说用户话的输出在sink之间是相互独立的，对于后者来说，如果这些客户化的操作是不需要的，会运行更快。这个此后端类的一个特性。

&emsp;&emsp;此程序库提供了许多[后端](#sink_backends)，他们分别实现了不同的日志处理逻辑。例如指定[Syslog](#log.detailed.sink_backends.syslog)后端，你可以通过网络将日志发送到syslog服务器。或者通过设置[Windows事件日志](log.detailed.sink_backends.event_log)后端，你通过标准的windows工具来监控你的程序运行情况。

&emsp;&emsp;最后在这里需要提醒的是，通过调用```locked_backend```成员函数可以获取sink后端，通过它可以对后端进行线程安全地访问。此函数在所有的sink前端都有提供。这个函数返回一个指向后端的智能指针，只要这个智能指针存在，后端就被锁住。这就意味着如果有其它线程尝试写日志，同时日志记录通过了sink的话，这条日志将会被阻塞，直到你释放了这个后端。但是此处有一个例外，[unlocked_sink](log.detailed.sink_frontends.unlocked)完全不同步，当他调用```locked_backend```函数时，其简单地返回不加锁的后端指针。

### <a name="create-logger-and-write-log"></a>创建logger并写日志
#### *专门的logger对象*
&emsp;&emsp;目前我们定义了日志将如何存储以及存储在哪儿。接下来将继续尝试写日志。我们可以创建一个日志源。在我们的例子中它是一个logger对象，非常简单。
```csharp
src::logger lg;
```

>![Note][note-image] **须知**
好奇的读者会发现我们在trivial logging中没有创建任何logger对象。事实上程序库已经提供了logger，在幕后已经使用

&emsp;&emsp;和sink不一样，日志源不需要在各处注册，因为它直接与日志核心进行交互。需要注意的是，程序库提供了两个版本的logger，线程安全版和非线程安全版。
对于非线程安全的logger，不同的线程使用不同的logger实例来写日志是安全的，因此需要每个线程有其单独的logger对象来写日志。线程安全版可以被不同的线程同时访问，但是这将会导致加锁，有可能会在打印日志的时候降低程序运行速度。线程安全的logger在他们的名字中包含_mt后缀。

&emsp;&emsp;抛开线程安全性，本程序库提供的所有的logger都支持复制构造并且支持交换。因此在你的类中添加一个logger对象是没有问题的，而且在之后你会看到，通过这种方式会得到一些额外的好处。

&emsp;&emsp;本程序库提供的一些logger包含不同的特性，比如严重等级和channel支持。这些特性可以互相组合来构建更加复杂的logger。点击[这里](#detailed-logging-source)可以看到更多细节。

#### *全局logger对象*
&emsp;&emsp;如果你不能将logger对象添加到你的类中，本程序库提供了一种声明全局logger的方式

```csharp
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::logger_mt)
```

&emsp;&emsp;在这里```my_logger```是一个用户定义的名称，通过使用此名称可以在后续获取logger实例。logger_mt是logger类型。任何本程序库提供的以及用户自己定义的logger类型都可以使用这种声明方式。然而，因为这个logger只有一个实例，你需要在多线程的情况下使用线程安全的logger作为全局logger。

>![Tip][tip-image] **小技巧**
有另外一个更复杂情况下的宏。细节在[这个小节](#log.detailed.sources.global_storage)介绍

&emsp;&emsp;后续你可以像这样获取logger
```csharp
src::logger_mt& lg = my_logger::get();
```

&emsp;&emsp;lg指向的是应用程序仅有的logger实例，即使这个应用程序含有许多模块。get函数本身是线程安全的，因此不需要在周围添加额外的同步信息。

#### *写日志*
&emsp;&emsp;无论你是用哪一种logger(成员变量或全局变量，线程安全或不安全)，你都可以按照以下形式来写日志记录。

```csharp
logging::record rec = lg.open_record();
if (rec)
{
    logging::record_ostream strm(rec);
    strm << "Hello, World!";
    strm.flush();
    lg.push_record(boost::move(rec));
}
```

&emsp;&emsp;在这里调用```open_record```函数来判断这条记录是否至少被一个sink处理。在此时会进行过滤。如果这条记录被处理，这个函数返回一个有效的日志记录对象，可以继续填充记录信息字符串。然后调用```push_record```函数完成日志记录的处理。

&emsp;&emsp;当然，上面的句法可以简单的包装到一个宏里面。而且实际上，我们鼓励用户用宏的方式而不是通过调用C++ logger方式。上述日志记录可以写成这样的形式。

```csharp
BOOST_LOG(lg) << "Hello, World!";
```

看起来短了很多。本程序库里还顶定义了一些类似于```BOOST_LOG```的其它宏。提供了一种类似于STL流的方式来格式化信息。
当所有的代码完成之后，编译运行，你会在"sample.log"文件中发现"Hello, World!"记录。你可以在[这里](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_logging.cpp)看到本节的完整代码。

### <a name="attributes"></a> 属性
&emsp;&emsp;在之前的章节中已经多次提到属性和属性值。在本节将带领大家学习如何使用属性为日志记录增加更多信息。

&emsp;&emsp;每一个日志记录都可以包含一些命名属性。属性可以表示日志记录发生时的任何基本信息。比如代码中的位置、可执行模块的名称、当前日期和时间、与你程序相关的任何数据以及运行环境。一个属性就像一个数值发生器一样，对于每一个日志记录返回不同的值。一旦属性产生了数值，后面的操作就与产生者独立了，可以被过滤器，格式化工具以及sink使用。用户想使用属性，必须要知道它的名字和类型。你可以在本文档中找到一些本程序库提供的常用属性。

&emsp;&emsp;除此之外，就像在[设计概要][#design-overview]中提到的，属性分为三种作用域类型，源相关属性、线程相关属性、全局属性。当日志记录产生之后，这三种类型的属性组成一个集合，传输到sink。对于sink来说，属性的来源没有任何差别。任何作用域可以注册任何属性。注册时，需要给属性一个唯一的名称，通过名称可以查找到该属性。如果在不同作用域内有同名的属性，最小的作用域会覆盖大的作用域中的属性。就是源相关属性>线程相关属性>全局属性。这样可以通过在logger中注册同名属性来覆盖全局属性和线程属性，减少线程干扰。

&emsp;&emsp;下面介绍一些属性注册过程。

#### *常用属性*
&emsp;&emsp;有一些属性几乎在所有的应用程序中都会使用，比如日志记录个数，时间。这些属性可以通过一个简单的函数来添加啊。

```csharp
logging::add_common_attributes();

```

&emsp;&emsp;通过调用此函数，日志行ID、时间戳、进程ID、线程ID都会被注册。行ID是一个日志记录计数器，第一个日志记录的ID是1。时间戳是当前的时间(日志记录产生的时间，不是传输到sink的时间）。最后的两个属性代表产生当前日志记录的进程和线程。

>![Note][note-image] **须知**
>在单线程程序中“线程ID”没有被注册。

&emsp;
>![Tip][tip-image] **小技巧**
>在程序启动时，默认没有任何属性被注册。应用程序需要在写日志之前注册必要的属性。注册过程可以在程序库初始化时完成。细心的读者会好奇trivial logging是怎么工作的。答案是trivial logging使用的默认sink，默认sink除了需要日志严重等级之外，不需要任何其他的属性，因此也不需要初始化。一旦你使用了过滤器、格式化工具和非默认sink，你必须注册必要的属性。

&emsp;&emsp;[add_common_attributes](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/add_common_attributes.html)函数在[这里][#log.detailed.utilities.setup.convenience]有更详细的描述。

&emsp;&emsp;logger在构建时会自动注册一些属性。例如[severity_logger][#log.detailed.sources.severity_level_logger]类会注册一个源相关的属性“Severity”，此属性可以用来表示一个日志记录的重要程度。例如：

```csharp
// We define our own severity levels
enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

void logging_function()
{
    // The logger implicitly adds a source-specific attribute 'Severity'
    // of type 'severity_level' on construction
    src::severity_logger< severity_level > slg;

    BOOST_LOG_SEV(slg, normal) << "A regular message";
    BOOST_LOG_SEV(slg, warning) << "Something bad is going on but I can handle it";
    BOOST_LOG_SEV(slg, critical) << "Everything crumbles, shoot me now!";
}
```

>![Tip][tip-image] **小技巧**
>你可以为通过重载```operator <<```函数为日志等级类型定义自己的格式化规则。
>查看[这里][#log.detailed.expressions.attr]来查看具体信息。

&emsp;&emsp;```BOOST_LOG_SEV```宏和```BOOST_LOG```宏很相似，
不同之处是此函数还需要提供调用的日志logger。```BOOST_LOG_SEV```宏可以被以下代码替换。

```csharp
void manual_logging()
{
    src::severity_logger< severity_level > slg;

    logging::record rec = slg.open_record(keywords::severity = normal);
    if (rec)
    {
        logging::record_ostream strm(rec);
        strm << "A regular message";
        strm.flush();
        slg.push_record(boost::move(rec));
    }
}
```

&emsp;&emsp;可以看到```open_record```函数可以支持一个命名的参数。本程序库提供的一些logger类型至此回这种参数方法。
用户也可以在自己的logger类中使用类似的方法。


#### *更多属性*
&emsp;&emsp;我们可以先理解一下add_common_attributes函数，它可能是类似于这样的形式

```csharp
void add_common_attributes()
{
    boost::shared_ptr< logging::core > core = logging::core::get();
    core->add_global_attribute("LineID", attrs::counter< unsigned int >(1));
    core->add_global_attribute("TimeStamp", attrs::local_clock());

    // other attributes skipped for brevity
}
```

&emsp;&emsp;在这里counter和local_clock都是属性类，他们派生自通用属性接口[attribute](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/attribute.html)。
&emsp;&emsp;本程序库提供了一些其他[属性类](#detailed-attribute)。包括一些函数属性，通过调用一些函数来获取数值。
例如，我们可以非常简单地注册一个[named_scope](#log.detailed.attributes.named_scope)属性。

```csharp
core->add_global_attribute("Scope", attrs::named_scope());
```

&emsp;&emsp;通过这样我们就可以在日志记录中存储命名空间，下面是它的用法。

```csharp
void named_scope_logging()
{
    BOOST_LOG_NAMED_SCOPE("named_scope_logging");

    src::severity_logger< severity_level > slg;

    BOOST_LOG_SEV(slg, normal) << "Hello from the function named_scope_logging!";
}
```

&emsp;&emsp;logger相关属性和全局属性同样重要。严重等级、channel名称是最重要的源相关属性。你也可以添加更多的属性到你的logger中。例如

```csharp
void tagged_logging()
{
    src::severity_logger< severity_level > slg;
    slg.add_attribute("Tag", attrs::constant< std::string >("My tag value"));

    BOOST_LOG_SEV(slg, normal) << "Here goes the tagged record";
}
```

&emsp;&emsp;这样此logger生成的所有的日志记录就会包含Tag属性，此属性可以用于后续的过滤和格式化。
属性的另外一个作用是标记应用程序不同部分产生的日志记录，这样可以高亮某一个过程的活动。
我们甚至可以实现一个粗糙的profiling工具来检测性能瓶颈。例如：

```csharp
void timed_logging()
{
    BOOST_LOG_SCOPED_THREAD_ATTR("Timeline", attrs::timer());

    src::severity_logger< severity_level > slg;
    BOOST_LOG_SEV(slg, normal) << "Starting to time nested functions";

    logging_function();

    BOOST_LOG_SEV(slg, normal) << "Stopping to time nested functions";
}
```

&emsp;&emsp;Timeline包含高精度的时间信息，可以用来判断哪一部分程序需要很多的执行时间。
并不是所有的logging_function函数生成的日志记录，或者它调用的其他函数包含"Timeline"属性。
"Timeline"属性在离开timed_logging函数的空间之后，就将失效。

&emsp;&emsp;可以参考[属性](#detailed-attribute)小结，来阅读本程序库提供属性的详细描述。
详细代码可以点击[这里](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_attributes.cpp)。

#### *定义属性占位符*
&emsp;&emsp;在未来的章节中我们会看到，用一个关键词来对应用程序使用的属性是非常有用的。
这个关键词可以参与到过滤和格式化的表达式中。就像日志等级占位符一样。
例如，要使用我们之前的示例中提到的属性，我们要像如下一样定义占位符。

```csharp
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", attrs::named_scope::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(timeline, "Timeline", attrs::timer::value_type)
```

&emsp;&emsp;每一个宏定义了一个关键词。第一个参数是占位符的名称，第二个是属性名称，最后一个属性是属性类型。
一旦定义，这个占位符可以在模板表达式以及其他地方使用。定义属性关键词的详细信息可以点击[这里](#log.detailed.expressions.attr_keywords)。

### <a name="log-record-formatting"></a> **日志记录格式化**
&emsp;&emsp;如果你尝试运行之前章节的示例，你可能会注意到只有日志记录的message信息会写到文件中。
当没有设置格式化工具时，没有默认的输出。即使你将属性添加到logger中，属性并没有输出到文件。除非你将这些数值进行格式化。

&emsp;&emsp;回到之前的一个示例。

```csharp
void init()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%]: %Message%"
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::info
    );
}
```

&emsp;&emsp;在```add_file_log```函数中，format参数可以指定日志记录的格式。
如果你手动设置sink，sink前端提供了```set_formatter```成员函数。
格式可以通过多种方法设置，在后续会继续介绍。

#### *Lambda形式formatter*
&emsp;&emsp;你可以创建一个像这样的lambda表达式。

```csharp
void init()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",
        // This makes the sink to write log records that look like this:
        // 1: <normal> A normal severity message
        // 2: <error> An error severity message
        keywords::format =
        (
            expr::stream
                << expr::attr< unsigned int >("LineID")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
}
```

[查看完成代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_stream.cpp)

&emsp;&emsp;在这个stream是一个将日志记录格式化的流的占位符。其它的插入参数，例如[attr](#log.detailed.expressions.attr)和
[message](#log.detailed.expressions.message)是流里定义的操作符。我们已经在过滤器表达式中看到过severity占位符，
在这里又应用在格式化工具中。这是一个非常好的统一：你可以在过滤器和格式化工具中使用同样的占位符。
[attr](#log.detailed.expressions.attr)占位符和severity占位符类似，它表示属性值。不同的是severity占位符表示名称为"Severity"和类型为
trivial::severity_level的属性，[attr](#log.detailed.expressions.attr)可以表示任何属性。否则它们是等价的。
我们可以通过以下方式替换severity占位符。

```csharp
expr::attr< logging::trivial::severity_level >("Severity")
```

>![Tip][tip-image] **须知**
>就像在其他章节中介绍的，我们可以将用户的属性定义成类似于severity的占位符。因为占位符表达这个属性的所有信息，这样可以这样可以简化模板表达式的句法。
>这样可以减少由于拼写带来的编码错误。因此推荐大家使用这种方法来定义新的属性，以及在模板表达式中使用。

&emsp;&emsp;这里还提供了其他的[格式化表达式](#log.detailed.expressions.formatters)，来支持日期、时间以及其他的类型。
一些格式化表达式接收额外的参数来客户化他们的行为。大部分参数都可以按照[Boost.Parameter](http://www.boost.org/doc/libs/1_60_0/libs/parameter/doc/html/index.html)
的方式来解析。

&emsp;&emsp;做一些改变，我们可以看一下人工初始化sink时情况。

```csharp
void init()
{
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("sample.log"));

    sink->set_formatter
    (
        expr::stream
               // line id will be written in hex, 8-digits, zero-filled
            << std::hex << std::setw(8) << std::setfill('0') << expr::attr< unsigned int >("LineID")
            << ": <" << logging::trivial::severity
            << "> " << expr::smessage
    );

    logging::core::get()->add_sink(sink);
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_stream_manual.cpp)

&emsp;&emsp;可以看到，可以在表达式中绑定格式转换参数。这些操作符会影响紧接着的属性值格式。
更多的操作符描述在[详细特征描述](#detailed-expresion)小节有介绍。

#### *Boost.Format-style formatter*

&emsp;&emsp;另外一种方式，我们也可以定义一种类似于[Boost.Format](http://www.boost.org/doc/libs/release/libs/format/index.html)格式的句法。
上面的格式我们可以按照下面的方式来书写：

```csharp
void init()
{
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("sample.log"));

    // This makes the sink to write log records that look like this:
    // 1: <normal> A normal severity message
    // 2: <error> An error severity message
    sink->set_formatter
    (
        expr::format("%1%: <%2%> %3%")
            % expr::attr< unsigned int >("LineID")
            % logging::trivial::severity
            % expr::smessage
    );

    logging::core::get()->add_sink(sink);
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_format.cpp)

&emsp;&emsp;fomat占位符接受位置相关的参数进行格式化。注意现在仅仅位置相关格式化，相通的格式化方式也可以在```add_file_log```以及类似的函数中使用。

##### *特殊formatter*

&emsp;&emsp;本程序库提供了针对数字的特殊formatter。例如日期，时间和named scope。这些formatter提供了针对格式化数值的扩展控制。例如，可以将日期和时间
格式化成(Boost.DateTime)[http://www.boost.org/doc/libs/release/doc/html/date_time.html]可以编译的格式。

```csharp
void init()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",
        // This makes the sink to write log records that look like this:
        // YYYY-MM-DD HH:MI:SS: <normal> A normal severity message
        // YYYY-MM-DD HH:MI:SS: <error> An error severity message
        keywords::format =
        (
            expr::stream
                << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                << ": <" << logging::trivial::severity
                << "> " << expr::smessage
        )
    );
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_stream.cpp)

&emsp;&emsp;同样的formatter可以在Boost.Format-style formatter中使用。


#### *使用字符串模板作为formatter*

&emsp;&emsp;在一些场景下，文本模板可以用作formatter。在此时程序库初始化支持代码会运行，来解析这些模板并重建合适的formatter。这里有许多注意事项需要牢记。在这里像如下定义就足够了。

```csharp
void init()
{
    logging::add_file_log
    (
        keywords::file_name = "sample_%N.log",
        keywords::format = "[%TimeStamp%]: %Message%"
    );
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_string.cpp)

&emsp;&emsp;在这里，format参数接受一个format模板。这个模板勀有包含一些占位符，用百分号(%)包围。每个占位符必须包含一个属性值来替代这个占位符。
%Message%占位符会被日志记录的message来替换。

>![Note][note-image] "须知"
>sink后端的set_formatter函数并不接受文本格式模板。为了让formatter解析文本模板，必须调用parse_formatter函数。
>在[这里](#log.detailed.utilities.setup.filter_formatter)看更多的详细信息。

#### *客户格式化函数*
&emsp;&emsp;你可以在sink中添加客户formatter来支持格式化。formatter实际上是一个函数对象，支持以下格式。

```csharp
void (logging::record_view const& rec, logging::basic_formatting_ostream< CharT >& strm);
```

>![Tip][tip-image]
>record_view和记录很相似。不同之处是record_view是不可变的，其实现了浅拷贝。 formatter和sink仅仅在record_view上操作，这样避免他们修改日志记录，这样其他线程的sink还可以继续使用这些日志记录。

&emsp;&emsp;格式化之后记录可以通过STL格式的输出流来组合。这里是一个客户formatter函数的示例。

```csharp
void my_formatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
    // Get the LineID attribute value and put it into the stream
    strm << logging::extract< unsigned int >("LineID", rec) << ": ";

    // The same for the severity level.
    // The simplified syntax is possible if attribute keywords are used.
    strm << "<" << rec[logging::trivial::severity] << "> ";

    // Finally, put the record message to the stream
    strm << rec[expr::smessage];
}

void init()
{
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("sample.log"));

    sink->set_formatter(&my_formatter);

    logging::core::get()->add_sink(sink);
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_fmt_custom.cpp)

### <a name="filtering-revisit"></a>回顾filtering
&emsp;&emsp;在前面的章节中我们提到了filtering，但是我们只是提到了一些皮毛。现在我们已经可以在日志记录中添加属性，并可以设置sink。
我们可以构建更加复杂的过滤器。

```csharp
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)

void init()
{
    // Setup the common formatter for all sinks
    logging::formatter fmt = expr::stream
        << std::setw(6) << std::setfill('0') << line_id << std::setfill(' ')
        << ": <" << severity << ">\t"
        << expr::if_(expr::has_attr(tag_attr))
           [
               expr::stream << "[" << tag_attr << "] "
           ]
        << expr::smessage;

    // Initialize sinks
    typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
    boost::shared_ptr< text_sink > sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("full.log"));

    sink->set_formatter(fmt);

    logging::core::get()->add_sink(sink);

    sink = boost::make_shared< text_sink >();

    sink->locked_backend()->add_stream(
        boost::make_shared< std::ofstream >("important.log"));

    sink->set_formatter(fmt);

    sink->set_filter(severity >= warning || (expr::has_attr(tag_attr) && tag_attr == "IMPORTANT_MESSAGE"));

    logging::core::get()->add_sink(sink);

    // Add attributes
    logging::add_common_attributes();
}
```

[查看完整代码](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_filtering.cpp)

&emsp;&emsp;在此示例中，我们初始化了两个sink。一个来存储完整的日志，另外一个来存储重要日志。两个sink都会向文本文件中写同样格式的日志记录。
[formatter](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/basic_formatter.html)类型是一个包含格式化调用签名的函数对象。
在很多方面它和```boost::function```或者```std::function```很类似。不同之处是它从来不会为空。有一个类似的
[过滤器函数对象](http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/filter.html)。

&emsp;&emsp;值得注意的是，这个formatter本身包含一个fiter。这个formatter包含一个条件判断，来判断Tag属性。
[has_attr](#log.detailed.expressions.predicates.has_attr)谓词判断此记录是否包含"Tag"属性值。然后控制是否将它打印到文件。
我们使用属性关键词来指定属性的名称和类型，也可以在[has_attr](#log.detailed.expressions.predicates.has_attr)调用中指定。
在[这里](#log.detailed.expressions.formatters.conditional)包含条件formatter的更详细描述。

&emsp;&emsp;回顾一下两个sink的初始化过程，第一个sink没有任何fiter，这意味着它会将所有的日志记录输出到文件爱你。第二个sink中调用了```set_filter```函数。
仅仅保存日志严重等级不低于```warning```的，或者包含```Tag```属性且属性值为"IMPORTANT_MESSAGE"的。我们可以看到过滤器句法和C++很像，特别当使用属性关键词时。

&emsp;&emsp;和formatter类似，可以使用客户化函数来当做filter。在此情况下[Boost.Phoenix][boost_phoenix]会非常有用。因为其bind实现和属性占位符是兼容的。
之前的示例，可以改成如下的格式。

```csharp
bool my_filter(logging::value_ref< severity_level, tag::severity > const& level,
               logging::value_ref< std::string, tag::tag_attr > const& tag)
{
    return level >= warning || tag == "IMPORTANT_MESSAGE";
}

void init()
{
    // ...
    namespace phoenix = boost::phoenix;
    sink->set_filter(phoenix::bind(&my_filter, severity.or_none(), tag_attr.or_none()));
    // ...
}
```

&emsp;&emsp;客户化filter接受封装到[value_ref](#log.detailed.utilities.value_ref)属性值。这个wrapper包含一个代表属性类型的选项。
当这个日志记录包含要求类型的属性值时此引用有效。```my_filter```函数中使用关系操作符不用判断有效性条件判断，因为当引用为无效是返回为```false```。
在bind表达式中会识别```severity```和```tag_attr```关键词。并抽取相关的值传给```my_filter```函数。


>![Note][note-image] **须知**
>因为与[Boost.Phoenix][boost_phoenix]整合的限制(参考[#7996](#https://svn.boost.org/trac/boost/ticket/7996))，
>在含有属性关键词时，调用phoenix::bind或者phoenix::function，必须显式地设置属性缺失时的回退策略。
>在上面的例子中，通过调用or_none来实现。这样在值无法找到时，会返回一个空的[value_ref](#log.detailed.utilities.value_ref)。
>在其他的场景下，默认采用此策略。还可以使用这里的[其他策略](#log.detailed.expressions.attr.fallback_policies)来替代。

&emsp;&emsp;你可以运行[示例](http://www.boost.org/doc/libs/1_60_0/libs/log/example/doc/tutorial_filtering.cpp)程序，来查看工作情况。

### <a name="wide-char-logging"></a>宽字节日志
&emsp;&emsp;本程序库支持国际化字符。基本来说有两种方式。在类UNIX系统下，一般采用多字节字符编码（例如UTF-8），在这种情况下和纯ascii码字符
差别不大，不需要额外的设置。

&emsp;&emsp;在Windows操作系统中，一般采用宽字节字符来表示国际化字符。大部分系统API是面向宽字节字符的，这就需要Windows的sink需要支持宽字节。
另一方面，通用的sink，例如[text_file_sink](#log.detailed.sink_backends.text_file)是面向字节的（因为，你是存储字节到文件中，而不是存储字符）。
这样会强制此程序库在sink需要时执行字符编码转换。为了设置程序库支持这种做法，我们必须将sink进行适当的本地化设置。[Boost.Locale][boost_locale]可以用了进行类似的设置。
我们可以查看一个示例:

```csharp
// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

void init_logging()
{
    boost::shared_ptr< sinks::synchronous_sink< sinks::text_file_backend > > sink = logging::add_file_log
    (
        "sample.log",
        keywords::format = expr::stream
            << expr::format_date_time(timestamp, "%Y-%m-%d, %H:%M:%S.%f")
            << " <" << severity.or_default(normal)
            << "> " << expr::message
    );

    // The sink will perform character code conversion as needed, according to the locale set with imbue()
    std::locale loc = boost::locale::generator()("en_US.UTF-8");
    sink->imbue(loc);

    // Let's add some commonly used attributes, like timestamp and record counter.
    logging::add_common_attributes();
}
```

&emsp;&emsp;首先我们看一下传给formatter的格式化参数。我们用窄字节的formatter初始化sink，因为text_file_sink处理字节。
我们可以在formatter中使用宽字节，但是不是在格式化字符串中。类似于我们使用[format_date_time](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/expressions/format_d_idm45635677645456.html)函数。同时需要注意的是用message关键字来表示日志记录的message，这个占位符既可以表示窄字节，也可以表示宽字节。
这样formatter在两种模式下都可以工作。在格式化的过程中，本程序库会将宽字节信息转换成多字节编码，我们在这里设置为UTF-8。

>![Tip][tip-image] **小技巧**
>和日志记录的message类似，属性值也可以包含宽字节。这些字符串会通过imbued locale转换成目标字符编码。

&emsp;&emsp;在这里我们没有将severity_level类型的定义。此类型是一个枚举类型，如果我们希望它同时支持宽字节和窄字节，我们可以实现它的流操作符。

```csharp
enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

template< typename CharT, typename TraitsT >
inline std::basic_ostream< CharT, TraitsT >& operator<< (
    std::basic_ostream< CharT, TraitsT >& strm, severity_level lvl)
{
    static const char* const str[] =
    {
        "normal",
        "notification",
        "warning",
        "error",
        "critical"
    };
    if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
        strm << str[lvl];
    else
        strm << static_cast< int >(lvl);
    return strm;
}
```

&emsp;&emsp;在这里我们可以输出日志记录。我们可以用还有名称含有w前缀的logger来组合宽字节信息。

```csharp
void test_narrow_char_logging()
{
    // Narrow character logging still works
    src::logger lg;
    BOOST_LOG(lg) << "Hello, World! This is a narrow character message.";
}

void test_wide_char_logging()
{
    src::wlogger lg;
    BOOST_LOG(lg) << L"Hello, World! This is a wide character message.";

    // National characters are also supported
    const wchar_t national_chars[] = { 0x041f, 0x0440, 0x0438, 0x0432, 0x0435, 0x0442, L',', L' ', 0x043c, 0x0438, 0x0440, L'!', 0 };
    BOOST_LOG(lg) << national_chars;

    // Now, let's try logging with severity
    src::wseverity_logger< severity_level > slg;
    BOOST_LOG_SEV(slg, normal) << L"A normal severity message, will not pass to the file";
    BOOST_LOG_SEV(slg, warning) << L"A warning severity message, will pass to the file";
    BOOST_LOG_SEV(slg, error) << L"An error severity message, will pass to the file";
}
```

&emsp;&emsp;可以看到，宽字节字符message的合成和窄字节非常相似。你可以同时使用窄字节和宽字节。我们的文件sink会处理所有的记录。
完整的代码可以点击[这里](#http://www.boost.org/doc/libs/1_60_0/libs/log/example/wide_char/main.cpp)

&emsp;&emsp;需要注意的是一些sink（主要是Windows相关的）允许指定目标字节类型。当希望在日志记录中记录国际字符时，我们需要使用wchar_t来作为目标字符类型。
因为这些sink会使用宽字节系统API来处理日志记录。当格式化时窄字节会使用locale加宽。

## <a name="detailed-feature-description"></a>详细特性描述

* [核心组件](#core-facilities)
* [日志源](#detailed-logging-source)

## <a name=core-facilities></a>核心组件

* [日志记录](#log.detailed.core.record)
* [日志核心](#logging-core)

### <a name="log.detailed.core.record"></a>日志记录

```csharp
#include <boost/log/core/record.hpp>
```

&emsp;&emsp;在本日志程序库中，所有的信息都被打包到[record](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/record.html)
对象中，所有的依附信息，包括message文本都会表示成命名属性值，可以被filter,formatter和sink处理。一些特殊的属性可以有不同的访问方式，这里有一些快速的例子。

* 通过[值来访问和提取](#log.detailed.attributes.related_components.value_processing)

```csharp
enum severity_level { ... };
std::ostream& operator<< (std::ostream& strm, severity_level level);

struct print_visitor
{
    typedef void result_type;
    result_type operator() (severity_level level) const
    {
        std::cout << level << std::endl;
    };
};

// Prints severity level through visitation API
void print_severity_visitation(logging::record const& rec)
{
    logging::visit< severity_level >("Severity", rec, print_visitor());
}

// Prints severity level through extraction API
void print_severity_extraction(logging::record const& rec)
{
    logging::value_ref< severity_level > level = logging::extract< severity_level >("Severity", rec);
    std::cout << level << std::endl;
}
```

* 通过日志记录的attribute_value方法查找[属性值集合](#log.detailed.attributes.related_components.attribute_value_set)

```csharp
// Prints severity level by searching the attribute values
void print_severity_lookup(logging::record const& rec)
{
    logging::attribute_value_set const& values = rec.attribute_values();
    logging::attribute_value_set::const_iterator it = values.find("Severity");
    if (it != values.end())
    {
        logging::attribute_value const& value = it->second;

        // A single attribute value can also be visited or extracted
        std::cout << value.extract< severity_level >() << std::endl;
    }
}
```

* 通过属性关键词下表。实际上是由一个方便的wrapper包含在值提取API周围

```csharp
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

// Prints severity level by using the subscript operator
void print_severity_subscript(logging::record const& rec)
{
    // Use the attribute keyword to communicate the name and type of the value
    logging::value_ref< severity_level, tag::severity > level = rec[severity];
    std::cout << level << std::endl;
}
```

&emsp;&emsp;日志记录不可以被拷贝，只能被移动。一个日志记录可以被默认初始化成为一个空状态。
这种记录基本是不可用的，也不能被传到程序库中来处理。一个非空的日志记录只能有[日志核心](#logging-core)产生，作为成功filtering的结果。
此非空日志记录包含属性需要的属性值。在filter之后，更多的属性值可以被添加到非空日志记录中。后续增加的值不会影响filtering结果，但是会在formatter和sink中使用。

&emsp;&emsp;在多线程环境中，一个非空的日志记录创建之后，此日志记录tie到当前的线程中。
被看作线程死昂管的资源。例如，此记录可能包含一个属性值来代表栈中的命名作用域。
因此，日志记录不可以在不同的线程之间互传。

#### *Record view*

```csharp
#include <boost/log/core/record_view.hpp>
```

&emsp;&emsp;当使用日志记录来填充信息时，此程序库使用另外的类型来处理它。
Record view提供了和日志记录类似的接口。但是有一些明显的区别：

* Record view是不可变的。这样避免formatter和sink在处理过程中修改记录。
* Record view是可以拷贝的。因为它的内容是常量，拷贝操作是浅层拷贝而且代价很低。

&emsp;&emsp;本程序库通过调用```lock```函数自动从日志记录中创建record view。
通过调用此函数，在sink异步的情况下，同时会保证结果view不再依附于当前线程。
调用```lock```函数是一个一次性操作。之后，record会处于空状态。
所有的通过属性值来与日志记录交互的方式同样适用于record view。
同时可以在客户化formatter和sink中使用。

### <a name="logging-core"></a>日志核心

* [属性集合](#attribute-set)
* [全局filtering](#global-filtering)

```csharp
#include <boost/log/core/core.hpp>
```

&emsp;&emsp;日志核心是一个中枢，它提供了一下功能：
* 保存全局和线程相关属性集合。
* 执行对于日志记录的全局filtering。
* 通过执行sink相关的filter在sink之间调度日志记录
* 提供一个全局的钩子来进行异常捕捉
* 为日志源提供日志记录输入的入口
* 提供```flush```函数，此函数可以为所有日志sink执行同步状态。

&emsp;&emsp;日志核心是一个应用程序等级的单例，因此所有的日志源都需要访问它。此核心实例可以通过静态方法```get```来访问。

```csharp
void foo()
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // ...
}
```

#### <a name="attribute-set"></a>*属性集合*
&emsp;&emsp;为了在核心中增加或者删除全局或者线程相关的属性，有一些相关的函数：
```add_global_attribute```， ```remove_global_attribute```， ```add_thread_attribute```和```remove_thread_attribute```。
属性集合提供的接口和std::map类似。因此```add_*```函数接受一个属性名字符串（key）和一个属性指针（映射value）。
同时返回一个iterator和boolean类型的pair，类似于std::map<...>::insert工作方式。```remove_*```方法接受一个之前增加的属性的iterator。

```csharp
void foo()
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Add a global attribute
    std::pair< logging::attribute_set::iterator, bool > res =
        core->add_global_attribute("LineID", attrs::counter< unsigned int >());

    // ...

    // Remove the added attribute
    core->remove_global_attribute(res.first);
}
```

>![Tip][tip-image] **小技巧**
>日志核心的所有函数，在多线程环境中都是线程安全的。然而对于其他的模块可能不是这样，比如iterator和属性集合。

&emsp;&emsp;获取完整的属性集合（全局和线程相关）的拷贝或者将他们设置到核心中是可以的。
可以通过函数```get_global_attributes```，```set_global_attributes```，```get_thread_attributes```和```set_thread_attributes```来实现。

>![Warning][warning-image] **警告**
>当设置完整的属性集合到核心之后，所有通过```add_*```函数添加产生的iterator都会失效。特别是他会影响[作用域属性](#log.detailed.attributes.related_components.scoped_attributes)，因此用户在设置属性集合是一定要非常小心。

#### <a name=global-filtering></a>*全局filtering*
&emsp;&emsp;全局filter是通过filter函数对象来处理的，通过```set_filter```函数来实现。
更多创建filter的信息在[此小节](#log.detailed.expressions.predicates)有介绍。
在这里我们介绍说filter就是接受一个属性值集合同时返回一个布尔值来判断此日志记录是否通过filter的过滤。
全局filter对于每个日志记录都会实施，因此它可以用来快速的清理过度的日志记录。

&emsp;&emsp;全局filter可以通过```reset_filter```函数来删除。当核心中没有filter时，就没有记录被过滤掉。
下面是日志核心初始构建之后的默认操作。

```csharp
enum severity_level
{
    normal,
    warning,
    error,
    critical
};

void foo()
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Set a global filter so that only error messages are logged
    core->set_filter(expr::attr< severity_level >("Severity") >= error);

    // ...
}
```

&emsp;&emsp;核心同时也提供了另外的方法来禁用日志，通过调用```set_logging_enable```函数，我们可以完全禁止
或者重新开启日志。禁用日志比设置全局filter来讲对于程序性能更好。

#### *sink管理*
&emsp;&emsp;当全局filtering实施之后，就会进入到sink。为了增加或者删除sink，核心提供了```add_sink```和```remove_sink```函数。
这两个函数都接受一个sink指针作为参数，如果一个sink还没有添加到核心中，```add_sink```会将此sink添加到核心中。
```remove_sink```函数会在已添加sink列表中查找给定的sink，如果找到就将它删除。核心并没有指定处理sink的顺序。

```csharp
void foo()
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Set a sink that will write log records to the console
    boost::shared_ptr< sinks::text_ostream_backend > backend =
        boost::make_shared< sinks::text_ostream_backend >();
    backend->add_stream(
        boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));

    typedef sinks::unlocked_sink< sinks::text_ostream_backend > sink_t;
    boost::shared_ptr< sink_t > sink = boost::make_shared< sink_t >(backend);
    core->add_sink(sink);

    // ...

    // Remove the sink
    core->remove_sink(sink);
}
```

&emsp;&emsp;你可以在一下的小节[sink前端](#sink_frontends)和[sink后端](#sink_backends)中
阅读关于sink设计的更多细节。


#### *异常处理*
&emsp;&emsp;核心提供了一种设置集中的异常处理的方式。在filtering或者处理一个sink时发生异常，如果用户通过```set_exception_handler```函数设置了异常处理句柄，核心会调用此异常处理句柄。
一个异常句柄是一个零元的函数对象，在catch中调起。此程序库提供了一些[工具](#log.detailed.utilities.exception_handlers)来简化异常句柄构建。

>![Tip][tip-image] **小技巧**
>日志核心中的异常句柄是全局的，用来处理一些通用的运行错误。sink和源也会提供一些异常处理功能(参考[这里](#log.detailed.sink_frontends.basic_services.exception_handling)和
[这里](#log.detailed.sources.exception_handling))，可以用来进行细粒度的错误处理。

```csharp
struct my_handler
{
    typedef void result_type;

    void operator() (std::runtime_error const& e) const
    {
        std::cout << "std::runtime_error: " << e.what() << std::endl;
    }
    void operator() (std::logic_error const& e) const
    {
        std::cout << "std::logic_error: " << e.what() << std::endl;
        throw;
    }
};

void init_exception_handler()
{
    // Setup a global exception handler that will call my_handler::operator()
    // for the specified exception types
    logging::core::get()->set_exception_handler(logging::make_exception_handler<
        std::runtime_error,
        std::logic_error
    >(my_handler()));
}
```

#### *提供日志记录*
&emsp;&emsp;日志核心最重要的函数是为所有的日志源提供了一个日志记录的入口。通过```open_record```和```push_record```函数来实现。

&emsp;&emsp;第一个函数用来初始化日志记录过程。它接受一个源相关集合。此函数构建一个包含了三种集合（全局，线程相关，源相关）的属性的通用属性集合。
同时执行filtering。如果filtering成功，比如至少一个sink接受此记录，此函数返回一个非空[record](#log.detailed.core.record)对象。
它可以被用来填充日志记录信息。如果filtering失败，则返回一个空record对象。

&emsp;&emsp;如果日志源已经完成了日志过程，它必须调用```push_record```函数，其参数是```open_record```的返回值。注意，不要对空记录调用```push_record```
函数。记录会被当作一个右值引用来进行传输。在调用之后，会构建一个record view，这个view会传给sink，可能会引入formatting和后续的处理，
例如，将它存储到文件或者传输给互联网。在此之后这个record对象可以毁灭。

```csharp
void logging_function(logging::attribute_set const& attrs)
{
    boost::shared_ptr< logging::core > core = logging::core::get();

    // Attempt to open a log record
    logging::record rec = core->open_record(attrs);
    if (rec)
    {
        // Ok, the record is accepted. Compose the message now.
        logging::record_ostream strm(rec);
        strm << "Hello, World!";
        strm.flush();

        // Deliver the record to the sinks.
        core->push_record(boost::move(rec));
    }
}
```

&emsp;&emsp;这些逻辑都隐含在宏和logger中调用的。但是这对于开发新的日志源是非常有用的。

### <a name="detailed-logging-source"></a>日志源

* [基本日志](#log.detailed.sources.basic_logger)
* [带有日志等级支持的logger](#log.detailed.sources.severity_level_logger)
* [支持channel的logger](#log.detailed.sources.channel_logger)
* [包含混合特性的logger](#log.detailed.sources.mixed_loggers")
* [全局存储的logger](#log.detailed.sources.global_storage")

### <a name="log.detailed.sources.basic_logger"></a>基本日志
```csharp
#include <boost/log/sources/basic_logger.hpp>
```

&emsp;&emsp;本程序库提供的最简单的日志源是[logger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/logger.html)以及其线程安全版本[logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/logger_mt.html)，在宽字节日志下，
是[wlogger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wlogger.html)和
[wlogger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wlogger_mt.html)。
这些logger将源相关属性存储在自身对象中，用来组成日志记录。在不需要日志等级判断等高级特性是，可以使用这些logger。
可以用来收集应用的统计信息，注册应用事件，比如通知和报警。在这些案例中logger一般结合
[scope attribute](#log.detailed.attributes.related_components.scoped_attributes)
一起使用。提供必要的数据来进行通知，下面是一个具体的示例。

```csharp
class network_connection
{
    src::logger m_logger;
    logging::attribute_set::iterator m_remote_addr;

public:
    void on_connected(std::string const& remote_addr)
    {
        // Put the remote address into the logger to automatically attach it
        // to every log record written through the logger
        m_remote_addr = m_logger.add_attribute("RemoteAddress",
            attrs::constant< std::string >(remote_addr)).first;

        // The straightforward way of logging
        if (logging::record rec = m_logger.open_record())
        {
            rec.attribute_values().insert("Message",
                attrs::make_attribute_value(std::string("Connection established")));
            m_logger.push_record(boost::move(rec));
        }
    }
    void on_disconnected()
    {
        // The simpler way of logging: the above "if" condition is wrapped into a neat macro
        BOOST_LOG(m_logger) << "Connection shut down";

        // Remove the attribute with the remote address
        m_logger.remove_attribute(m_remote_addr);
    }
    void on_data_received(std::size_t size)
    {
        // Put the size as an additional attribute
        // so it can be collected and accumulated later if needed.
        // The attribute will be attached only to this log record.
        BOOST_LOG(m_logger) << logging::add_value("ReceivedSize", size) << "Some data received";
    }
    void on_data_sent(std::size_t size)
    {
        BOOST_LOG(m_logger) << logging::add_value("SentSize", size) << "Some data sent";
    }
};
```

&emsp;&emsp;上面代码片段中```network_connection```类，利用简单的日志，来统计网络应用的信息。
每个方法标记一个相关事件，这些事件可以在sink级别进行追踪。此外，类中的其他函数，也可以写日志。
值得注意的是，```network_connection```对象在连接状态下的每一个日志记录都可以标记远端站点的IP地址。

### <a name="log.detailed.sources.severity_level_logger"></a>带有日志等级支持的logger

```csharp
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
```

&emsp;&emsp;日志严重等级是一个日志记录最常用到的属性之一，用它来标记一条日志的严重程度。模板类
[severity_logger](#www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/severity_logger.html)和
[severity_logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/severity_logger_mt.html)
(以及在宽字节下的[wseverity_logger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wseverity_logger.html)和[wseverity_logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wseverity_logger_mt.html))
提供了此功能。

&emsp;&emsp;此logger自动注册一个特殊的源相关属性"Severity"，通过它可以设置每一条日志记录的严重等级，同时可以通过一个参数```severity```来调用。
可以将它传输给```open_record```函数或者构造函数。传给```open_record```函数```severity```参数设置了此日子记录的等级。日志等级可以作为logger模板类的
模板参数，其默认类型是int。

&emsp;&emsp;此属性的实际值以及意义用户可以自己定义。但是我们推荐将等级值从0开始。因为默认构造的logger对象将默认的日志等级设置为0.
同时推荐在整个应用程序中使用同样的日志等级，这样避免在写日志时混淆。下面的代码介绍了```severity_logger```的用法。

```csharp
// We define our own severity levels
enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

void logging_function()
{
    // The logger implicitly adds a source-specific attribute 'Severity'
    // of type 'severity_level' on construction
    src::severity_logger< severity_level > slg;

    BOOST_LOG_SEV(slg, normal) << "A regular message";
    BOOST_LOG_SEV(slg, warning) << "Something bad is going on but I can handle it";
    BOOST_LOG_SEV(slg, critical) << "Everything crumbles, shoot me now!";
}
```

```csharp
void default_severity()
{
    // The default severity can be specified in constructor.
    src::severity_logger< severity_level > error_lg(keywords::severity = error);

    BOOST_LOG(error_lg) << "An error level log record (by default)";

    // The explicitly specified level overrides the default
    BOOST_LOG_SEV(error_lg, warning) << "A warning level log record (overrode the default)";
}
```

&emsp;&emsp;如果你不喜欢使用宏

```csharp
void manual_logging()
{
    src::severity_logger< severity_level > slg;

    logging::record rec = slg.open_record(keywords::severity = normal);
    if (rec)
    {
        logging::record_ostream strm(rec);
        strm << "A regular message";
        strm.flush();
        slg.push_record(boost::move(rec));
    }
}
```

&emsp;&emsp;当然severity logger也提供了[basic_logger](#log.detailed.sources.basic_logger)所包含的功能。

### <a name="log.detailed.sources.channel_logger"></a>支持channel的logger

&emsp;&emsp;在很多时候，需要将日志记录与应用程序中的模块关联起来。可以通过
[channel_logger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/channel_logger.html)，
[channel_logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/channel_logger_mt.html)
以及其宽字节版本
[wchannel_logger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wchannel_logger.html)，
[wchannel_logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/wchannel_logger_mt.html)来实现。
这些logger中自动注册"Channel"属性。logger的channel名可以通过构造函数中的```channel```参数设置。
channel属性可以看作一个包含```std::string```模板参数的logger。除此之外，其和[basic_logger](#log.detailed.sources.basic_logger)非常类似。

```csharp
class network_connection
{
    src::channel_logger< > m_net, m_stat;
    logging::attribute_set::iterator m_net_remote_addr, m_stat_remote_addr;

public:
    network_connection() :
        // We can dump network-related messages through this logger
        // and be able to filter them later
        m_net(keywords::channel = "net"),
        // We also can separate statistic records in a different channel
        // in order to route them to a different sink
        m_stat(keywords::channel = "stat")
    {
    }

    void on_connected(std::string const& remote_addr)
    {
        // Add the remote address to both channels
        attrs::constant< std::string > addr(remote_addr);
        m_net_remote_addr = m_net.add_attribute("RemoteAddress", addr).first;
        m_stat_remote_addr = m_stat.add_attribute("RemoteAddress", addr).first;

        // Put message to the "net" channel
        BOOST_LOG(m_net) << "Connection established";
    }

    void on_disconnected()
    {
        // Put message to the "net" channel
        BOOST_LOG(m_net) << "Connection shut down";

        // Remove the attribute with the remote address
        m_net.remove_attribute(m_net_remote_addr);
        m_stat.remove_attribute(m_stat_remote_addr);
    }

    void on_data_received(std::size_t size)
    {
        BOOST_LOG(m_stat) << logging::add_value("ReceivedSize", size) << "Some data received";
    }

    void on_data_sent(std::size_t size)
    {
        BOOST_LOG(m_stat) << logging::add_value("SentSize", size) << "Some data sent";
    }
};
```

&emsp;&emsp;同时我们可以为每一个单独的日志记录设置channel，当使用[全局logger](#log.detailed.sources.global_storage)时，
这样是非常有用的。可以通过channel修改器或者一个特殊的宏来进行记录日志。例如：

```csharp
// Define a global logger
BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(my_logger, src::channel_logger_mt< >, (keywords::channel = "general"))

class network_connection
{
    std::string m_remote_addr;

public:
    void on_connected(std::string const& remote_addr)
    {
        m_remote_addr = remote_addr;

        // Put message to the "net" channel
        BOOST_LOG_CHANNEL(my_logger::get(), "net")
            << logging::add_value("RemoteAddress", m_remote_addr)
            << "Connection established";
    }

    void on_disconnected()
    {
        // Put message to the "net" channel
        BOOST_LOG_CHANNEL(my_logger::get(), "net")
            << logging::add_value("RemoteAddress", m_remote_addr)
            << "Connection shut down";

        m_remote_addr.clear();
    }

    void on_data_received(std::size_t size)
    {
        BOOST_LOG_CHANNEL(my_logger::get(), "stat")
            << logging::add_value("RemoteAddress", m_remote_addr)
            << logging::add_value("ReceivedSize", size)
            << "Some data received";
    }

    void on_data_sent(std::size_t size)
    {
        BOOST_LOG_CHANNEL(my_logger::get(), "stat")
            << logging::add_value("RemoteAddress", m_remote_addr)
            << logging::add_value("SentSize", size)
            << "Some data sent";
    }
};
```

&emsp;&emsp;我们注意到，修改channel是持续性的，除非channel的名称被reset，否则后续的日志都会属于新的channel。

>![Tip][tip-image] **小技巧**
>处于性能的原因，我们建议避免动态为每个日志记录设置channel名称。因为这回触发动态内存分配。
>如果可能的话，为每个channel分配不同的logger，可以避免这种额外开销。

### <a name="log.detailed.sources.exception_handling"></a>带有异常处理支持的logger
```csharp
#include <boost/log/sources/exception_handler_feature.hpp>
```

&emsp;&emsp;本程序库提供一个logger特性，允许用户在logger等级处理或者抑制异常。
[exception_handler](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/exception_handler.html)特征
增加一个```set_exception_handler```函数到logger中，允许设置当日志核心在filtering以及处理日志记录产生异常时可以调用的函数对象。
用户可以使用[程序库提供的适配器](#log.detailed.utilities.exception_handlers)来实现异常处理句柄。使用示例如下：

```csharp
enum severity_level
{
    normal,
    warning,
    error
};

// A logger class that allows to intercept exceptions and supports severity level
class my_logger_mt :
    public src::basic_composite_logger<
        char,
        my_logger_mt,
        src::multi_thread_model< boost::shared_mutex >,
        src::features<
            src::severity< severity_level >,
            src::exception_handler
        >
    >
{
    BOOST_LOG_FORWARD_LOGGER_MEMBERS(my_logger_mt)
};

BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(my_logger, my_logger_mt)
{
    my_logger_mt lg;

    // Set up exception handler: all exceptions that occur while
    // logging through this logger, will be suppressed
    lg.set_exception_handler(logging::make_exception_suppressor());

    return lg;
}

void logging_function()
{
    // This will not throw
    BOOST_LOG_SEV(my_logger::get(), normal) << "Hello, world";
}
```

>![Tip][tip-image] **小技巧**
>[日志核心](#log.detailed.core.core.exception_handling)和
>[sink前端](#log.detailed.sink_frontends.basic_services.exception_handling)
>同样支持设置异常处理句柄。

### <a name="log.detailed.sources.mixed_loggers"></a>包含混合特性的logger

```csharp
#include <boost/log/sources/severity_channel_logger.hpp>
```

&emsp;&emsp;本程序库同样提供
[severity_channel_logger](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/severity_channel_logger.html)
和[severity_channel_logger_mt](#http://www.boost.org/doc/libs/1_60_0/libs/log/doc/html/boost/log/sources/severity_channel_logger_mt.html)
他们将组合```severity level```和```channel```来描述logger。
```severity_channel_logger_mt```的用法：

```csharp
enum severity_level
{
    normal,
    notification,
    warning,
    error,
    critical
};

typedef src::severity_channel_logger_mt<
    severity_level,     // the type of the severity level
    std::string         // the type of the channel name
> my_logger_mt;


BOOST_LOG_INLINE_GLOBAL_LOGGER_INIT(my_logger, my_logger_mt)
{
    // Specify the channel name on construction, similarly as with the channel_logger
    return my_logger_mt(keywords::channel = "my_logger");
}

void logging_function()
{
    // Do logging with the severity level. The record will have both
    // the severity level and the channel name attached.
    BOOST_LOG_SEV(my_logger::get(), normal) << "Hello, world!";
}
```

### <a name="log.detailed.sources.global_storage"></a>全局存储的logger

```csharp
#include <boost/log/sources/global_logger_storage.hpp>
```

&emsp;&emsp;有时候通过一个日志对象来写日志不太方便。特别在函数风格的代码中，没有地方来存储logger。
在此时，如果有一个或多个方便访问的全局logger就很必要。

&emsp;&emsp;本程序库提供了一种声明全局logger的方法，就像访问```std::cout```一样。
事实上，任何logger都可以使用此特性，包括用户定义的logger。如果已经声明了全局logger，
可以确保在任何地方线程安全地访问这个logger实例，程序库保证全局logger实例是唯一的。
这样logging只需要在头中声明，然后就可以在任何模块中使用。

&emsp;&emsp;最简便地声明全局logger的方法是使用下面的宏
```csharp
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(my_logger, src::severity_logger_mt< >)
```

&emsp;&emsp;```my_logger```参数提供的logger的名称，用来获取logger实例。
第二个参数是logger的类型，在多线程的应用中，当logger可以被不同的线程访问时，用户一般会使用线程安全的logger。

&emsp;&emsp;如果需要传参数到logger的构造函数，有另外一个宏

```csharp
BOOST_LOG_INLINE_GLOBAL_LOGGER_CTOR_ARGS(
    my_logger,
    src::severity_channel_logger< >,
    (keywords::severity = error)(keywords::channel = "my_channel"))
```










<a name="detailed-attribute"></a>属性

<a name="detailed-expresion"></a>表达式

<a name="log.detailed.attributes.named_scope"></a>命名空间
<a name="log.detailed.attributes.related_components.attribute_value_set"></a>属性值集合



<a name="sink_frontends"></a>sink前端

<a name="log.detailed.sink_frontends.basic_services.exception_handling"></a>异常处理

<a name="sink_backends"></a>sink后端

<a name="log.detailed.sink_backends.syslog"></a>syslog后端
<a name="log.detailed.sink_backends.event_log"></a>Windows事件日志后端


<a name="log.detailed.sink_frontends.sync"></a>同步sink前端

<a name="log.detailed.sink_backends.text_ostream"></a>文本输出

<a name="log.detaild.sink_backends.text_file"></a>文本文件后端

<a name="log.detailed.sink_frontends.unlocked"></a>不加锁的sink前端

<a name="log.detailed.sink_frontends.basic_services.exception_handling"></a>异常处理句柄


<a name="log.detailed.core.core.exception_handling"></a>异常处理句柄


<a name="log.detailed.attributes.related_components.value_processing"></a>属性值抽取和访问

<a name="log.detailed.attributes.related_components.scoped_attributes"></a>作用域属性

<a name="log.detailed.expressions.attr"></a>具体属性

<a name="log.detailed.expressions.message"></a>message信息

<a name="log.detailed.expressions.attr_keywords"></a>定义属性关键词

<a name="log.detailed.expressions.formatters"></a>格式化表达式

<a name="log.detailed.expressions.predicates.has_attr"></a>属性存在性过滤器

<a name="log.detailed.expressions.formatters.conditional"></a>条件formatter

<a name="log.detailed.expressions.attr.fallback_policies"></a>客户化回退策略

<a name="log.detailed.expressions.predicates"></a>谓词表达式

<a name="log.detailed.utilities.setup.filter_formatter"></a>过滤器和formatter解析器

<a name="log.detailed.utilities.exception_handlers"></a>异常句柄

<a name="log.detailed.utilities.value_ref"></a>值引用wrapper

<a name="log.detailed.utilities.setup.convenience"></a>快捷函数

<a name="log.detailed.utilities.exception_handlers"></a>异常处理句柄



[boost_regex]: http://www.boost.org/doc/libs/release/libs/regex/index.html
[boost_xpressive]: http://www.boost.org/doc/libs/release/doc/html/xpressive.html
[boost_thread]: http://www.boost.org/doc/libs/release/doc/html/thread.html
[boost_asio]: http://www.boost.org/doc/libs/release/doc/html/boost_asio.html
[boost_phoenix]: http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/index.html
[boost_locale]: http://www.boost.org/doc/libs/release/libs/locale/doc/html/index.html
[note-image]: http://www.boost.org/doc/libs/1_60_0/doc/src/images/note.png
[tip-image]: http://www.boost.org/doc/libs/1_60_0/doc/src/images/tip.png
[warning-image]: http://www.boost.org/doc/libs/1_60_0/doc/src/images/warning.png
