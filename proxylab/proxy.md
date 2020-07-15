## Telnet/cURL Demo

Telnet

+ 交互式远程shell–类似没有安全性的ssh
  + 必须手动生成HTTP请求
  + 如果您想测试对格式错误的头的响应，这可能很有用

cURL

+ 带有命令行程序的“URL传输库”

+ 为您生成有效的HTTP请求

  `curl http://www.baidu.com`

+ 也可用于生成HTTP代理请求

  `curl --proxy lemonshark.ics.cs.cmu.edu:3092 http://www.cmu.edu/`

## 传输HTTP数据

如果有东西从web服务器请求文件，

它怎么知道转移完成了？

A） 它读取空字节。

B） 连接关闭。

C） 它显示一个空白行。

D） HTTP头指定要接收的字节数。

E） 读取功能接收EOF。

## PxyDrive

+ 介绍基本流程：

  生成数据，创建服务器，从服务器获取/请求文件，