## 进程

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





