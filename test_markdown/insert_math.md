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

&emsp;&emsp;然而遇到的问题是不work啊。
我在我的个人网站中插入了一条公式。
```
$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$
```
&emsp;&emsp;然后使用Greasemonkey脚本，显示正常，具体可以参考链接<www.wanguanglu.com/gossip>。
然而同样的公式在github里依然不显示，需要继续研究。

$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$



## 参考
[1] <http://blog.csdn.net/xiahouzuoxin/article/details/26478179>

[2] <https://www.mathjax.org>
[mathjax-website]:https://www.mathjax.org

[3] <www.greasespot.net>
[greasemonkey-website]:www.greasespot.net
