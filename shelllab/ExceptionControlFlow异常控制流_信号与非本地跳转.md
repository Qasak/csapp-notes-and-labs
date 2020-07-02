# 异常控制流：信号与非本地跳转

异常控制流(ECF)存在于系统的每个层次

+ 异常
+ 进程上下文切换
+ 信号
+ 非本地跳转

+ Linux 进程层次结构

  ![img](https://github.com/Qasak/all-about-computer-system/blob/master/shelllab/Linux_Process_Hierarchy.png)

## shells

`shellex.c`问题：

+ 后台job

  + 终止时将会编程僵死进程
  + 永远不会被回收，因为shell（通常）不会终止
  + 会造成内存泄漏，导致内核内存不足

+ 解决办法：异常控制流

  + 内核将中断正常进程，以便在后台进程完成时向我们发出警报
  + 在Unix这种机制叫信号

  

## 信号

+ 信号是一个小消息，它通知进程系统中发生了某种类型的事件
+ 类似于异常和中断
+ 从内核（有时应另一个进程的请求）发送到进程
+ 信号类型由小整数ID标识（1-30）
+ 信号中唯一的信息是它的ID和它到达的事实

| **ID** | **Name** | **Default Action** | **Corresponding Event**           |
| ------ | -------- | ------------------ | --------------------------------- |
| 2      | SIGINT   | Terminate          | User  typed ctrl-c                |
| 9      | SIGKILL  | Terminate          | Kill  program (不能覆盖或忽略)    |
| 11     | SIGSEGV  | Terminate          | Segmentation  violation(分段冲突) |
| 14     | SIGALRM  | Terminate          | Timer  signal                     |
| 17     | SIGCHLD  | Ignore             | Child  stopped or terminated      |

### 发送信号

+ 内核通过更新目标进程上下文中的某些状态来向目标进程发送（传递）信号
+ 内核由于以下原因之一发送信号
  + 内核检测到系统事件，如被零除（SIGFPE）或子进程终止（SIGCHLD）
  + 另一个进程调用 `kill` 系统调用显式地请求内核向目标进程发送信号

### 接收信号

+ 目标进程被内核强制以某种方式对信号的传递做出反应

+ 反应方式
  + *忽略*
  + *终止*
  + *捕获*通过执行用户级函数*signal handler*
    + 类似于为响应异步中断而调用的硬件异常处理程序

### 挂起和阻塞信号

+ 如果信号已发送但尚未接收，则该信号处于*挂起*状态
  + 任何特定类型的挂起信号最多只能有一个
  + 重要提示：信号不会排队
    + 如果一个进程有一个k类型的挂起信号，那么发送到该进程的k类型的后续信号将被丢弃
+ 进程可以*阻塞*某些信号的接收
  + 被阻塞的信号可以被传送，但在信号被解除阻塞之前不会被接收
+ 最多接收一次挂起的信号

### 挂起/阻塞 位

+ 内核在每个进程的上下文中维护挂起的和阻塞的位向量
  + `pending`表示挂起信号的集合
    + 当传递k类型的信号时，内核在pending中设置位k
    + 当接收到k类型的信号时，内核清除pending中的位k
  + `blocked`表示被阻塞的信号的集合

## 非本地跳转



