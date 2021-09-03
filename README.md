# No.js
一 简介：基于V8的JS运行时，只支持Linux5.5及以上<br/>
1 g++ -g -Iinclude/v8 -Iinclude/io_uring No.cc core/*.cc  core/ipc/*.cc -o No -lv8_monolith  -luring -ldl -Llib -pthread -std=c++14 -DV8_COMPRESS_POINTERS<br/>
2 软链到系统路径：sudo ln -s /Users/name/code/tmp/demo/No.js/No /usr/local/bin/No<br/>
3 执行test目录下的例子

二 调试<br/>
1 sudo su<br/>
2 echo "/core/%e.core.%p" > /proc/sys/kernel/core_pattern<br/>
3 ulimit -c unlimited<br/>
4 gdb ./No coredump文件的路径<br/>