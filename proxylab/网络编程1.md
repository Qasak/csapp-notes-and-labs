## Socket接口`socket`

+ 客户端和服务器使用`socket`函数创建*socket描述符*

  ```c
  int socket(int domain, int type, int protocol)
  
  /* AF_INET: Indicates that we are using 32-bit IPV4 addresses*/
  /* SOCK_STREAM: 这个socket是连接的一个端点 */
  int clientfd = Socket(AF_INET, SOCK_STREAM, 0);
  
  ```

  **协议特定！最佳实践是使用getaddrinfo自动生成参数，以便代码独立于协议**

socket返回的`clientfd`仅是部分打开的，还不能用于读写。

如何完成打开socket的工作，取决于我们是客户端还是服务器



## Socket接口`bind`

+ 服务器使用bind请求内核将服务器的套接字地址与套接字描述符相关联：

  ```c
  int bind(int sockfd, SA *addr, socklen_t addrlen);
  ```

> ` typedef struct sockaddr SA;`

+ 进程可以通过从描述符`sockfd`读取到达端点为`addr`的连接的字节。
+ 类似地，对`sockfd`的写入将沿着端点为`addr`的连接进行传输。

最佳实践是使用`getaddrinfo`来提供参数`addr`和`addrlen`

## Socket接口`listen`

+ 默认情况下，kernel假设socket函数中的描述符是一个*活动套接字*，它将位于连接的客户端

+ 服务器调用listen函数告诉内核描述符将由服务器而不是客户端使用：

  ```c
  int listen(int sockfd, int backlog);
  ```

+ 将`sockfd`从*活动套接字*转换为可以接受来自客户端的连接请求的*侦听套接字*。

+ `backlog`暗示内核在开始拒绝连接请求之前，队列中要排队的未完成的连接请求的数量。

  + 需要深入理解TCP/IP协议
  + 通常设置为一个较大的值，例如1024

## Socket接口`accept`

+ 服务器通过调用`accept`来等待来自客户端的连接请求：

  ```c
  int accept(int listenfd, SA *addr, int *addrlen);
  ```

+ 等待连接请求到达绑定到`listenfd`的连接，然后在`addr`中填写客户端的套接字地址，在`addrlen`中填写套接字地址的大小。

+ 返回可用于通过Unix I/O例程与客户端通信的*已连接描述符(connected descriptor)*。

## Socket接口`connect`

+ 客户端通过调用connect与服务器建立连接：

  ```c
  int connect(int clientfd, SA *addr, socklen_t addrlen);
  ```

+ 尝试在套接字地址`addr`处与服务器建立连接

  + 如果成功，那么`clientfd`现在就可以读写了。

  + 结果连接的特征是套接字对

    `(x:y, addr.sin_addr:addr.sin_port)`

  x是客户地址

  y是唯一标识客户端主机上的客户机进程的临时端口

最佳实践是使用`getaddrinfo`来提供参数`addr`和`addrlen`。



## `accept`图解

![img](https://github.com/Qasak/all-about-computer-system/blob/master/proxylab/%E5%9B%BE%E8%A7%A3accept.png)



## 连接描述符 vs. 监听描述符

+ 监听描述符

  客户端连接请求的终点

  创建一次并在服务器的生存期内存在

+ 连接描述符

  客户端和服务器之间连接的终点

  每次服务器接受来自客户机的连接请求时，都会创建一个新的描述符

  只有在服务客户机时才存在

+ 为什么有区别？

  允许并发服务器同时通过多个客户端连接进行通信

  e.g., 每次我们收到一个新的请求，我们都会fork一个子进程来处理这个请求

## Sockets辅助函数：`open_clientfd`

+ 和服务器建立连接

  ```c
  int open_clientfd(char *hostname, char *port) {
      int clientfd;
      struct addrinfo hints, *listp, *p;
  
      /* Get a list of potential server addresses */
      memset(&hints, 0, sizeof(struct addrinfo));
      hints.ai_socktype = SOCK_STREAM;  /* Open a connection */
      hints.ai_flags = AI_NUMERICSERV;  /* ... using a numeric port arg. */
      hints.ai_flags |= AI_ADDRCONFIG;  /* Recommended for connections */
      Getaddrinfo(hostname, port, &hints, &listp);
    
      /* Walk the list for one that we can successfully connect to */
      for (p = listp; p; p = p->ai_next) {
          /* Create a socket descriptor */
          if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
              continue; /* Socket failed, try the next */
  
          /* Connect to the server */
          if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) 
              break; /* Success */
          Close(clientfd); /* Connect failed, try another */  //line:netp:openclientfd:closefd
      } 
  
      /* Clean up */
      Freeaddrinfo(listp);
      if (!p) /* All connects failed */
          return -1;
      else    /* The last connect succeeded */
          return clientfd;
  }
  ```

  

  ## Socket辅助函数：`open_listenfd`

  ```c
   int open_listenfd(char *port)
  {
      struct addrinfo hints, *listp, *p;
      int listenfd, optval=1;
  
      /* Get a list of potential server addresses */
      memset(&hints, 0, sizeof(struct addrinfo));
      hints.ai_socktype = SOCK_STREAM;             /* Accept connect. */
      hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* …on any IP addr */
      hints.ai_flags |= AI_NUMERICSERV;            /* …using port no. */
      Getaddrinfo(NULL, port, &hints, &listp);
      /* Walk the list for one that we can bind to */
      for (p = listp; p; p = p->ai_next) {
          /* Create a socket descriptor */
          if ((listenfd = socket(p->ai_family, p->ai_socktype, 
                                 p->ai_protocol)) < 0)
              continue;  /* Socket failed, try the next */
  
          /* Eliminates "Address already in use" error from bind */
          Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
                     (const void *)&optval , sizeof(int));
  
          /* Bind the descriptor to the address */
          if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
              break; /* Success */
          Close(listenfd); /* Bind failed, try the next */
      }
      /* Clean up */
      Freeaddrinfo(listp);
      if (!p) /* No address worked */
          return -1;
  
      /* Make it a listening socket ready to accept conn. requests */
      if (listen(listenfd, LISTENQ) < 0) {
          Close(listenfd);
          return -1;
      }
      return listenfd;
  }
  
  ```

  ## Echo客户端：主流程

  ```c
  #include "csapp.h"
  
  int main(int argc, char **argv) 
  {
      int clientfd;
      char *host, *port, buf[MAXLINE];
      rio_t rio;
  
      if (argc != 3) {
  	fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
  	exit(0);
      }
      host = argv[1];
      port = argv[2];
  
      clientfd = Open_clientfd(host, port);
      Rio_readinitb(&rio, clientfd);
  
      while (Fgets(buf, MAXLINE, stdin) != NULL) {
  	Rio_writen(clientfd, buf, strlen(buf));
  	Rio_readlineb(&rio, buf, MAXLINE);
  	Fputs(buf, stdout);
      }
      Close(clientfd); //line:netp:echoclient:close
      exit(0);
  }
  /* $end echoclientmain */
  
  ```

  ## 迭代Echo服务器：主流程

  ```c
  #include "csapp.h”
  void echo(int connfd);
  
  int main(int argc, char **argv)
  {
      int listenfd, connfd;
      socklen_t clientlen;
      struct sockaddr_storage clientaddr; /* Enough room for any addr */                                                                                                               
      char client_hostname[MAXLINE], client_port[MAXLINE];
  
      listenfd = Open_listenfd(argv[1]);
      while (1) {
  	clientlen = sizeof(struct sockaddr_storage); /* Important! */
  	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
  	Getnameinfo((SA *) &clientaddr, clientlen, 
                      client_hostname, MAXLINE, client_port, MAXLINE, 0);
  	printf("Connected to (%s, %s)\n", client_hostname, client_port);
  	echo(connfd);
  	Close(connfd);
      }
      exit(0);
  }
  ```

  ## Echo 服务器：`echo`函数

  + 服务器使用RIO读取和回显文本行，直到遇到EOF（文件结束）条件。

    客户端调用`close(clientfd)`造成EOF条件

```c
void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { 
        printf("server received %d bytes\n", (int)n);
	Rio_writen(connfd, buf, n);
    }
}
```

## 测试服务器：`telnet`

+ telnet程序对于测试通过Internet连接传输ASCII字符串的服务器是极其有用的

  + 我们的简单echo服务器

  + Web服务器

  + 邮件服务器

+ 使用：

  + linux> telnet <host> <portnumber>
  + 创建与在<host>上运行并侦听端口<portnumber>的服务器的连接

## 用`telnet`测试Echo服务器

+ telnet程序对于测试通过Internet连接传输ASCII字符串的服务器是非常有用的
  + 我们的简单echo服务器
  + Web服务器
  + 邮件服务器



+ 用法：
  + linux>telnet<主机><端口号>
  + 创建与在<host>上运行并侦听端口<portnumber>的服务器的连接

## Web服务器基础

+ 客户机和服务器使用超文本传输协议（HTTP）进行通信
  + 客户端和服务器建立TCP连接
  + 客户端请求内容
  + 服务器用请求的内容响应
  + 客户端和服务器关闭连接（最终）

+ 当前版本是HTTP/1.1
  + RFC 2616 1999年6月。

## Web内容

+ Web服务器将内容返回给客户端

  内容：具有相关MIME（多用途Internet邮件扩展 Multipurpose Internet Mail Extensions）类型的字节序列

+ Example MIME types
  text/html	HTML document
  text/plain	Unformatted text
  image/gif	Binary image encoded in GIF format
  image/png	Binar image encoded in PNG format
  image/jpeg	Binary image encoded in JPEG format

## 静态与动态内容

+ HTTP响应中返回的内容可以是静态的，也可以是动态的
  + 静态内容：存储在文件中并响应HTTP请求检索的内容
    + 示例：HTML文件、图像、音频剪辑
    + 请求标识哪个内容文件

+ 动态内容：为响应HTTP请求而动态生成的内容
  + 示例：由服务器代表客户机执行的程序生成的内容
  + 请求标识包含可执行代码的文件

**底线：Web内容与服务器管理的文件相关联**



## URLs

+ 文件的唯一名称：URL（Universal Resource Locator）

+ 示例URL:http://www.cmu.edu:80/index.html
+ 客户端使用前缀(http://www.cmu.edu:80）推断：
  + 要联系的服务器类型（协议）（HTTP）
  + 服务器在哪里(www.cmu.edu)
  + 它正在监听哪个端口（80）

+ 服务器使用后缀(/index.html)来：
  + 确定请求是静态内容还是动态内容。
    + 没有硬性规定
    + 一个约定：可执行文件位于`cgi-bin`目录中

+ 在文件系统上查找文件
  + 后缀中的首字母“/”表示请求内容的home目录
  + 最小后缀为“/”，服务器扩展为配置的默认文件名（通常，index.html)



## HTTP请求

+ HTTP请求是一个请求行(***request line***)，后跟零个或多个请求头(***request headers***)



+ 请求行：<method><uri><version>

  + <method>是GET、POST、OPTIONS、HEAD、PUT、DELETE或TRACE之一

  + <uri>通常是代理的URL，服务器的URL后缀

    + URL是URI（统一资源标识符）的一种类型

    + 详见http://www.ietf.org/rfc/rfc2396.txt

  + <version>是请求的HTTP版本（HTTP/1.0或HTTP/1.1）



+ 请求头：<header name>：<header data>
  + 向服务器提供附加信息

## Tiny Web Server

+ 文本描述的微型Web服务器

  + Tiny是一个顺序Web服务器

  + 为真实浏览器提供静态和动态内容
    + 文本文件、HTML文件、GIF、PNG和JPEG图像
  + 239行带注释的C代码
  + 不像真正的Web服务器那样完整或健壮
    + 您可以用格式错误的HTTP请求（例如，用“\n”而不是“\r\n”来终止行）

## Tiny操作

+ 接受来自客户端的连接
+ 从客户端读取请求（通过连接的套接字）

+ 拆分为<method><uri><version>
  + 如果method没有GET，则返回错误

+ 如果URI包含“cgi-bin”，则提供动态内容
  + （如果有文件"abcgi-bingo.html"会报错)
  + Fork子进程来执行程序

+ 否则提供静态内容
  + 将文件复制到输出

## 提供静态内容

```c
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);       
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    Rio_writen(fd, buf, strlen(buf));       
   
    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY, 0);    
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);                           
    Rio_writen(fd, srcp, filesize);         
    Munmap(srcp, filesize);                 
}
```

## 提供动态内容

+ 客户端向服务器发送请求

+ 如果请求URI包含字符串“/cgi-bin”，微型服务器假定请求是针对动态内容的

+ 服务器创建一个子进程并运行由该进程中的URI标识的程序

+ 子进程运行并生成动态内容

+ 服务器捕获子对象的内容并将其转发给客户端，而不进行任何修改

## 动态内容服务中的问题

+ 客户端如何向服务器传递程序参数？

+ 服务器如何将这些参数传递给子对象？

+ 服务器如何将与请求相关的其他信息传递给孩子？

+ 服务器如何捕获子进程生成的内容？

+ 公共网关接口（CGI, Common Gateway Interface）规范解决了这些问题。

## CGI

+ 因为这些子类是根据CGI规范编写的，所以它们通常被称为CGI程序。

+ 然而，CGI实际上定义了一个简单的标准，用于在客户机（浏览器）、服务器和子进程之间传输信息。

+ CGI是生成动态内容的原始标准。在很大程度上被其他更快的技术所取代：
  + E.g.,fastCGI，Apache模块，java servlet，Rails controllers
  + 避免在运行中创建进程（昂贵且缓慢）。

## 使用GET提供动态内容

Q：客户端如何向服务器传递参数？

A：参数被附加到URI中

+ `http://add.com/cgi-bin/adder?15213&18213`

+ 参数列表以“?”开头

+ 用“&”分隔参数

+ 用“+”或“%20”表示空格

Q：服务器如何将这些参数传递给子进程？

A：环境变量中的QUERY_STRING

+ 一个包含“？”后所有内容的字符串？

+ 对于add:QUERY_STRING=“15213&18213”

```c
    /* Extract the two arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
        p = strchr(buf, '&');
	 *p = '\0';
        strcpy(arg1, buf);
        strcpy(arg2, p+1);
        n1 = atoi(arg1);
        n2 = atoi(arg2);
    }
```

Q：服务器如何捕获子进程生成的内容？

A：子进程在stdout上生成其输出。服务器使用dup2将stdout重定向到其连接的套接字。

```c
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0) { /* Child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1); 
        Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */          
        Execve(filename, emptylist, environ); /* Run CGI program */ 
    }
    Wait(NULL); /* Parent waits for and reaps child */
}

```



+ 请注意，只有CGI子进程知道内容类型和长度，因此它必须生成这些头

  ```c
      /* Make the response body */
      sprintf(content, "Welcome to add.com: ");
      sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
      sprintf(content, "%sThe answer is: %d + %d = %d\r\n<p>",
              content, n1, n2, n1 + n2);
      sprintf(content, "%sThanks for visiting!\r\n", content);
  
      /* Generate the HTTP response */
      printf("Content-length: %d\r\n", (int)strlen(content));
      printf("Content-type: text/html\r\n\r\n");
      printf("%s", content);
      fflush(stdout);
  
      exit(0);
  
  ```

  

## 数据交换机制

+ 标准
  + 使用内容长度指定总长度
  + 需要程序缓冲整个消息

+ 分块
  + 分成块
  + 为每个块添加字节数前缀（十六进制编码）

## proxy

+ 代理是客户端和源服务器之间的中介
  + 对于客户端来说，代理就像一个服务器
  + 对于服务器来说，代理就像一个客户端

+ 可以在请求和响应经过时执行有用的功能
  + 示例：缓存，匿名，过滤、转码