## 说明

此文件包含attacklab的一个实例的材料。

### ctarget

具有代码注入漏洞的Linux二进制文件。phase 1~3

### rtarget

具有面向返回的编程漏洞的Linux二进制文件。phase 4~5。

### cookie.txt



包含此实验室实例所需的4字节签名的文本文件。



### farm.c



此rtarget实例中存在gadget farm的源代码。你

可以编译（使用flag-Og）并反汇编它来查找gadget。

### hex2raw

生成字节序列的实用程序。见实验室文件
