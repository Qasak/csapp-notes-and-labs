## Unix I/O

+ 文件到设备的优雅映射允许内核导出名为Unix I/O的简单接口：
  

+ 打开和关闭文件
  + open（）和close（）

+ 读写文件
  + read（）和write（）

+ 更改当前文件位置（seek）
  + 指示文件中要读或写的下一个偏移量
  + lseek（）

### 文件类型

+ 每个文件都有一个类型，指示其在系统中的角色
  + 常规文件：包含任意数据
    + 包括文本文件和二进制文件，内核不做区分。应用程序需要区分
  + 目录：相关文件组的索引
  + 套接字：用于与另一台计算机上的进程通信



+ 超出我们范围的其他文件类型
  + 命名管道（FIFO）
  + 符号链接
  + 字符设备和块设备

### 打开文件

+ 打开一个文件会通知内核您准备好访问该文件

```c
int fd;   /* file descriptor */

if ((fd = open("/etc/hosts", O_RDONLY)) < 0) {
   perror("open");
   exit(1);
}
```



+ 返回一个小整数标识的文件描述符
  + fd==-1表示发生错误

+ Linux shell创建的每个进程都是从与终端相关联的三个打开的文件开始的：

  + 0：标准输入（标准输入）
  + 1： 标准输出（标准输出）

  + 2： 标准错误（stderr）

### 关闭文件

关闭一个文件会通知内核您已经完成了对该文件的访问

```c
int fd;     /* file descriptor */
int retval; /* return value */

if ((retval = close(fd)) < 0) {
   perror("close");
   exit(1);
}
```

关闭一个已经关闭的文件会导致线程程序的灾难

寓意：始终检查返回代码，即使是对于`close()`之类的看似良性的函数也是如此

### 读文件

读取文件将字节从当前文件位置复制到内存，然后更新文件位置

```c
char buf[512];
int fd;       /* file descriptor */
int nbytes;   /* number of bytes read */

/* Open file fd ...  */
/* Then read up to 512 bytes from file fd */
if ((nbytes = read(fd, buf, sizeof(buf))) < 0) {
   perror("read");
   exit(1);
}
```



+ 返回从文件fd读入buf的字节数
  + 返回类型ssize_t是有符号整数
  + nbytes<0表示发生错误
  + 不足值（nbytes<sizeof（buf））是可能的，不是错误！

### 写文件

写入文件将字节从内存复制到当前文件位置，然后更新当前文件位置

```c
char buf[512];
int fd;       /* file descriptor */
int nbytes;   /* number of bytes read */

/* Open the file fd ... */
/* Then write up to 512 bytes from buf to file fd */
if ((nbytes = write(fd, buf, sizeof(buf)) < 0) {
   perror("write");
   exit(1);
}
```

+ 返回从buf写入文件fd的字节数
  + nbytes<0表示发生错误

  + 与读取一样，不足值是可能的，不是错误！