# 同步：基础

Q: 线程化C程序中哪些变量是共享的？

A: 并不像“全局变量是共享的”和“栈变量是私有的”那么简单



定义：当且仅当多个线程引用变量x的某个实例时，才共享变量x。



需要回答以下问题：

+ 线程的内存模型是什么？

+ 变量的实例如何映射到内存？

+ 有多少线程可以引用这些实例？

## 线程内存模型

概念模型：

+ 多个线程在单个进程的上下文中运行

+ 每个线程都有自己独立的线程上下文
  + 线程ID、栈、栈指针、PC、条件码和GP(general-purpose)寄存器

+ 所有线程共享剩余的进程上下文
  + 进程虚拟地址空间的代码、数据、堆和共享库段
  + 打开文件和已安装的(信号)处理程序

在实际上，这一模式没有严格执行：

+ 寄存器值是真正独立和受保护的，但是…

+ 任何线程都可以读写任何其他线程的堆栈



**概念模型和操作模型之间的不匹配是混淆和错误的根源**



## 将变量实例映射到内存

全局变量

+ Def：在函数外部声明的变量

+ 虚拟内存只包含任何全局变量的一个实例



局部变量

+ Def：函数内部声明的变量，没有static属性

+ 每个线程栈包含每个局部变量的一个实例



局部静态变量

+ Def：在函数内部用static属性声明的变量

+ 虚拟内存只包含任何局部静态变量的一个实例。

### eg:badcnt

```asm
thread:
.LFB92:
	.cfi_startproc
	movq	(%rdi), %rcx
	testq	%rcx, %rcx
	jle	.L2
	movl	$0, %eax
.L3:
	movq	cnt(%rip), %rdx
	addq	$1, %rdx
	movq	%rdx, cnt(%rip)
	addq	$1, %rax
	cmpq	%rcx, %rax
	jne	.L3
.L2:
	movl	$0, %eax
	ret
	.cfi_endproc
```

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/badcnt-thread%E4%B8%AD%E7%9A%84%E5%BE%AA%E7%8E%AF.png)

### 并发执行

关键思想：一般来说，任何顺序一致的交织都是可能的，但有些会产生意想不到的结果！

+ $I_i$表示线程$i$执行指令$I$

+ $\%rdx_i$是线程$i$上下文中$\%rdx$的内容

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/thread%E6%89%A7%E8%A1%8C%E9%A1%BA%E5%BA%8F0.png)

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/thread%E6%89%A7%E8%A1%8C%E9%A1%BA%E5%BA%8F1.png)

### 进度图(progress graph)

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/%E8%BF%9B%E5%BA%A6%E5%9B%BE-%E4%B8%8D%E5%AE%89%E5%85%A8%E5%8C%BA.png)



关键部分（与某些共享变量相关）中的指令不应交错

这种交织发生在不安全区域的状态集

### 强制互斥(Enforcing Mutual Exclusion)

Q：我们如何保证轨道(trajectory 进度图的路径)安全？

A：我们必须同步线程的执行，这样它们就不会有不安全的轨迹。

+ i.e., 需要保证每个关键部分的互斥访问。



经典解决方案：

+ 信号灯/信号量(semaphores)（Edsger Dijkstra）



其他方法（超出我们的范围）

+ 互斥锁(mutex)和条件变量(condition variables)（Pthreads）

+ 监视器（Java）

## 信号量

信号量：非负全局整数同步变量。由P和V操作控制。(P V : 系统调用)

P（s）

+ 如果s非零，则将s减1并立即返回。
  + 测试和减量操作以原子方式（不可分割地）发生

+ 如果s为零，则挂起线程，直到s变为非零，然后通过V操作重新启动线程。
+ 重新启动后，P操作递减s，并将控制权返回给调用者。

> 若信号量值非零，则减一，并继续
>
> 若信号量值为零，则等待值被V操作增加。某处的V操作增加s后，P操作将s减一，返回控制权给调用者

V（s）：

+ 将s增加1。
  + 增量操作以原子方式进行

+ 如果在P操作中有任何线程被阻塞，等待s变为非零，则**只重新启动其中一个线程**，然后通过递减s来完成其P操作。(以不可假定的顺序)

>  V操作从不阻塞，它首先增加信号量值s，然后再队列中选取一个阻塞的信号，重启它

**信号量不变量**：（s>=0）





## 使用信号量来互斥

基本思路：

+ 将一个唯一的信号量互斥量（最初为1）与每个共享变量（或相关的共享变量集）相关联。

+ 用`P(mutex)` 和 `V(mutex)`操作。

(P V是系统调用，有开销)

术语：

+ 二进制信号量：值总是0或1的信号量

+ 互斥锁(mutex)：用于互斥的二进制信号量
  + P操作：“锁定”互斥锁
  + V操作：“解锁”或“释放”互斥锁
  + “保持”互斥锁：锁定但尚未解锁。

+ 计数信号量：用作一组可用资源的计数器。

## 互斥锁原理

![img](https://github.com/Qasak/csapp-notes-and-labs/blob/master/proxylab/mutex%E4%BA%92%E6%96%A5%E9%94%81%E5%8E%9F%E7%90%86.png)

