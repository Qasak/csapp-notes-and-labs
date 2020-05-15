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
|Carry Flag(for unsigned)<br>进位标志|Zero Flag <br>刚刚计算的值是0|Sign Flag(for signed) <br>最高有效位是1|Overflow Flag(for signed)<br>有符号数运算溢出|



SetX Instructions

SetX指令

根据条件码设置低位字节为0或1

|SetX|Condition|Description|
|  ----  | ----  |  ----  |
|sete|ZF|Equal/Zero|
|setne|~ZF|Not Equal/Not Zero|
|sets|SF|Negative|
|setns|~SF|Nonnegative|
|setg|~(SF^OF)&~ZF|Greater(Signed)|
|setge|~(SF^OF)|Greater or Equal(Signed)|
|setl|(SF^OF)|Less(signed)|
|setle|(SF^OF)&#124;ZF|Less or Equal(Signed)|
|seta|~CF&~ZF|Above(unsigned)|
|setb|CF|Below(unsigned)|



jX Instructions

jX跳转指令

根据条件码跳转到代码的不同部分

|jX|Condition|Description|
|  ----  | ----  | ----  |
|jmp|1|Unconditional|
|je|ZF|Equal/Zero|
|jne|~ZF|Not Equal/Zero|
|js|SF|Negative|
|jns|~SF|Nonnegative|
|jg|~(SF^OF)&~ZF|Greater(Signed)|
|jge|~(SF^OF)|Greater or Equal(Signed)|
|jl|(SF^OF)|Less(Signed)|
|jle|(SF^OF) &#124;ZF|Less or Equal(Signed)|
|ja|~CF&~ZF|Above(Unsigned)|
|jb|CF|Below(Unsigned)|


### phase1

```x86asm
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi       #rsi(第二个参数)
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq  
```
gdb查看指定地址处的内容：

0x402400:       0x42    0x6f    0x72    0x64    0x65    0x72    0x20    0x72
0x402408:       0x65    0x6c    0x61    0x74    0x69    0x6f    0x6e    0x73
0x402410:       0x20    0x77    0x69    0x74    0x68    0x20    0x43    0x61
0x402418:       0x6e    0x61    0x64    0x61    0x20    0x68    0x61    0x76
0x402420:       0x65    0x20    0x6e    0x65    0x76    0x65    0x72    0x20
0x402428:       0x62    0x65    0x65    0x6e    0x20    0x62    0x65    0x74
0x402430:       0x74    0x65    0x72    0x2e    0x00    0x00    0x00    0x00
0x402438:       0x57    0x6f    0x77    0x21    0x20    0x59    0x6f    0x75
0x402440:       0x27    0x76    0x65    0x20    0x64    0x65    0x66    0x75
0x402448:       0x73    0x65    0x64    0x20    0x74    0x68    0x65    0x20
0x402450:       0x73    0x65    0x63    0x72    0x65    0x74    0x20    0x73
0x402458:       0x74    0x61    0x67    0x65    0x21    0x00    0x66    0x6c
0x402460:       0x79    0x65    0x72

这个就是我们要找的答案了

### phase2

> All solutions written by github user Qasak.

> as O'Hallaron said, whatever you do, don't cheat.

> https://qasak.github.io/
