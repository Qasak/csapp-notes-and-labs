## 异常

+ 异常是指控制权转移到操作系统内核以响应某些事件（即处理器状态的变化）
  + Kernel是操作系统驻留内存的部分
  + ![image-20200629181634103](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\exception.png)

![image-20200629182838368](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\exception table.png)

## 异常的类别

| 类别          | 原因              | 异步/同步 | 返回行为         |
| ------------- | ----------------- | --------- | ---------------- |
| 中断interrupt | 来自I/O设备的信号 | 异步      | 下一条指令       |
| 陷阱trap      | 故意的异常        | 同步      | 下一条指令       |
| 故障fault     | 潜在可恢复错误    | 同步      | 可能返回当前指令 |
| 终止abort     | 不可恢复错误      | 同步      | 不返回           |

### 中断

+ 由处理器外部事件引发

  + 设置处理器的中断引脚
  + 处理程序(handler)返回”下一条“指令

+ 例如：

  + 定时器中断

    + Linux的0号中断

    + 每隔几毫秒，外部时钟芯片触发一次
    + 内核用来从用户程序收回控制权(内核判断进程运行了足够长时间，切换到一个新的进程)
    + “时钟中断”是整个操作系统的脉搏

  + 外部设备I/O中断

    + 按下ctrl-c
    + 一个packet从网络发来
    + 磁盘收到数据

硬件中断不是由任何一条专门指令造成的，这个意义上讲，它是异步的

## 同步异常

+ 执行当前指令导致

### 陷阱

+ 故意的
+ 例如：**系统调用**，断点陷阱，特殊指令
+ 返回"下一条"指令

#### 系统调用

每x86-64的系统调用都有唯一id

eg:

| **Number** | **Name** | **Description**        |
| ---------- | -------- | ---------------------- |
| 0          | read     | Read file              |
| 1          | write    | Write file             |
| 2          | open     | Open file              |
| 3          | close    | Close file             |
| 4          | stat     | Get info  about file   |
| 57         | fork     | Create process         |
| 59         | execve   | Execute a program      |
| 60         | _exit    | Terminate process      |
| 62         | kill     | Send signal to process |

##### 实例：打开文件

```asm
00000000000e5d70 <__open>:
...
e5d79:   b8 02 00 00 00      mov  $0x2,%eax  # open is syscall #2
e5d7e:   0f 05               syscall         # Return value in %rax
e5d80:   48 3d 01 f0 ff ff   cmp  $0xfffffffffffff001,%rax 
...
e5dfa:   c3                  retq
```

+ %rax 保存系统调用码

+ 其他参数在 %rdi, %rsi, %rdx, %r10, %r8, %r9

+ 返回值在  %rax

+ 若 `errno`为负数，则说明有错误

### 故障

+ 非故意，潜在可恢复
+ 例如：缺页(page fault，可恢复)，保护错误(protection faults,不可恢复)，浮点异常
+ 重新执行故障指令("current")或终止(abort)

#### 缺页

+ 页：虚拟内存一个连续的块(4KB)

+ eg:用户写入的页面没在物理内存，而在磁盘上

+ ```c
  int a[1000];
  main ()
  {
      a[500] = 13;
  }
  ```

+ ```asm
  80483b7:	c7 05 10 9d 04 08 0d 	movl   $0xd,0x8049d10
  ```

![image-20200629185703963](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\fault.png)

#### 无效内存引用

+ abort

+ ```c
  int a[1000];
  main ()
  {
      a[5000] = 13;
  }
  
  ```

+ ```asm
  80483b7:	c7 05 60 e3 04 08 0d 	movl   $0xd,0x804e360
  ```

+ 发送`SIGSEGV`给用户进程
+ 用户进程退出 with "segmentation fault"

![image-20200629190015182](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\invalid_mem_ref.png)



### 终止

+ 非故意，且不可恢复
+ 比如：非法指令，奇偶校验错误(parity error)，机器检查(machine check)
+ 终止当前程序

## 进程

+ 定义：一个进程是一个运行着的程序的实例
+ definition: A process is an instance of a running program.
  + 计算机科学中最意义深远的概念之一

+ 进程给每个程序提供了两个关键抽象
  + **逻辑控制流**Logical control flow
    + 每个程序看起来独占CPU
    + 由内核的*上下文切换(context switching)*机制提供
  + **私有地址空间**Private address space
    + 每个程序看起来独占使用内存
    + 给正在运行的程序一个***状态***
    + 由内核的*虚拟内存(virtual memory)*机制提供

![image-20200629190516621](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\mem.png)![image-20200629190522887](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\cpu.png)

### 多进程：假象

+ 计算机同时运行许多进程
  + 多用户应用
    + 网页浏览器，email终端，编辑器...
  + 后台任务
    + 监视网络 & I/O设备

eg：在linux终端敲top

#### 多进程：传统实现

![image-20200629191333229](C:\Users\qasak\AppData\Roaming\Typora\typora-user-images\multi-processing.png)

+ 单处理器并发执行多个进程

  § 进程执行交错（多任务）

  § 由虚拟内存系统管理地址空间

  § 没有在执行的进程把寄存器值保存在内存

  § 当切换地址空间(上下文切换)时，从内存加载寄存器值

### 上下文切换

+ 进程由内核管理
  + 内核不是一个单独的进程，内核不是一个单独的进程，而是作为某个现有进程的一部分运行。
+ 控制流通过上下文切换从一个进程传递到另一个进程



## 进程控制

### 系统调用错误处理

+ 遇到错误，Linux系统级函数通常返回-1，设置全局变量`errno`以指示原因

+ 硬性规定（Hard and fast rule）
  + 必须检查每个系统调用级函数的返回状态
  + 唯一的例外是有些函数返回`void`,比如说exit和free

+ eg:

  ```c
      if ((pid = fork()) < 0) {
          fprintf(stderr, "fork error: %s\n", strerror(errno));
          exit(0);
      }
  ```

> ```
> #include <errno.h>
> ```
>
> C 库函数 **char \*strerror(int errnum)** 从内部数组中搜索错误号 **errnum**，并返回一个指向错误消息字符串的指针。**strerror** 生成的错误字符串取决于开发平台和编译器。

### 错误报告函数

```c
void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
```

+ Stevens-style错误包装器

  ```c
  pid_t Fork(void)
  {
      pid_t pid;
  
      if ((pid = fork()) < 0)
          unix_error("Fork error");
      return pid;
  }
  ```

### 获取进程IDs

+ **pid_t** **getpid**(void)
  + 返回当前进程PID

+ **pid_t** **getppid**(void)
  + 返回父进程PID

### 创建与终止进程

在程序员看来，进程有三种状态

+ 运行
  + 进程要么正在执行，要么等待执行，最终将由内核调度（即选择执行）
+ 暂停(stopped)
  + 进程执行暂停，除非另行通知，否则不会安排执行
+ 终止(terminated)
  + 进程永久停止

### 终止进程

+ 进程因为以下三种原因被终止
  + 接收默认动作为终止的信号
  + 从主程序(main routine)返回
  + 调用`exit`函数
+ `void exit(int status)`
  + 终止并以`status`状态退出
  + 约定：正常返回状态为0，出错时为非零
  + 另一种显式设置退出状态的方法是从主程序返回一个整数值
+ `exit`被调用一次但永远不返回

### 创建进程

+ 父进程通过调用`fork`创建新的运行的子进程
+ `int fork(void)`
  + 将0返回给子进程，将子进程的PID返回给父进程
  + 子与父几乎相同
    + 子进程获取父进程虚拟地址空间的相同（但独立）副本
    + 子进程获取父进程打开文件描述符的相同副本
    +  子进程的PID与父进程不同

### `fork`例子：

```c
int main()
{
    pid_t pid;
    int x = 1;

    pid = Fork(); 
    if (pid == 0) {  /* Child */
        printf("child : x=%d\n", ++x); 
	exit(0);
    }

    /* Parent */
    printf("parent: x=%d\n", --x); 
    exit(0);
}

```

### 进程图

```c
void fork5()
{
    printf("L0\n");
    if (fork() == 0) {
        printf("L1\n");
        if (fork() == 0) {
            printf("L2\n");
        }
    }
    printf("Bye\n");
}

```

<img src="C:\Users\qasak\Documents\GitHub\all-about-csapp-labs\shelllab\process_graph0.png" style="zoom:38%;" />



### 回收子进程

当一个进程由于某种状态终止，内核并不是把它从系统清除，而是保持在已终止状态，直到它被父进程回收(reaped)

如果父进程没有回收它的僵死子进程就结束了，内核会安排init进程取回收他们(init PID：1，是所有进程的祖先)。

不过，长时间运行的程序，比如shell或服务器，总是应该回收它们的僵死子进程

即使僵死子进程没有运行，也消耗系统的内存资源

回收：

+ 父进程终止子进程（使用wait或waitpid）

### `wait` 与子进程同步

+ **int** **wait(int**  \****child_status***)
  + 挂起当前进程，直到它的一个子进程终止
  + 返回值是终止的子进程的pid

```c
void fork9() {
    int child_status;

    if (fork() == 0) {
        printf("HC: hello from child\n");
	exit(0);
    } else {
        printf("HP: hello from parent\n");
        wait(&child_status);
        printf("CT: child has terminated\n");
    }
    printf("Bye\n");
}

```

![img](https://github.com/Qasak/all-about-computer-system/blob/master/shelllab/%E5%AD%90%E8%BF%9B%E7%A8%8B%E5%90%8C%E6%AD%A5Synchronizing%20with%20Children.png)

+ 多个子进程的完成将是任意顺序的
+ 可以使用宏WIFEXITED和WEXITSTATUS获取有关退出状态的信息

```c
void fork10() {
    pid_t pid[N];
    int i, child_status;

    for (i = 0; i < N; i++)
        if ((pid[i] = fork()) == 0) {
            exit(100+i); /* Child */
        }
    for (i = 0; i < N; i++) { /* Parent */
        pid_t wpid = wait(&child_status);
        if (WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d\n",
                   wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminate abnormally\n", wpid);
    }
}

```

```
Child 196 terminated with exit status 100
Child 197 terminated with exit status 101
Child 198 terminated with exit status 102
Child 199 terminated with exit status 103
Child 200 terminated with exit status 104
```



### `waitpid`:等待特定进程

```c
void fork11() {
    pid_t pid[N];
    int i;
    int child_status;

    for (i = 0; i < N; i++)
        if ((pid[i] = fork()) == 0)
            exit(100+i); /* Child */
    for (i = N-1; i >= 0; i--) {
        pid_t wpid = waitpid(pid[i], &child_status, 0);
        if (WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d\n",
                   wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminate abnormally\n", wpid);
    }
}

```



### `execve`:加载并允许程序

+ **int** execve(char \*filename, char \*argv[], char \*envp[])

+ 在当前进程中加载并允许

  + 可执行文件 `filename`

    + 可以是目标文件(object file)或以`# ! interpreter`开头的脚本文件(script file)

      (e.g., #! /bin/bash)

  + 参数表`argv`

    + 按惯例`argv[0]=filename`

  + 环境变量表`envp`

    + "name=value"字符串(e.g., USER=droh)
    + `getenv`,`putenv`,`printenv`(`#include<stdlib.h>`)

+ 覆盖code,data和stack

  + 保留PID，打开文件和信号上下文

+ 一次调用，永不返回

  + 除非有报错(例如找不到`filename`)

## 总结

+ 异常
  + 需要非标准控制流的事件
  + 外部（中断）或内部（陷阱和故障）产生
+ 进程
  + 在任何给定时间，系统都有多个活动进程
  + 在一个内核上一次只能执行一个
  + 每个进程看起来完全控制了处理器+私有内存空间

### 进程状态补充

+ R**(TASK_RUNNING)**:可执行

很多操作系统教科书将正在CPU上执行的进程定义为RUNNING状态、而将可执行但是尚未被调度执行的进程定义为READY状态，这两种状态在linux下统一为 TASK_RUNNING状态。

+ S **(TASK_INTERRUPTIBLE)**可中断的睡眠

这些进程的task_struct结构被放入对应事件的等待队列中。当这些事件发生时（由外部中断触发、或由其他进程触发），对应的等待队列中的一个或多个进程将被唤醒。

+ D**(TASK_UNINTERRUPTIBLE)**不可中断的睡眠

在进程对某些硬件进行操作时（比如进程调用read系统调用对某个设备文件进行读操作，而read系统调用最终执行到对应设备驱动的代码，并与对应的物理设备进行交互），可能需要使用TASK_UNINTERRUPTIBLE状态对进程进行保护，以避免进程与设备交互的过程被打断，造成设备陷入不可控的状态。这种情况下的TASK_UNINTERRUPTIBLE状态总是非常短暂的，通过ps命令基本上不可能捕捉到。

linux系统中也存在容易捕捉的TASK_UNINTERRUPTIBLE状态。执行vfork系统调用后，父进程将进入TASK_UNINTERRUPTIBLE状态，直到子进程调用exit或exec（参见《神奇的vfork》）。
通过下面的代码就能得到处于TASK_UNINTERRUPTIBLE状态的进程：

\#include  void main() { if (!vfork()) sleep(100); }

+ T**(TASK_STOPPED or TASK_TRACED)**暂停或跟踪

向进程发送一个SIGSTOP信号，它就会因响应该信号而进入TASK_STOPPED状态（除非该进程本身处于TASK_UNINTERRUPTIBLE状态而不响应信号）。（SIGSTOP与SIGKILL信号一样，是非常强制的。不允许用户进程通过signal系列的系统调用重新设置对应的信号处理函数。）
向进程发送一个SIGCONT信号，可以让其从TASK_STOPPED状态恢复到TASK_RUNNING状态。

当进程正在被跟踪时，它处于TASK_TRACED这个特殊的状态。“正在被跟踪”指的是进程暂停下来，等待跟踪它的进程对它进行操作。比如在gdb中对被跟踪的进程下一个断点，进程在断点处停下来的时候就处于TASK_TRACED状态。而在其他时候，被跟踪的进程还是处于前面提到的那些状态。

+ Z**(TASK_DEAD - EXIT_ZOMBIE)**僵尸进程

进程在退出的过程中，处于TASK_DEAD状态。

在这个退出过程中，进程占有的所有资源将被回收，除了task_struct结构（以及少数资源）以外。于是进程就只剩下task_struct这么个空壳，故称为僵尸。
之所以保留task_struct，是因为task_struct里面保存了进程的退出码、以及一些统计信息。而其父进程很可能会关心这些信息。比如在shell中，$?变量就保存了最后一个退出的前台进程的退出码，而这个退出码往往被作为if语句的判断条件。
当然，内核也可以将这些信息保存在别的地方，而将task_struct结构释放掉，以节省一些空间。但是使用task_struct结构更为方便，因为在内核中已经建立了从pid到task_struct查找关系，还有进程间的父子关系。释放掉task_struct，则需要建立一些新的数据结构，以便让父进程找到它的子进程的退出信息。

父进程可以通过wait系列的系统调用（如wait4、waitid）来等待某个或某些子进程的退出，并获取它的退出信息。然后wait系列的系统调用会顺便将子进程的尸体（task_struct）也释放掉。
子进程在退出的过程中，内核会给其父进程发送一个信号，通知父进程来“收尸”。这个信号默认是SIGCHLD，但是在通过clone系统调用创建子进程时，可以设置这个信号。

通过下面的代码能够制造一个EXIT_ZOMBIE状态的进程：

\#include  void main() { if (fork()) while(1) sleep(100); }