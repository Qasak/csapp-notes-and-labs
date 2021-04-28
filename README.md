# all-about-csapp-labs
Solutions of *Computer Systems: A Programmer's Perspective* labs

> 如果你能解决它,那它就是一道习题,否则它就是一个研究题

CSAPP lab全部题解

> as [O'Hallaron](https://www.cs.cmu.edu/~droh/) said in 15-213 class "whatever you do, don't cheat. " 
>
> 仅供个人学习的记录使用,请独立完成所有lab!
>
> All solutions written by github user [Qasak](https://qasak.github.io/).    

## data_lab

[data_lab](https://github.com/Qasak/all-about-csapp-labs/blob/master/datalab/README.md)

实现简单的逻辑函数、二补函数和浮点函数，但使用的是C的一个高度受限的子集。例如，他们可能会被要求仅使用位级操作和直线代码计算数字的绝对值。

## bomb_lab

[bomb_lab](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/README.md)

运行时，它会提示用户键入6个不同的字符串。如果其中任何一个不正确，炸弹就会“爆炸”，打印错误消息并将事件记录在评分服务器上。学生必须通过分解和逆向工程来“拆除”他们自己独特的炸弹，以确定6个字符串应该是什么。该实验室教学生理解汇编语言，并强迫他们学习如何使用调试器。这也很有趣。CMU本科生中的传奇实验

## attack_lab

[attack_lab](https://github.com/Qasak/all-about-csapp-labs/blob/master/attacklab/README.md)

获得一对唯一的自定义生成的x86-64二进制可执行文件，称为targets，它们具有缓冲区溢出错误。一个目标易受代码注入攻击(Code Injection)。另一种是易受面向返回的编程攻击(return-oriented programming)。通过开发基于代码注入或面向返回的编程的漏洞来修改目标的行为。

## cache_lab

[cache_lab](https://github.com/Qasak/all-about-csapp-labs/blob/master/cachelab/README.md)

编写一个通用的缓存模拟器，然后优化一个小矩阵转置内核，使模拟缓存上的未命中数最小化。这个实验室使用Valgrind工具生成地址跟踪。

## Shell_lab

用作业控制(job control)构建一个简单的Unix shell。

它们被赋予了一个名为tsh.c（Tiny Shell）的框架，该框架实现了一些不太有趣的函数

然后要求实现提供作业控制的函数和信号处理程序。

## Malloc_lab

要求编写自己的存储分配器，特别是malloc、free和realloc函数的实现。跟踪驱动程序（mdriver）根据空间利用率和吞吐量来评估实现质量。

## Proxy_lab

构建一个简单的并发缓存Web代理，它接受来自客户端的HTTP 1.0请求，将它们转发到终端服务器，然后将响应发送回客户端。代理缓存终端服务器返回的对象，并尝试在将请求转发到终端服务器之前满足来自缓存的客户端的请求。

### ps

解决github图片无法加载的问题:用文本编辑器打开`C:\Windows\System32\drivers\etc\hosts`，在文件末尾处添加

```c
# GitHub Start

192.30.253.112    github.com 
192.30.253.119    gist.github.com
151.101.184.133    assets-cdn.github.com
151.101.184.133    raw.githubusercontent.com
151.101.184.133    gist.githubusercontent.com
151.101.184.133    cloud.githubusercontent.com
151.101.184.133    camo.githubusercontent.com
151.101.184.133    avatars0.githubusercontent.com
151.101.184.133    avatars1.githubusercontent.com
151.101.184.133    avatars2.githubusercontent.com
151.101.184.133    avatars3.githubusercontent.com
151.101.184.133    avatars4.githubusercontent.com
151.101.184.133    avatars5.githubusercontent.com
151.101.184.133    avatars6.githubusercontent.com
151.101.184.133    avatars7.githubusercontent.com
151.101.184.133    avatars8.githubusercontent.com

# GitHub End
```
