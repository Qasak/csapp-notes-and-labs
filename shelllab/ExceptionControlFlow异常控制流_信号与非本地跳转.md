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
| 11     | SIGSEGV  | Terminate & Dump   | Segmentation  violation(分段冲突) |
| 14     | SIGALRM  | Terminate          | Timer  signal                     |
| 17     | SIGCHLD  | Ignore             | Child  stopped or terminated      |

### 发送信号

+ 内核通过更新目标进程上下文中的某些状态来向目标进程发送（传递）信号
+ 内核由于以下原因之一发送信号
  + 内核检测到`系统事件`，如被零除（SIGFPE）或子进程终止（SIGCHLD）
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
    + 可使用`sigprocmask`函数设置和清除
    + 也称为信号掩码

### 进程组

+ 每个进程只属于一个进程组

### 用/bin/kill发送信号

+ /bin/kill程序向进程或进程组发送任意信号
  + /bin/kill–9 24818发送SIGKILL到进程24818
  + /bin/kill–9–24817将SIGKILL发送到进程组24817中的每个进程

### 键盘发送信号

键入ctrl-c（ctrl-z），内核向前台进程组中的每个作业发送SIGINT（SIGTSTP）。

### `kill`函数发送信号

```c
 kill(pid[i], SIGINT);
```

### 接收信号

+ 假设内核正在从异常处理程序返回(内核模式切换到用户模式，例如：从系统调用返回或完成了一次上下文切换)，并准备将控制权传递给进程p

+ 内核计算`pnb=pending&~blocked`

  + 进程p的挂起的非阻塞信号集合

+ `if (pnb == 0)`

  + 将控制权传递给p的逻辑流中的下一条指令

+ `else`

  + 在pnb中选择最小非零位k并强制进程p接收信号k

    信号的接收触发了p的一些动作

    对pnb中所有非零k重复上述步骤

    将控制权传递给p的逻辑流中的下一条指令

### 默认行为

+ 每种信号类型都有一个预定义的默认行为，即：
  + 进程终止
  + 进程终止并转储内存
  + 进程停止，直到被SIGCONT信号重启
  + 忽略

### 安装信号处理程序(Installing Signal Handlers)

+ signal函数修改与接收信号signum相关的默认操作(唯一的例外是SIGSTOP和SIGKILL

  它们的默认行为不可被修改)：

  + `handler_t *signal(int signum, handler_t *handler)`

+ 不同值的`handler`

  + SIG_IGN：忽略signum类型的信号

  + SIG_DFL：在接收到signum类型的信号时恢复为默认操作

  + 否则，handler是用户级信号处理程序的地址

    + 进程接收信号时的类型是signum时被调用

    + 称为“安装”处理程序

    + 执行处理程序被称为“捕捉”或“处理”信号

    + 当处理程序执行其return语句时，控制权会传回因接收到信号而中断的进程的控制流中的指令

```c
void sigint_handler(int sig) /* SIGINT handler */
{
    printf("So you think you can stop the bomb with ctrl-c, do you?\n");
    sleep(2);
    printf("Well...");
    fflush(stdout);
    sleep(1);
    printf("OK. :-)\n");
    exit(0);
}

int main()
{
    /* Install the SIGINT handler */
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
        unix_error("signal error");

    /* Wait for the receipt of a signal */
    pause();

    return 0;
}

```

### 信号处理程序作为并发流

+ 信号处理程序是与主程序并行运行的独立逻辑流（而不是进程）

### 嵌套信号处理程序

+ 处理程序可以被其他处理程序中断

### 信号的阻塞/解除阻塞

+ 隐式阻塞机制

  + 内核阻止当前正在处理的任何类型的挂起信号
  + e.g.,一个SIGINT处理程序不能被另一个SIGINT中断

+ 显示阻塞与解阻塞机制

  + `sigprocmask`函数

+ 支持函数

  + `signemptySet`–创建空集

    `sigfillset`–信号编号全集

    `sigaddset`–将信号编号添加到集合

    `sigdelset`–从集合中删除信号编号

### `sigprocmask`

+ `int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)`

行为依赖于how值

+ `SIG_BLOCK`set中的信号添加到blocked中(blocked=blocked | set)
+ `SIG_UNBLOCK`从blocked中删除set中的信号(blocked=blocked & ~set)
+ `SIG_SETMASK`block=set

若`oldset`非空，则`blocked`位向量之前的值保存在`oldset`中

### 临时阻塞信号

```c
    sigset_t mask, prev_mask;

    Sigemptyset(&mask);
    Sigaddset(&mask, SIGINT);

    /* Block SIGINT and save previous blocked set */
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask);

        /* Code region that will not be interrupted by SIGINT */

    /* Restore previous blocked set, unblocking SIGINT */
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL);

```

### 安全处理信号

+ 处理程序很棘手，因为它们与主程序并发并且共享相同的全局数据结构。
  + 共享数据结构可能会损坏
+ G0:处理程序尽可能简单
  + eg:设置一个全局标志(global flag)，然后返回
+ G1:处理程序中只调用`异步信号安全(async-signal-safe)`的函数
  + `printf,sprintf,malloc,exit`都不安全
+ G2:保存和恢复`errno`(许多Linux异步信号安全的函数出错返回时设置`errno`，处理程序中调用这种函数可能干扰主程序中其他依赖于`errno`的部分)
  + 这样其他处理程序就不会覆盖errno的值
+ G3:访问共享数据结构时临时阻塞所有信号
  + 防止可能的数据损坏
+ G4:用`volatile`声明全局变量
  + 避免编译器优化全局变量到寄存器，每次都要从内存读取
+ G5:用`sig_atomic_t`声明标志(global flags)
  + flag：只读或写的变量（例如flag=1，而不是flag++,flag=flag+10(它们是非原子指令)）
  + `sig_atomic_t`是原子的，而不需要临时阻塞信号，
  + `volatile sig_atomic_t`

### 异步信号安全

要么是

+ 可重入(所有变量保存在栈帧(只访问局部变量))

要么是

+ 不可被信号中断(**or non-interruptible by signals**)

常用的`异步信号安全`函数：

+ `_exit,write,wait,waitpid,slepp,kill`
+ `write`是唯一的异步信号安全的输出函数

常用的非异步信号安全函数：

+ `printf,sprintf,malloc,exit`

### 安全生成格式化输出

### 正确的信号处理

+ 挂起的信号不排队
  + 对于每种信号类型，一个bit表示信号是否处于挂起状态
  + 因此，最多有一个特定类型的挂起信号
+ 不能使用信号来计数事件，例如子进程终止
+ 必须等待所有终止的子进程
  + 将wait放入一个循环中以获取所有终止的子进程

```c
void child_handler2(int sig)
{
    int olderrno = errno;
    pid_t pid;
    while ((pid = wait(NULL)) > 0) {
        ccount--;
        Sio_puts("Handler reaped child ");
        Sio_putl((long)pid);
        Sio_puts(" \n");
    }
    if (errno != ECHILD)
        Sio_error("wait error");
    errno = olderrno;
}


```

### 可移植的信号处理

+ 不同版本的Unix可以有不同的信号处理语义
  + 一些较旧的系统在捕捉到信号后将操作恢复为默认值
  + 一些中断的系统调用可能返回`errno==EINTR`
  + 有些系统不会阻止正在处理的类型的信号

+ 解决办法:`sigaction`

```c
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* Restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

```



### 同步流以避免竞争



## 非本地跳转



## 总结

+ 信号提供进程级的异常处理
  + 能在用户程序中生成
  + 能通过信号处理程序声明影响
  + 小心编写信号处理程序

+ 非本地跳转在进程中提供了异常控制流