## 客户端服务器事务

+ 大多数网络应用都基于客户端服务器模型
  + 一个服务器进程和一个或多个客户端进程
  + 服务器控制一些资源
  + 服务器通过给客户端操作资源来提供服务
  + 服务器被客户端的请求激活(类似自动售货机)
  + 

## 计算机网络

+ 网络是一个由盒子和电线组成的按地理位置排列的层次系统
  + SAN(System area network)跨越集群或机房
    + 交换式以太网，Quadrics QSW
  + LAN(Local area network)跨越建筑物或校园
    + 以太网是最有名的例子
  + WAN(Wide are network)跨越国家或世界
    + 典型的高速点对点电话线
+ 因特网是一组相互连接的网络
  + 全球IP互联网（大写“I”）是internet最著名的例子（小写“I”）

## 最底层：以太网段

+ 以太网段由一组主机组成，这些主机通过导线（双绞线）连接到集线器(hub)
+ 跨越建筑物中的房间或地板
+ 操作
  + 每个以太网适配器有一个唯一的48-bit地址(MAC地址)
    + E.g., 00:16:ea:e3:54:e6
  + 主机在被称为帧(frames)的块中发送比特给其他任何主机
  + hub集线器被动地(slavishly)将每个位从每个端口复制到每个其他端
    + 每一个主机都能看到每一个bit
    + 注意：hub即将退出。网桥（交换机、路由器）变得足够便宜，足以取代它们

## 下一层：桥接以太网段

+ 跨建筑或校园
+ 网桥巧妙地了解哪些主机可以从哪个端口访问，然后有选择地将帧从一个端口复制到另一个端口

## 下一层：因特网(internets)

+ 多个不兼容的局域网可以通过称为路由器的专用计算机进行物理连接
+ 连接的网络称为internet（小写）

## 协议

+ 在互不兼容的LAN和WAN之间传数据
+ 协议软件运行在每个主机与路由器上
  + 协议是一组规则，用来控制主机和路由器在网络间传输数据时应该如何合作。
  + 消除不同网络之间的差异

## 协议做了什么？

+ 提供命名方案

  + 因特网协议定义了主机地址的统一格式
  + 每个主机（和路由器）至少分配一个唯一标识它的因特网地址

+ 提供发送机制

  + 因特网协议定义了一个标准传输单元（包packet）

  + 包由报头和有效载荷组成

    + 报头：包含包大小、源地址和目标地址等信息

    + 有效负载：包含从源主机发送的数据位

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E5%B0%81%E8%A3%85.png)

## 其他重要问题：查阅计算机网络

+ 如果不同的网络有不同的最大帧大小呢？（分段）
+ 路由器如何知道在哪里转发帧？
+ 当网络拓扑结构发生变化时，如何通知路由器？
+ 如果包丢失了怎么办？

## 全球IP互联网(Internet, 大写)

+ 基于TCP/IP协议族
  + IP(Internet Protocol)
    + 提供从*主机到主机*的基本命名方案和不可靠的数据包（数据报）传递能力
  + UDP(Unreliable Datagram Protocol)
    + 使用IP提供从*进程到进程*不可靠数据包传输
  + TCP(Transmission Control Protocol)
    + 使用IP通过链接提供从*进程到进程*可靠的字节流传输

通过混合使用Unix文件I/O和sockets接口的函数进行访问

## 互联网应用的软硬件组织

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E4%BA%92%E8%81%94%E7%BD%91%E5%BA%94%E7%94%A8%E7%9A%84%E8%BD%AF%E7%A1%AC%E4%BB%B6%E7%BB%84%E7%BB%87.png)

## 互联网：程序员的视角

1. 主机映射到32位IP地址
2. IP地址集映射到一组称为Internet域名的标识符
3. 一个Internet主机上的进程可以通过连接与另一个Internet主机上的进程通信

### IP地址

+ 32位IP地址存储在IP地址结构中
  + IP地址总是以网络字节顺序（big-endian字节顺序）存储在内存中

```c
/* Internet address structure */
struct in_addr {
    uint32_t  s_addr; /* network byte order (big-endian) */
};
```

点分十进制

+ 按照惯例，32位IP地址中的每个字节都由其十进制值表示，并用句点分隔
  + IP地址：0x8002C2F2=128.2.194.242

+ 使用`getaddrinfo`和`getnameinfo`函数（稍后介绍）在IP地址和点分隔十进制格式之间进行转换。

### 互联网域名

Domain Naming System(DNS)

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E4%BA%92%E8%81%94%E7%BD%91%E5%9F%9F%E5%90%8D.png)

+ 互联网在一个巨大的全球分布式数据库DNS中维护着IP地址和域名之间的映射
+ 从概念上讲，程序员可以将DNS数据库视为数百万个主机条目的集合。
  + 每个主机条目定义一组域名和IP地址之间的映射。
  + 在数学意义上，主机条目是域名和IP地址的等价类。

### DNS映射的属性

+ 可以使用`nslookup`探索DNS映射的属性

+ 每个主机都有一个本地定义的域名localhost，它总是映射到环回地址127.0.0.1
+ 使用`hostname`确定本地主机的真实域名：

### 互联网连接

+ 客户机和服务器通过`连接`发送字节流进行通信。每个连接是：
  + 点对点：连接一对进程。
  + 全双工(Full-duplex)：数据可以同时双向流动，
  + 可靠：源发送的字节流最终由目的地按发送顺序接收。

+ `Socket`是连接的端点
  + `socket`地址是`IP地址`：`端口`对
+ 端口是标识进程的16位整数：
  + 临时端口：当客户端发出连接请求时，由`客户端`内核自动分配。
  + 已知端口：与`服务器`提供的某些服务相关（例如，端口80与Web服务器关联）

### 已知端口和服务名

+ 流行服务已永久分配了已知端口和相应的已知服务名称：
  + echo服务：7/echo
  + ssh服务：22/ssh
  + 电子邮件服务：25/smtp
  + Web服务：80/http
+ 已知端口和服务名称之间的映射包含在每台Linux机器上的/etc/services文件中。

### 连接的解剖

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/connection%E7%94%B1socket%E5%AF%B9%E5%94%AF%E4%B8%80%E6%A0%87%E8%AF%86.png)

+ 连接由其端点（套接字对）的套接字地址进行唯一标识
  + (cliaddr:cliport, servaddr:servport)

### 使用端口标识服务

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E4%BD%BF%E7%94%A8%E7%AB%AF%E5%8F%A3%E6%A0%87%E8%AF%86%E6%9C%8D%E5%8A%A1.png)

### Socket接口

+ 与Unix I/O结合使用的一组系统级函数，用于构建网络应用程序。
+ 创建于80年代早期，是最初的Berkeley Unix发行版的一部分，包含了早期版本的互联网协议。
+ 可用于所有现代系统
  + Unix变体、Windows、OS X、IOS、Android、ARM

### socket

+ 什么是socket?
  + 对内核来说，socket是通信的端点(endpoint)
  + 对应用来说，socket是允许应用从网络读写的文件描述符
  + 请记住：所有Unix I/O设备（包括网络）都被建模为文件

+ 客户机和服务器通过读写socket描述符来相互通信
+ 常规文件I/O和套接字I/O之间的主要区别是应用程序如何“打开”套接字描述符

### Socket地址数据结构

+ 给`connect`,`bind`,`accept`的地址参数

+ 这是必需的，因为在设计sockets接口时C没有泛型（void*）指针

+ 为了方便，我们采用了Stevens的约定：

  ` typedef struct sockaddr SA;`

  ```c
  /* 泛型socket地址结构(给connect, bind, accept) */
  struct sockaddr { 
    uint16_t  sa_family;    /* Protocol family */ 
    char      sa_data[14];  /* Address data.  */ 
  };       
  ```

+ 特定于Internet的socket地址：

  对于采用套接字地址参数的函数，必须将（struct sockaddr_in*）强制转换为（struct sockaddr*）。（in后缀指Internet）

  ```c
  struct sockaddr_in  { 
    uint16_t        sin_family;  /* Protocol family (always AF_INET) */ 
    uint16_t        sin_port;    /* Port num in network byte order 大端序的16位端口号 */ 
    struct in_addr  sin_addr;    /* IP addr in network byte order 32位IP地址*/ 
    unsigned char   sin_zero[8]; /* Pad to sizeof(struct sockaddr) */ 
  }; 
  ```

  

### Sockets接口

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/sockets%E6%8E%A5%E5%8F%A3.png)

### 主机和服务转换：`getaddrinfo`

+ `getaddrinfo`是将主机名、主机地址、端口和服务名的字符串表示形式转换为套接字地址结构的现代方法。

  替换过时的`gethostbyname`和`getservbyname`函数。

+ 优势：

  可重入（线程程序可以安全地使用）

  允许我们编写可移植的协议无关的代码

  同时适用于IPv4和IPv6

+ 劣势：

  有点复杂

  幸运的是，在大多数情况下，少量的使用模式就足够了

```c
int getaddrinfo(const char *host,            /* Hostname or address */
                const char *service,         /* Port or service name */
                const struct addrinfo *hints,/* Input parameters */
                struct addrinfo **result);   /* Output linked list */

void freeaddrinfo(struct addrinfo *result);  /* Free linked list */

const char *gai_strerror(int errcode);       /* Return error msg */
```

+ 给定主机和服务，`getaddrinfo`返回指向`addrinfo`结构的链接列表的结果，每个结构指向相应的套接字地址结构，并且包含套接字接口函数的参数。

  helper函数：

  `freeadderinfo`释放整个链接列表。

  `gai_strerror`将错误代码转换为错误消息。

### `getaddrinfo`返回的链表

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/getaddrinfo%E8%BF%94%E5%9B%9E%E7%9A%84%E9%93%BE%E8%A1%A8.png)

+ 客户端：遍历这个列表，依次尝试每个套接字地址，直到调用`socket`和`connect`成功为止。

+ 服务器：遍历列表，直到调用`socket`和`bind`成功为止。

### `addrinfo`结构体

```c
struct addrinfo {
    int              ai_flags;     /* Hints argument flags */
    int              ai_family;    /* First arg to socket function */
    int              ai_socktype;  /* Second arg to socket function */
    int              ai_protocol;  /* Third arg to socket function  */
    char            *ai_canonname; /* Canonical host name */
    size_t           ai_addrlen;   /* Size of ai_addr struct */
    struct sockaddr *ai_addr;      /* Ptr to socket address structure */
    struct addrinfo *ai_next;      /* Ptr to next item in linked list */
};
```

+ `getaddrinfo`返回的每个`addrinfo`结构都包含可以直接传递给套接字函数的参数。
+ 还指向可以直接传递给connect和bind函数的套接字地址结构。

### `getnameinfo`

+ `getnameinfo`与`getaddrinfo`相反，它将套接字地址转换为相应的主机和服务。

  + 替换过时的`gethostbyaddr`和`getservbyport`函数。

  + 可重入和协议无关。

```c
int getnameinfo(const SA *sa, socklen_t salen, /* In: socket addr */
                char *host, size_t hostlen,    /* Out: host */
                char *serv, size_t servlen,    /* Out: service */
                int flags);                    /* optional flags */
```

## 基本互联网组件

+ 互联网骨架
  + 通过高速点对点网络连接的路由器集合（全国或全球）
+ 互联网交换节点(IXP)
  + 连接多个主干网（通常称为对等节点）的路由器
  + 也称为网络接入点（NAP）
+ 区域网络
  + 较小的主干线覆盖较小的地理区域（如城市或州）
+ 存在点（POP）
  + 连接到互联网的机器
+ 互联网提供商(Internet Service Providers ISPs)
  + 提供拨号或直接访问到POPs

## IP地址结构

+ 以w.x.y.z/n表格填写的网络ID
+ n=主机地址中的位数(子网掩码)
  + e,g.,CMU写为128.2.0.0/16
    + B类地址
+ 未路由（专用）IP地址：
  + 10.0.0.0/8 172.16.0.0/12 192.168.0.0/16

## 互联网的进化

+ 初始idea
  + 每个互联网上的节点都有唯一IP地址
    + 每个人可以和每个人对话
  + 没有保密或认证
    + 对同一LAN上的路由器和主机可见的消息
    + 可能在包头中伪造源字段
+ 缺点
  + IP地址不够
  + 不希望每个人都能访问或了解所有其他主机
  + 安全问题要求保密和认证

### 命名

+ 动态地址分配

  + 大多数主机不需要知道地址
    + 只有那些作为服务器的(需要)
  + DHCP(Dynamic Host Configuration Protocol)动态主机配置协议
    + 当地ISP分配地址给临时用户

+ e,g,.

  + CMU的笔记本电脑（有线连接）

    IP地址128.2.213.29（`bryant-tp4.cs.cmu.edu`)

    静态分配

  + 家用笔记本电脑

    IP地址192.168.1.5

    仅在家庭网络内有效

### 防火墙

+ 防火墙

  对Internet的其余部分隐藏组织节点

  使用组织内的本地IP地址

  对于外部服务，提供代理服务

  客户端请求：src=10.2.2.2，dest=216.99.99.99

  防火墙转发：src=176.3.3.3，dest=216.99.99.99

  服务器响应：src=216.99.99.99，dest=176.3.3.3

  防火墙转发响应：src=126.99.99.99，dest=10.2.2.2

