# all-about-csapp-labs
Solutions of Computer Systems: A Programmer's Perspective's labs

CSAPP lab全部题解

## data lab:
| Score   | Rating  | Errors | Function|
|  ----  | ----  |  ----  | ----  |
| 1  | 1 | 0  | bitXor |
| 1  | 1 | 0  | tmin |
| 1  | 1 | 0  | isTmax |
| 2  | 2 | 0  | allOddBits |
| 2  | 2 | 0  | negate |
| 3  | 3 | 0  | isAsciiDigit |
| 3  | 3 | 0  | conditional |
| 3  | 3 | 0  | isLessOrEqual |
| 4  | 4 | 0  | logicalNeg |
| 4  | 4 | 0  | howManyBits | 
| 4  | 4 | 0  | floatScale2 |
| 4  | 4 | 0  | floatFloat2Int |
| 4  | 4 | 0  | floatPower2 |

## bomb lab:
### hint:
`gdb`

http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Debug.html
有一个非常方便的单页gdb摘要，您可以打印出来作为参考。下面是一些使用gdb的其他技巧。

`汇编基础`

Condition Codes

条件码

They are all one bit flags and they get set not directly, bute as side effect of other operations that take place.

|CF|ZF|SF|OF|
|  ----  | ----  |  ----  | ----  |
|Carry Flag(for unsigned)进位标志|Zero Flag 刚刚计算的值是0|Sign Flag(for signed) 最高有效位是1|Overflow Flag(for signed)有符号数运算溢出|



SetX Instructions

SetX指令

根据条件码设置低位字节为0或1

|sete|||



jX Instructions

jX跳转指令

根据条件码跳转到代码的不同部分

|jX|Condition|Description|
|  ----  | ----  |  ----  | ----  |
|jmp|1|Unconditional|
|je|ZF||
||||
||||
||||
||||
||||
||||
||||
||||
||||
||||
||||
||||

> All solutions written by github user Qasak.

> as O'Hallaron said, whatever you do, don't cheat.

> https://qasak.github.io/
