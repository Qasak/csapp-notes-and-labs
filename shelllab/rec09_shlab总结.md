## 进程

+ `int fork(void)`
  + 创建一个和父进程一样的进程
  + 子进程和父进程状态相同：
    + 虚拟地址空间(内存)，包括堆栈
    + 寄存器，除了返回值(%eax/%rax)

+ `int exec()`
  + 替换当前进程的状态和上下文
    + 但是保留PID，文件描述符，信号上下文
  + 提供加载和运行其他程序的方法
    + 用新的程序替换当前运行内存映像
    + 用参数和环境变量重新设置栈
    + 从进入点开始执行
  + 成功的执行从不返回
  + 新加载程序的视角：好像之前的程序从没运行过
  + 更有用的变体`int execve()`
+ `coid exit(int status)`
  + 状态0正常退出(其他数字暗示出错)
  + 终止当前进程
  + OS释放堆空间和文件描述符资源
  + 变成僵死态
    + 必须wait被父进程回收(或父进程died过后init回收)
    + 信号被发送到父进程通知死亡
    + 回收者可以检测退出状态
+ `int wait(int *child_status)`
  + 挂起当前进程，直到其中一个子进程结束
  + 返回值时终止子进程的pid
    + 当wait返回pid>0,子进程被回收
    + 所有子进程资源释放
  + 如果child_status!=NULL,它指向的对象将被设置为状态指示子进程终止的原因
  + 更有用的变体`int waitpid()`

## 进程示例

```c

pid_t child_pid = fork();
if (child_pid == 0){
   /* only child comes here */   
    printf(“Child!\n”);   
    exit(0);
} else {   
 
    printf(“Parent!\n”);
}

```

output:

```
Child!
Parent!
or:
Parent!
Child!
```

```c
int status;
pid_t child_pid = fork();
if (child_pid == 0){
   /* only child comes here */   
    printf(“Child!\n”);   
    exit(0);
} else {   
 	waitpid(child_pid, &status,0);
    printf(“Parent!\n”);
}

```

output:

```
Child!
Parent!
```

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



### 信号处理 signal handler

+ 形如`void handler(int signum)`
+ 进程中的独立控制流
+ 根据返回恢复正常控制流
+ 适当的信号被触发时，可随时被调用

### `int sigsuspend(const sigset_t *mask)`

+ 挂起进程，直到收到行为是调用handler或终止进程的信号
+ 若捕获到信号，但会
  + 信号mask恢复成之前的状态

### SIGCHLD

