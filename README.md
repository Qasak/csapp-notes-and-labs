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

### bitXor

两种解法：

1.与非门实现异或门

2.观察到用&和~可以实现映射f=lambda x,y:~x&y

f(a,b)即:在a和b的相同位上,将x为0且y为1的位置为1

那么x^y可描述为f(a,b)|f(b,a)

又 a|b可描述为g=lambda a,b:~(~a&~b)

综上,x^y=g(f(a,b),f(b,a))

```c
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
 
/*
    1)用与非门(NAND)实现异或门
 */
int bitXor(int x, int y) {
    int nand0=(~(x&y));
    int nand1=(~(x&nand0));
    int nand2=(~(y&nand0));
    int ans=(~(nand1&nand2));
  return ans;
}
/*
    2)观察到(~x)&y意义为:x某位是0,但y的同位是1的位组成的数
        eg: x=0110, y=1001
           ~x=1001,~y=0110
           (~x)&y:
                  1001
                  1001 &
                  ----
                  1001
           (~y)&x:
                  0110
                  0110 &
                  ----
                  0110
           x^y:
                  0110
                  1001 ^
                  ----
                  1111
                  
    同理    (~y)&x意义为:y某位是0,x的同位是1的位组成的数
    那么,有 x^y=((~x)&y)|((~y)&x);
    又有a|b=~(~a&~b)
        eg:
        a   b   a|b     ~a  ~b  ~a&~b   ~(~a&~b)
        1   0   1       0   1   0       1
        1   1   1       0   0   0       1
        0   0   0       1   1   1       0
        0   1   1       1   0   0       1
    综上
    x^y=~((~((~x)&y))&(~((~y)&x)))
 */
int bitXor(int x,int y)
{
    return ~((~((~x)&y))&(~((~y)&x)));
}
```


## bomb lab:
### hint:
`gdb`


有一个非常方便的单页[gdb摘要](http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Debug.html)

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
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi       
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq  
```

注意到我们要比较的字符串是放在0x402400这个内存地址里面的

这时使用[gdb打印运行时指定内存地址处的内容](https://sourceware.org/gdb/current/onlinedocs/gdb/Memory.html)就可以了

>x/nfu addr  
x for “examine”  
n, the repeat count:重复计数是十进制整数；默认值为1。它指定要显示多少内存（以u为单位）。如果指定了负数，则从addr向后检查内存。  
f,  显示格式  
显示格式是print（“x”、“d”、“u”、“o”、“t”、“a”、“c”、“f”、“s”）以及“i”（用于机器指令）所使用的格式之一。默认值最初为“x”（十六进制）。每次使用x或print时，默认值都会更改。  
u,单位大小,b:字节  

eg:

x/128db 0x402400

### phase2

```x86asm
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp
  400efd:	53                   	push   %rbx
  400efe:	48 83 ec 28          	sub    $0x28,%rsp
  400f02:	48 89 e6             	mov    %rsp,%rsi
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp)
  400f0e:	74 20                	je     400f30 <phase_2+0x34>
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb>
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax
  400f1a:	01 c0                	add    %eax,%eax
  400f1c:	39 03                	cmp    %eax,(%rbx)
  400f1e:	74 05                	je     400f25 <phase_2+0x29>
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb>
  400f25:	48 83 c3 04          	add    $0x4,%rbx
  400f29:	48 39 eb             	cmp    %rbp,%rbx
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b>
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40>
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b>
  400f3c:	48 83 c4 28          	add    $0x28,%rsp
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq   
```

> All solutions written by github user Qasak.
as O'Hallaron said, whatever you do, don't cheat.
https://qasak.github.io/
