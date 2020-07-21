## 并发编程的问题

人类的思维倾向于循序渐进

时间的概念常常是误导人的

考虑计算机系统中所有可能的事件序列至少容易出错，而且常常是不可能的

+ 并发程序的经典问题：
  + 竞争：结果取决于系统中其他地方的任意调度决策
    + 谁得到飞机上最后一个座位？
  + 死锁：不正确的资源分配阻止了前进
    + 例如：交通堵塞
  + 活锁/饥饿/公平性：外部事件和/或系统调度决策可以阻止子任务的进度
    + 人们总是在你前面排队

## 迭代服务器

+ 迭代服务器一次处理一个请求

第二个客户端的请求阻塞在哪里？

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E5%AE%A2%E6%88%B7%E7%AB%AF%E9%98%BB%E5%A1%9E%E5%9C%A8read.png)

+ `connect`调用返回
  + 尽管连接上了，但并没有`accepted`
  + 服务端TCP管理器将请求排队
  + 这称作"TCP监听积压"特性
+ 调用`rio_written`返回
  + 服务端管理器缓存数据

+ 调用`rio_readlineb`阻塞
  + 服务端尚未写入(客户端)要读取的任何内容

## 并发服务器

### 进程

基于进程的服务器的issues

+ 侦听服务器进程必须获取僵尸子进程
  + 以避免致命的内存泄漏

+ 父进程必须关闭它的connfd副本
  + 内核为每个套接字/打开的文件保留引用计数
  + fork后，`refcnt（connfd）=2`
  + 连接只有`refcnt（connfd）=0`之后才会关闭

#### 基于进程的服务器的优缺点(pros and cons)

+同时处理多个连接

+清楚的模型

描述符（否）

文件表（是）

全局变量（否）

+简单明了

–过程控制的额外开销

–在进程之间共享数据非常重要

需要IPC（进程间通信）机制

FIFO（命名管道）、system v共享内存和信号量





### 事件

#### 基于事件的服务器的优缺点

+一个逻辑控制流和地址空间。

+可以用调试器单步执行。

+没有进程或线程控制开销。

>  高性能网络服务器和搜索引擎的设计选择。例如。Node.js nginx Tornado



–比基于进程或线程的设计复杂得多

–难以提供细粒度的并发性

+ E、 g.，如何处理部分HTTP请求

–无法利用多核

+ 单线程控制



### 线程

+ 传统视角

进程=进程上下文+(代码，数据，和栈)

代码，数据，和栈=栈+SP+共享库+brk+运行时堆+读/写数据+只读代码/数据

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E8%BF%9B%E7%A8%8B.png)

+ 另一个视角



进程上下文=程序上下文+内核上下文

+ 程序上下文=数据寄存器+状态码(condition code)+栈指针(SP)+程序技术器(PC)
+ 内核上下文=虚拟内存数据结构+描述符表+断点

线程上下文=进程上下文-内核上下文

进程=线程+(代码，数据和内核代码)

线程=栈+线程上下文

线程上下文=程序上下文

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E7%BA%BF%E7%A8%8B.png)

### 一个具有多线程的进程

多个线程可以与一个进程相关联

每个线程都有自己的逻辑控制流

每个线程共享相同的代码、数据和内核上下文

每个线程都有自己的局部变量堆栈

但不受其他线程的保护

每个线程都有自己的线程id（TID）

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E4%B8%80%E4%B8%AA%E5%85%B7%E6%9C%89%E5%A4%9A%E7%BA%BF%E7%A8%8B%E7%9A%84%E8%BF%9B%E7%A8%8B.png)

### 线程:逻辑视角

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E4%B8%8E%E7%BA%BF%E7%A8%8B%E7%9B%B8%E5%85%B3%E7%9A%84%E7%BA%BF%E7%A8%8B%E5%BD%A2%E6%88%90%E5%AF%B9%E7%AD%89%E6%B1%A0.png)

## 线程 vs. 进程

+ 线程和进程有多像?
  + 每个都有自己的逻辑控制流
  + 每一个都可以与其他内核并发运行（可能在不同的内核上）
  + 每个都是上下文切换的

+ 线程和进程的区别？
  + 线程共享所有代码和数据（除了本地堆栈）
  + 进程（通常）不会
  + 线程比进程开销少一些

+ 进程控制（创建和获取）的成本是线程控制的两倍

Linux中的数据：

+ 约20k个周期来创建和回收一个进程

+ 10K周期（或更少）来创建和回收一个线程

### `Posix` 线程(`Pthread`)接口

+ Pthreads:60个操作C程序线程的函数的标准接口

+ 创建和获取线程
  + `pthread_create（）`
  + `pthread_join（）`

+ 确定线程ID
  + `pthread_self（）`

+ 终止线程
  + `pthread_cancel（）`
  + `pthread_exit（）`
  + `exit（）`[终止所有线程]，RET[终止当前线程]

+ 同步访问共享变量
  + `pthread_mutex_init`

  + `pthread_mutex_[un]lock`

## 线程服务器issues

+ #### 必须运行“detached”以避免内存泄漏

  + 在任何时候，线程要么是可joinable，要么是detached
  + joinable线程可以被其他线程Kill或回收
  + 必须回收（使用`pthread_join`）以释放内存资源
  + detached线程不能被其他线程捕获或终止
  + 终止时自动获取资源
  + 默认状态是joinable
  + 使用`pthread_detach（pthread_self（））`进行detach

+ #### 必须小心避免无意间的共享

  + 例如，传递指向主线程栈的指针
  + `Pthread_create（&tid，NULL，thread，（void*）&connfd）；`
    + eg：![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E7%BA%BF%E7%A8%8Becho.png)
    + 传递**指向堆**的单独分配区域的指针
    + 如果不这样(选择直接传主线程的地址)，可能引起竞争(我们假设了thread解引用在主线程的下一次循环开始前完成)

+ #### 线程调用的所有函数都必须是线程安全的