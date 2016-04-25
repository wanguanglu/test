# 在github的markdown文件中插入公式

&emsp;&emsp;在github上管理文档非常方便，自己也慢慢喜欢上了在github上编写文档。
然而当我尝试markdown文档中插入公式的时候，我发现这个事情就不是那么有趣了。

&emsp;&emsp;在经过一番搜索之后，从<http://blog.csdn.net/xiahouzuoxin/article/details/26478179>处找到了在markdown中插入公式的三种方法。

## 动态生成图片
&emsp;&emsp;其中方法一和方法二基本可以认为是一种方法，其基本思路是将公式传到公式解析服务器，生成图片，然后将图片插入到文档中。

&emsp;&emsp;方法一通过google chart服务器

```
http://chart.googleapis.com/chart?cht=tx&chl=\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})
```
**注意:** 由于markdown本身对```\```有转义，因此在需要写```\```的地方要写```\\```。
公式显示如下，

![测试公式](http://chart.googleapis.com/chart?cht=tx&chl=\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})

&emsp;&emsp;方法二很类似，将服务器改为forkosh服务器

```
http://www.forkosh.com/mathtex.cgi?\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}
```

&emsp;&emsp;公式显示如下

![测试公式](http://www.forkosh.com/mathtex.cgi?\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})

&emsp;&emsp;通过点击鼠标右键，可以看出来它们本质都是图片，这样的缺点就是
* 不是矢量图，在放大时清晰度降低
* 图片背景是白色，当页面背景是其他颜色时，会使得整个体验不好。

## 使用MathJax引擎
&emsp;&emsp;目前插入数学公式，很多主流网站逐渐切换到使用MathJax引擎。
[MathJax][mathjax-website]直接调用javascript使用矢量字库或者SVG文件来显示数学公式，
显示效果更好。

&emsp;&emsp;然而当我按照作者介绍的方式操作时，确无论如何都无法成功。
最后究其原因发现，作者提到的在markdown文件中插入javascript脚本是无法生效的。
这种情况在管理自己的网站时非常容易解决，只需要在自己的网站的模本html中添加上javascript引用就可以。
但是问题是在github网站上如何显示？还是无法解决。

### 使用油猴子（Greasemonkey）插件
&emsp;&emsp;既然在markdown文件中插入javascript脚本无法生效，那么剩下的办法就是只能是在外部加入javascript脚本。
这时调研发现了[Greasemonkey][greasemonkey-website]，这是一个神奇的东东，主流浏览器都有相关插件，
具体每个浏览器的安装方法可以参考[链接](http://meta.math.stackexchange.com/questions/4208/a-prototype-for-incremental-preview-updates)。

&emsp;&emsp;同时在[StackOverflow](http://stackoverflow.com/questions/11255900/mathjax-support-in-github-using-a-chrome-browser-plugin/11284227#11284227)上找到了这个问题的答案，同时也提供了可以运行的user script。

&emsp;&emsp;在chrome下安装了[TamperMonkey][tampermonkey-website]插件，然后添加user script

```javascript
// ==UserScript==
// @name           Run MathJax in Github
// @namespace      http://www.mathjax.org/
// @description    Runs MathJax on any page in github.com
// @include        http://github.com/*
// @include        https://github.com/*
// @include        http://www.wanguanglu.com/*
// @include        file:///*
// @run-at        document-start;
// ==/UserScript==

/*****************************************************************/

(function () {
  var script = document.createElement("script");
  script.type = "text/javascript";
  script.src  = "http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML";
  var parent = (document.head || document.body || document.documentElement);
  parent.appendChild(script);
})();
```

&emsp;&emsp;然而遇到的问题是不work啊。
我在我的个人网站中插入了一条公式。
```
$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$
```
&emsp;&emsp;然后使用Greasemonkey脚本，显示正常，具体可以参考[链接](http://www.wanguanglu.com/gossip)。
然而同样的公式在github里依然不显示，需要继续研究。
又经过一番深究，发现问题处在```http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML```，
这里进行了一些配置。不知是Greasemonkey的问题还是哪儿的问题，此处配置不稳定。

&emsp;&emsp;然后继续查看，在<http://docs.mathjax.org/en/latest/configuration.html#inline-config>处介绍了另外一种配置方法。
然后修改User script，修改之后的脚本如下
```javascript
// ==UserScript==
// @name           Run MathJax in Github
// @namespace      http://www.mathjax.org/
// @description    Runs MathJax on any page in github.com
// @include        http://github.com/*
// @include        https://github.com/*
// @include        http://www.wanguanglu.com/*
// @include        file:///*
// @run-at        document-start;
// ==/UserScript==

/*****************************************************************/

(function () {
    var mathconfig = document.createElement("script");
    mathconfig.type = "text/x-mathjax-config";
    mathconfig.text = [
        "MathJax.Hub.Config({",
            "extensions: ['tex2jax.js'],",
            "jax: ['input/TeX', 'output/HTML-CSS'],",
            "tex2jax: {",
                "inlineMath: [ ['$','$'], ['\\\\(','\\\\)'] ],",
                "displayMath: [ ['$$','$$'], ['\\\\[','\\\\]'] ],",
                "processEscapes: true",
            "},",
            "'HTML-CSS': { availableFonts: ['TeX'] }",
        "});"
    ].join("\n");
    var mathparent = (document.head || document.body || document.documentElement);
    mathparent.appendChild(mathconfig);
    var script = document.createElement("script");
    script.type = "text/javascript";
    script.src  = "http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML";
    var parent = (document.head || document.body || document.documentElement);
    parent.appendChild(script);
})();
```

&emsp;&emsp;然后依然不生效

## 安装chrome插件
最后在山穷水复之际，在google中搜到了一个[GitHub with MathJax](https://chrome.google.com/webstore/detail/github-with-mathjax/ioemnmodlmafdkllaclgeombjnmnbima/related)插件，安装之后，竟然奇迹般的成功了。虽然我还不知道它的原理，但是下面的这一行数学公式确实显示正常了

$$
x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}
$$

再测试一下行内公式$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$，也可以正常显示，搞定。

## latex math语法
&emsp;&emsp;下面学习一下latex math的一些主要语法，详细得可以参考[wiki][latex-math-wiki]。
### 上标下标
上标&emsp;&emsp;```x^y```&emsp;&emsp; $x^y$

下标&emsp;&emsp;```x_y```&emsp;&emsp; $x_y$

### 分式根式

分式&emsp;&emsp;```\frac{y}{x}```&emsp;&emsp; $\frac{y}{x}$

根式&emsp;&emsp;```\sqrt{x}```&emsp;&emsp; $\sqrt{x}$

高根式&emsp;&emsp;```\sqrt[n]{x}```&emsp;&emsp; $\sqrt[n]{x}$

### 上下划线

&emsp;```\hat{x}```&emsp;&emsp;  $\hat{x}$

&emsp;```\bar{x}```&emsp;&emsp; $\bar{x}$

&emsp;```\overline{x}```&emsp;&emsp; $\overline{x}$

&emsp;```\tilde{x}```&emsp;&emsp; $\tilde{x}$

## 参考
[1] <http://blog.csdn.net/xiahouzuoxin/article/details/26478179>

[2] <https://www.mathjax.org>

[3] <http://www.greasespot.net>

[4] <https://en.wikibooks.org/wiki/LaTeX/Mathematics>

[mathjax-website]: https://www.mathjax.org
[greasemonkey-website]:www.greasespot.net
[tampermonkey-website]:https://www.google.com.hk/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwjJ4qX2yqHMAhVIjZQKHVPABRoQFggeMAA&url=%68%74%74%70%73%3a%2f%2f%63%68%72%6f%6d%65%2e%67%6f%6f%67%6c%65%2e%63%6f%6d%2f%77%65%62%73%74%6f%72%65%2f%64%65%74%61%69%6c%2f%74%61%6d%70%65%72%6d%6f%6e%6b%65%79%2f%64%68%64%67%66%66%6b%6b%65%62%68%6d%6b%66%6a%6f%6a%65%6a%6d%70%62%6c%64%6d%70%6f%62%66%6b%66%6f%3f%68%6c%3d%7a%68%2d%43%4e&usg=AFQjCNGz6TBBSuW4e26RzTalotV4aygN5A
[latex-math-wiki]:https://en.wikibooks.org/wiki/LaTeX/Mathematics
