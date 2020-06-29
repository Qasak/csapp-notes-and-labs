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

