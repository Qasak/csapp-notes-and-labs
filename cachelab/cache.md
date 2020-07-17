## 局部性

+ 局部性原理：程序倾向于使用地址接近或等于最近使用的数据和指令
+ 时间局部性：最近引用的项目可能在不久的将来再次被引用
+ 空间局部性：具有附近地址的项目往往在一次被一起引用

## 存储层次结构

L0:寄存器

L1: L1 cache (SRAM)

L2: L2 cache (SRAM)

L3: L3 cache (SRAM)

L4: 主存 (DRAM)

L5: 本地二级存储 (本地磁盘)

L6: 远程二级存储 (比如: 网络服务器)



## cache通用结构

`cache(S, E, B, m)`

考虑一个计算机系统，其中每个存储器地址有m位，形成$M=2^m$个不同的地址

这个机器的高速缓存被组织成一个有$S=2^s$个高速缓存组(cache set)的数组

每个组包含E个高速缓存行(cache line)

每个行由一个$B=2^b$字节的数据块(block)组成

有效位(valid bit)指明这个行是否包含有意义信息

标记位(tag bit)唯一地标识存储在这个高速缓存行中的块

![img](https://github.com/Qasak/all-about-computer-system/blob/master/cachelab/cache%E7%BB%84%E7%BB%87(S%2CE%2CB).png)

## cache运行过程

假设一个系统

它有一个CPU，一个寄存器文件，一个L1高速缓存，一个主存

当CPU执行一条读内存字w的指令，它向L1高速缓存请求这个字

这个字是否命中，然后抽取出被请求字的过程：

1. 组选择
2. 行匹配
3. 字抽取

![img](https://github.com/Qasak/all-about-computer-system/blob/master/cachelab/cache%E8%AF%BB.png)





## 直接映射

每个组只有一行(E=1)

![img](https://github.com/Qasak/all-about-computer-system/blob/master/cachelab/cache%E7%9B%B4%E6%8E%A5%E6%98%A0%E5%B0%840.png)

![img](https://github.com/Qasak/all-about-computer-system/blob/master/cachelab/cache%E7%9B%B4%E6%8E%A5%E6%98%A0%E5%B0%841.png)

+ 组选择：

  高速缓存从w的地址中抽取s个组索引位。

  这些位被解释成一个对应于组号的无符号整数(就像数组索引)

+ 行匹配：

  因为只有一行，所以只用看有效位

+ 字抽取：

  把块堪称一个字节数组，字节偏移是这个数组的一个索引

  这个例子中块偏移是$100_2$，表明w的副本是从块中的字节4开始的

+ 不命中时的行替换：

  用新取出的行替换当前行

