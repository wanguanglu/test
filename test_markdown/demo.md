# markdown使用方法介绍

&emsp;&emsp;本文档主要介绍markdown的一些使用方法。

## 标题的使用

一级标题可以使用```# 一级标题名```

二级标题可以使用```## 二级标题名```

以此类推


## 类Tex数学表达式

<script type="text/x-mathjax-config">
MathJax.Hub.Config({
    jax: ["input/TeX", "output/HTML-CSS"],
    tex2jax: {
        inlineMath: [ ['$', '$'] ],
        displayMath: [ ['$$', '$$']],
        processEscapes: true,
        skipTags: ['script', 'noscript', 'style', 'textarea', 'pre', 'code']
    },
    messageStyle: "none",
    "HTML-CSS": { preferredFont: "TeX", availableFonts: ["STIX","TeX"] }
});
</script>
<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script>

$$x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}$$

\\(x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}\\)
