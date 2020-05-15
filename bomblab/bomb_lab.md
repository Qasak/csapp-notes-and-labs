This is an x86-64 bomb for self-study students. 

## bomb lab:

### bomb

bomb是一个二进制文件,不能直接查看

其对应的主流程[.c文件](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/bomb.c)

`objdump -d bomb`反汇编得到解开谜题[所需的汇编代码信息](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/bomb.d).

### main

```asm
0000000000400da0 <main>:
  400da0:	53                   	push   %rbx
  400da1:	83 ff 01             	cmp    $0x1,%edi
  400da4:	75 10                	jne    400db6 <main+0x16>
  400da6:	48 8b 05 9b 29 20 00 	mov    0x20299b(%rip),%rax        # 603748 <stdin@@GLIBC_2.2.5>
  400dad:	48 89 05 b4 29 20 00 	mov    %rax,0x2029b4(%rip)        # 603768 <infile>
  400db4:	eb 63                	jmp    400e19 <main+0x79>
  400db6:	48 89 f3             	mov    %rsi,%rbx
  400db9:	83 ff 02             	cmp    $0x2,%edi
  400dbc:	75 3a                	jne    400df8 <main+0x58>
  400dbe:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
  400dc2:	be b4 22 40 00       	mov    $0x4022b4,%esi
  400dc7:	e8 44 fe ff ff       	callq  400c10 <fopen@plt>
  400dcc:	48 89 05 95 29 20 00 	mov    %rax,0x202995(%rip)        # 603768 <infile>
  400dd3:	48 85 c0             	test   %rax,%rax
  400dd6:	75 41                	jne    400e19 <main+0x79>
  400dd8:	48 8b 4b 08          	mov    0x8(%rbx),%rcx
  400ddc:	48 8b 13             	mov    (%rbx),%rdx
  400ddf:	be b6 22 40 00       	mov    $0x4022b6,%esi
  400de4:	bf 01 00 00 00       	mov    $0x1,%edi
  400de9:	e8 12 fe ff ff       	callq  400c00 <__printf_chk@plt>
  400dee:	bf 08 00 00 00       	mov    $0x8,%edi
  400df3:	e8 28 fe ff ff       	callq  400c20 <exit@plt>
  400df8:	48 8b 16             	mov    (%rsi),%rdx
  400dfb:	be d3 22 40 00       	mov    $0x4022d3,%esi
  400e00:	bf 01 00 00 00       	mov    $0x1,%edi
  400e05:	b8 00 00 00 00       	mov    $0x0,%eax
  400e0a:	e8 f1 fd ff ff       	callq  400c00 <__printf_chk@plt>
  400e0f:	bf 08 00 00 00       	mov    $0x8,%edi
  400e14:	e8 07 fe ff ff       	callq  400c20 <exit@plt>
  400e19:	e8 84 05 00 00       	callq  4013a2 <initialize_bomb>
  400e1e:	bf 38 23 40 00       	mov    $0x402338,%edi
  400e23:	e8 e8 fc ff ff       	callq  400b10 <puts@plt>
  400e28:	bf 78 23 40 00       	mov    $0x402378,%edi
  400e2d:	e8 de fc ff ff       	callq  400b10 <puts@plt>
  400e32:	e8 67 06 00 00       	callq  40149e <read_line>
  400e37:	48 89 c7             	mov    %rax,%rdi
  400e3a:	e8 a1 00 00 00       	callq  400ee0 <phase_1>
  400e3f:	e8 80 07 00 00       	callq  4015c4 <phase_defused>
  400e44:	bf a8 23 40 00       	mov    $0x4023a8,%edi
  400e49:	e8 c2 fc ff ff       	callq  400b10 <puts@plt>
  400e4e:	e8 4b 06 00 00       	callq  40149e <read_line>
  400e53:	48 89 c7             	mov    %rax,%rdi
  400e56:	e8 a1 00 00 00       	callq  400efc <phase_2>
  400e5b:	e8 64 07 00 00       	callq  4015c4 <phase_defused>
  400e60:	bf ed 22 40 00       	mov    $0x4022ed,%edi
  400e65:	e8 a6 fc ff ff       	callq  400b10 <puts@plt>
  400e6a:	e8 2f 06 00 00       	callq  40149e <read_line>
  400e6f:	48 89 c7             	mov    %rax,%rdi
  400e72:	e8 cc 00 00 00       	callq  400f43 <phase_3>
  400e77:	e8 48 07 00 00       	callq  4015c4 <phase_defused>
  400e7c:	bf 0b 23 40 00       	mov    $0x40230b,%edi
  400e81:	e8 8a fc ff ff       	callq  400b10 <puts@plt>
  400e86:	e8 13 06 00 00       	callq  40149e <read_line>
  400e8b:	48 89 c7             	mov    %rax,%rdi
  400e8e:	e8 79 01 00 00       	callq  40100c <phase_4>
  400e93:	e8 2c 07 00 00       	callq  4015c4 <phase_defused>
  400e98:	bf d8 23 40 00       	mov    $0x4023d8,%edi
  400e9d:	e8 6e fc ff ff       	callq  400b10 <puts@plt>
  400ea2:	e8 f7 05 00 00       	callq  40149e <read_line>
  400ea7:	48 89 c7             	mov    %rax,%rdi
  400eaa:	e8 b3 01 00 00       	callq  401062 <phase_5>
  400eaf:	e8 10 07 00 00       	callq  4015c4 <phase_defused>
  400eb4:	bf 1a 23 40 00       	mov    $0x40231a,%edi
  400eb9:	e8 52 fc ff ff       	callq  400b10 <puts@plt>
  400ebe:	e8 db 05 00 00       	callq  40149e <read_line>
  400ec3:	48 89 c7             	mov    %rax,%rdi
  400ec6:	e8 29 02 00 00       	callq  4010f4 <phase_6>
  400ecb:	e8 f4 06 00 00       	callq  4015c4 <phase_defused>
  400ed0:	b8 00 00 00 00       	mov    $0x0,%eax
  400ed5:	5b                   	pop    %rbx
  400ed6:	c3                   	retq   
  400ed7:	90                   	nop
  400ed8:	90                   	nop
  400ed9:	90                   	nop
  400eda:	90                   	nop
  400edb:	90                   	nop
  400edc:	90                   	nop
  400edd:	90                   	nop
  400ede:	90                   	nop
  400edf:	90                   	nop

```



### phase_1

```asm

  400e19:	e8 84 05 00 00       	callq  4013a2 <initialize_bomb> #这里修改的寄存器值会在后面覆盖,所以这个初始化并没有什么用
  400e32:	e8 67 06 00 00       	callq  40149e <read_line>
  400e37:	48 89 c7             	mov    %rax,%rdi            #input在内存中的地址在rdi(第一个参数)
  400e3a:	e8 a1 00 00 00       	callq  400ee0 <phase_1>
  
00000000004013a2 <initialize_bomb>:
  4013a2:	48 83 ec 08          	sub    $0x8,%rsp
  4013a6:	be a0 12 40 00       	mov    $0x4012a0,%esi
  4013ab:	bf 02 00 00 00       	mov    $0x2,%edi
  4013b0:	e8 db f7 ff ff       	callq  400b90 <signal@plt>
  4013b5:	48 83 c4 08          	add    $0x8,%rsp
  4013b9:	c3                   	retq   
  
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi       #rsi(第二个参数)
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal> #比较rsi和rdi中的地址指向的字符串是否相等
  400eee:	85 c0                	test   %eax,%eax	#if eax&eax==0,即返回值为0,两字符串相等.跳过引爆炸弹
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq  
  
0000000000401338 <strings_not_equal>:
  401338:	41 54                	push   %r12
  40133a:	55                   	push   %rbp
  40133b:	53                   	push   %rbx
  40133c:	48 89 fb             	mov    %rdi,%rbx    #&input
  40133f:	48 89 f5             	mov    %rsi,%rbp    #0x402400
  401342:	e8 d4 ff ff ff       	callq  40131b <string_length>
  401347:	41 89 c4             	mov    %eax,%r12d   #r12d=length(input)
  40134a:	48 89 ef             	mov    %rbp,%rdi    #rdi=0x402400
  40134d:	e8 c9 ff ff ff       	callq  40131b <string_length>
  401352:	ba 01 00 00 00       	mov    $0x1,%edx    #edx=0x1
  401357:	41 39 c4             	cmp    %eax,%r12d   #eax=length((0x402400))  if(length(input)!=length(rbp))
  40135a:	75 3f                	jne    40139b <strings_not_equal+0x63>
  40135c:	0f b6 03             	movzbl (%rbx),%eax  #rax=input[0]
  40135f:	84 c0                	test   %al,%al      #al&al==0,输入字符串以0结尾
  401361:	74 25                	je     401388 <strings_not_equal+0x50>
  401363:	3a 45 00             	cmp    0x0(%rbp),%al        #*(rbp[0])==al 输入值的低位字节
  401366:	74 0a                	je     401372 <strings_not_equal+0x3a>
  401368:	eb 25                	jmp    40138f <strings_not_equal+0x57>
  40136a:	3a 45 00             	cmp    0x0(%rbp),%al
  40136d:	0f 1f 00             	nopl   (%rax)
  401370:	75 24                	jne    401396 <strings_not_equal+0x5e>
  401372:	48 83 c3 01          	add    $0x1,%rbx            #rbx++ (&input)
  401376:	48 83 c5 01          	add    $0x1,%rbp            #rbp++ (0x402400)
  40137a:	0f b6 03             	movzbl (%rbx),%eax          
  40137d:	84 c0                	test   %al,%al              #低位!='\0'
  40137f:	75 e9                	jne    40136a <strings_not_equal+0x32>
  401381:	ba 00 00 00 00       	mov    $0x0,%edx
  401386:	eb 13                	jmp    40139b <strings_not_equal+0x63>
  401388:	ba 00 00 00 00       	mov    $0x0,%edx    #edx=0
  40138d:	eb 0c                	jmp    40139b <strings_not_equal+0x63>
  40138f:	ba 01 00 00 00       	mov    $0x1,%edx
  401394:	eb 05                	jmp    40139b <strings_not_equal+0x63>
  401396:	ba 01 00 00 00       	mov    $0x1,%edx
  40139b:	89 d0                	mov    %edx,%eax    #eax=edx =1
  40139d:	5b                   	pop    %rbx
  40139e:	5d                   	pop    %rbp
  40139f:	41 5c                	pop    %r12
  4013a1:	c3                   	retq   
  
000000000040131b <string_length>:
  40131b:	80 3f 00             	cmpb   $0x0,(%rdi)      #起始地址
  40131e:	74 12                	je     401332 <string_length+0x17>
  401320:	48 89 fa             	mov    %rdi,%rdx        #rdx=rdi
  401323:	48 83 c2 01          	add    $0x1,%rdx        #rdx++
  401327:	89 d0                	mov    %edx,%eax        #eax=edx
  401329:	29 f8                	sub    %edi,%eax        #eax=eax-edi #地址差值即为长度
  40132b:	80 3a 00             	cmpb   $0x0,(%rdx)      #是否==字符串结束符'\0'
  40132e:	75 f3                	jne    401323 <string_length+0x8>
  401330:	f3 c3                	repz retq 
  401332:	b8 00 00 00 00       	mov    $0x0,%eax
  401337:	c3                   	retq   
 
```

注意到`phase_1()`函数中的

```assembly
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
```

通过阅读`strings_not_equal()`函数的汇编代码

```asm
0000000000401338 <strings_not_equal>:
  401338:	41 54                	push   %r12
  40133a:	55                   	push   %rbp
  40133b:	53                   	push   %rbx
  40133c:	48 89 fb             	mov    %rdi,%rbx    #&input
  40133f:	48 89 f5             	mov    %rsi,%rbp    #0x402400
```

可知，其两个参数的地址分别放在`rdi`和`rsi`寄存器中，

而通过

```asm
  400e32:	e8 67 06 00 00       	callq  40149e <read_line>
  400e37:	48 89 c7             	mov    %rax,%rdi            #input在内存中的地址在rdi(第一个参数)
```

可知`read_line()`的返回值被放在了`rdi`中,由此可知,`esi`中存放的就是将要和我们的输入比较的字符串在内存中的地址`0x402400`。

那么怎么拿到其中的内容呢？

这时使用[gdb打印运行时指定内存地址处的内容](https://sourceware.org/gdb/current/onlinedocs/gdb/Memory.html)就可以了

>x/nfu addr  
>x for “examine”  
>n, the repeat count:重复计数是十进制整数；默认值为1。它指定要显示多少内存（以u为单位）。如果指定了负数，则从addr向后检查内存。  
>f,  显示格式  
>显示格式是print（“x”、“d”、“u”、“o”、“t”、“a”、“c”、“f”、“s”）以及“i”（用于机器指令）所使用的格式之一。默认值最初为“x”（十六进制）。每次使用x或print时，默认值都会更改。  
>u,单位大小,b:字节  

`x/128db 0x402400`

```asm
0x402400:       0x42    0x6f    0x72    0x64    0x65    0x72    0x20    0x72

0x402408:       0x65    0x6c    0x61    0x74    0x69    0x6f    0x6e    0x73

0x402410:       0x20    0x77    0x69    0x74    0x68    0x20    0x43    0x61

0x402418:       0x6e    0x61    0x64    0x61    0x20    0x68    0x61    0x76

0x402420:       0x65    0x20    0x6e    0x65    0x76    0x65    0x72    0x20

0x402428:       0x62    0x65    0x65    0x6e    0x20    0x62    0x65    0x74

0x402430:       0x74    0x65    0x72    0x2e
```

->

`Border relations with Canada have never been better.`

### phase_2

```asm
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

顺着主流程的代码看下来，我们发现:同样的，我们的输入的地址被放在了`rdi`寄存器。



### hint:

`gdb`


有一个非常方便的单页[gdb摘要](http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Debug.html)

`汇编基础`

Condition Codes

条件码

They are all one bit flags and they get set not directly, bute as side effect of other operations that take place.

| CF                                   | ZF                            | SF                                      | OF                                            |
| ------------------------------------ | ----------------------------- | --------------------------------------- | --------------------------------------------- |
| Carry Flag(for unsigned)<br>进位标志 | Zero Flag <br>刚刚计算的值是0 | Sign Flag(for signed) <br>最高有效位是1 | Overflow Flag(for signed)<br>有符号数运算溢出 |



SetX Instructions

SetX指令

根据条件码设置低位字节为0或1

| SetX  | Condition       | Description              |
| ----- | --------------- | ------------------------ |
| sete  | ZF              | Equal/Zero               |
| setne | ~ZF             | Not Equal/Not Zero       |
| sets  | SF              | Negative                 |
| setns | ~SF             | Nonnegative              |
| setg  | ~(SF^OF)&~ZF    | Greater(Signed)          |
| setge | ~(SF^OF)        | Greater or Equal(Signed) |
| setl  | (SF^OF)         | Less(signed)             |
| setle | (SF^OF)&#124;ZF | Less or Equal(Signed)    |
| seta  | ~CF&~ZF         | Above(unsigned)          |
| setb  | CF              | Below(unsigned)          |



jX Instructions

jX跳转指令

根据条件码跳转到代码的不同部分

| jX   | Condition        | Description              |
| ---- | ---------------- | ------------------------ |
| jmp  | 1                | Unconditional            |
| je   | ZF               | Equal/Zero               |
| jne  | ~ZF              | Not Equal/Zero           |
| js   | SF               | Negative                 |
| jns  | ~SF              | Nonnegative              |
| jg   | ~(SF^OF)&~ZF     | Greater(Signed)          |
| jge  | ~(SF^OF)         | Greater or Equal(Signed) |
| jl   | (SF^OF)          | Less(Signed)             |
| jle  | (SF^OF) &#124;ZF | Less or Equal(Signed)    |
| ja   | ~CF&~ZF          | Above(Unsigned)          |
| jb   | CF               | Below(Unsigned)          |
