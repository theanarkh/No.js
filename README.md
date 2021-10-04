# No.js

<img src="https://img-blog.csdnimg.cn/4a6c5376c93d43baa090fdefecbb65a2.png" /> 
一 简介：基于V8的JS运行时，只支持Linux5.5及以上  
1 执行make编译  
2 ./No your js file or 软链到系统路径：sudo ln -s /Users/name/code/tmp/demo/No.js/No /usr/local/bin/No  
3 执行test目录下的例子

二 调试  
1 选择代码打断点  
2 修改launch.json的args配置为你执行的文件  
3 点击vscode左侧的小虫子开始调试  

三 core dump 排查  
1 sudo su  
2 echo "/core/%e.core.%p" > /proc/sys/kernel/core_pattern  
3 exit && ulimit -c unlimited  
4 gdb ./No coredump文件的路径  

四 文章  
[No.js---基于V8和io_uring的JS运行时](https://zhuanlan.zhihu.com/p/407085340)
