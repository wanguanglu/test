## 在github的markdown文件中插入公式

&emsp;&emsp;在github上管理文档非常方便，自己也慢慢喜欢上了在github上编写文档。
然而当我尝试markdown文档中插入公式的时候，我发现这个事情就不是那么有趣了。

&emsp;&emsp;在经过一番搜索之后，从<http://blog.csdn.net/xiahouzuoxin/article/details/26478179>处找到了在markdown中插入公式的三种方法。
其中方法一和方法二基本可以认为是一种方法，其基本思路是将公式传到公式解析服务器，生成图片，然后将图片插入到文档中。
例如通过google chart服务器

```
http://chart.googleapis.com/chart?cht=tx&chl=\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})
```
公式显示如下，注意由于markdown本身对```\```有转义，因此在需要写```\```的地方要写```\\```。

![测试公式](http://chart.googleapis.com/chart?cht=tx&chl=\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})

类似的方法可以使用forkosh服务器

```
http://www.forkosh.com/mathtex.cgi?\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a}
```

公式显示如下

![测试公式](http://www.forkosh.com/mathtex.cgi?\\Large x=\\frac{-b\\pm\\sqrt{b^2-4ac}}{2a})











### 参考
[1] <http://blog.csdn.net/xiahouzuoxin/article/details/26478179>
