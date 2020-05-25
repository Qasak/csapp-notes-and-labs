This is an x86-64 bomb for self-study students. 

## bomb lab:

> 有人说：“我理解了一般规则，不愿意劳神去学习细节！”他们实际上是在自欺欺人。

### bomb

bomb是一个二进制文件,不能直接查看

其对应的主流程[.c文件](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/bomb.c)

`objdump -d bomb`反汇编得到解开谜题[所需的汇编代码信息](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/bomb.d).



### phase_1

先来看一下phase_1调用的上下文

```asm
  400e19:	e8 84 05 00 00       	callq  4013a2 <initialize_bomb>#这里修改的寄存器值会在后面覆盖,所以这个初始化并没有什么用
  400e1e:	bf 38 23 40 00       	mov    $0x402338,%edi
  400e23:	e8 e8 fc ff ff       	callq  400b10 <puts@plt>
  400e28:	bf 78 23 40 00       	mov    $0x402378,%edi
  400e2d:	e8 de fc ff ff       	callq  400b10 <puts@plt>
  400e32:	e8 67 06 00 00       	callq  40149e <read_line>
  400e37:	48 89 c7             	mov    %rax,%rdi #input在内存中的地址在rdi(第一个参数)
  400e3a:	e8 a1 00 00 00       	callq  400ee0 <phase_1>
  400e3f:	e8 80 07 00 00       	callq  4015c4 <phase_defused>
```



再来看一下其中涉及的关键函数的汇编代码

```asm
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

`x/128xb 0x402400`

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

先来看一下`phase_2`调用的上下文

```asm
  400e4e:	e8 4b 06 00 00       	callq  40149e <read_line>
  400e53:	48 89 c7             	mov    %rax,%rdi
  400e56:	e8 a1 00 00 00       	callq  400efc <phase_2>
  400e5b:	e8 64 07 00 00       	callq  4015c4 <phase_defused>
```

同样的，我们的输入的地址被放在了`rdi`寄存器。

再看一下`phase_2`本身

```asm
0000000000400efc <phase_2>:
  400efc:	55                   	push   %rbp # M[R[rsp]]=R[rbp]
  400efd:	53                   	push   %rbx # M[R[rsp]]=R[rbx]
  400efe:	48 83 ec 28          	sub    $0x28,%rsp # R[rsp]<-R[rsp]-0x28.栈开了0x28=40Bytes,40/8=5个字的空间
  400f02:	48 89 e6             	mov    %rsp,%rsi # R[rsi]=R[rsp],即R[rsi]存了当前栈顶地址
  400f05:	e8 52 05 00 00       	callq  40145c <read_six_numbers>
  400f0a:	83 3c 24 01          	cmpl   $0x1,(%rsp) 
  400f0e:	74 20                	je     400f30 <phase_2+0x34> # if *rsp==1 goto 400f30
  400f10:	e8 25 05 00 00       	callq  40143a <explode_bomb> # else BOOM!
  400f15:	eb 19                	jmp    400f30 <phase_2+0x34>
  400f17:	8b 43 fc             	mov    -0x4(%rbx),%eax # from 400f3a. eax=*(rbx-4). from 400f2c
  400f1a:	01 c0                	add    %eax,%eax # eax+=eax
  400f1c:	39 03                	cmp    %eax,(%rbx) 
  400f1e:	74 05                	je     400f25 <phase_2+0x29> # if eax==*(rbx) goto 400f25
  400f20:	e8 15 05 00 00       	callq  40143a <explode_bomb> # else BOOM
  400f25:	48 83 c3 04          	add    $0x4,%rbx # from 400f1e. rbx=rbx+4
  400f29:	48 39 eb             	cmp    %rbp,%rbx 
  400f2c:	75 e9                	jne    400f17 <phase_2+0x1b> # if rbp!=rbx goto 400f17
  400f2e:	eb 0c                	jmp    400f3c <phase_2+0x40> # else goto 400f3c
  400f30:	48 8d 5c 24 04       	lea    0x4(%rsp),%rbx # from 400f0e. rbx=&rsp+4
  400f35:	48 8d 6c 24 18       	lea    0x18(%rsp),%rbp # rbp=&rsp+20
  400f3a:	eb db                	jmp    400f17 <phase_2+0x1b> # goto 400f17
  400f3c:	48 83 c4 28          	add    $0x28,%rsp # rsp=0x28
  400f40:	5b                   	pop    %rbx
  400f41:	5d                   	pop    %rbp
  400f42:	c3                   	retq   
```

`read_six_numbers`

```asm
000000000040145c <read_six_numbers>:
  40145c:	48 83 ec 18          	sub    $0x18,%rsp #栈开了24Byte/8=3字的空间
  401460:	48 89 f2             	mov    %rsi,%rdx #R[rdx]=R[rsi]
  401463:	48 8d 4e 04          	lea    0x4(%rsi),%rcx # rcx=&rsi+4
  401467:	48 8d 46 14          	lea    0x14(%rsi),%rax # rax=&rsi+20
  40146b:	48 89 44 24 08       	mov    %rax,0x8(%rsp) # *(rsp+8)=rax ??
  401470:	48 8d 46 10          	lea    0x10(%rsi),%rax # rax=&rsi+16
  401474:	48 89 04 24          	mov    %rax,(%rsp) # *(rsp)=rax ??
  401478:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9 # r9=&rsi+12
  40147c:	4c 8d 46 08          	lea    0x8(%rsi),%r8 # r8=&rsi+8
  401480:	be c3 25 40 00       	mov    $0x4025c3,%esi # esi=0x4023c3
  401485:	b8 00 00 00 00       	mov    $0x0,%eax # eax=0x0
  40148a:	e8 61 f7 ff ff       	callq  400bf0 <__isoc99_sscanf@plt> #读字符串
  40148f:	83 f8 05             	cmp    $0x5,%eax # eax-5,sscanf的返回值是有效字符数
  401492:	7f 05                	jg     401499 <read_six_numbers+0x3d> # eax>5 goto 401499
  401494:	e8 a1 ff ff ff       	callq  40143a <explode_bomb> # BOOM!
  401499:	48 83 c4 18          	add    $0x18,%rsp # rsp+=24
  40149d:	c3                   	retq   

```

注意到%esi=0x4025c3.

这里打出`0x4025c3`对应内存地址里的内容,不出所料是“%d %d %d %d %d %d”

即为下方sscanf的格式,和函数名一样,是从%rdi指向的字符串读入六个整数.

下面的图片可以很好的解释phase_2干了什么

![img](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/phase_2_stack.jpg)

`1 2 4 8 16 32`



### phase_3

```asm
0000000000400f43 <phase_3>:
  400f43:	48 83 ec 18          	sub    $0x18,%rsp
  400f47:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  400f4c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
  400f51:	be cf 25 40 00       	mov    $0x4025cf,%esi
  400f56:	b8 00 00 00 00       	mov    $0x0,%eax
  400f5b:	e8 90 fc ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  400f60:	83 f8 01             	cmp    $0x1,%eax
  400f63:	7f 05                	jg     400f6a <phase_3+0x27>
  400f65:	e8 d0 04 00 00       	callq  40143a <explode_bomb>
  400f6a:	83 7c 24 08 07       	cmpl   $0x7,0x8(%rsp)
  400f6f:	77 3c                	ja     400fad <phase_3+0x6a> # Above (unsigned)  0,1,2,3,4,5,6,7
  400f71:	8b 44 24 08          	mov    0x8(%rsp),%eax
  400f75:	ff 24 c5 70 24 40 00 	jmpq   *0x402470(,%rax,8) # 跳转到绝对地址0x402470+%rax*8.跳转表,实现switch语句
  400f7c:	b8 cf 00 00 00       	mov    $0xcf,%eax
  400f81:	eb 3b                	jmp    400fbe <phase_3+0x7b>
  400f83:	b8 c3 02 00 00       	mov    $0x2c3,%eax
  400f88:	eb 34                	jmp    400fbe <phase_3+0x7b>
  400f8a:	b8 00 01 00 00       	mov    $0x100,%eax
  400f8f:	eb 2d                	jmp    400fbe <phase_3+0x7b>
  400f91:	b8 85 01 00 00       	mov    $0x185,%eax
  400f96:	eb 26                	jmp    400fbe <phase_3+0x7b>
  400f98:	b8 ce 00 00 00       	mov    $0xce,%eax
  400f9d:	eb 1f                	jmp    400fbe <phase_3+0x7b>
  400f9f:	b8 aa 02 00 00       	mov    $0x2aa,%eax
  400fa4:	eb 18                	jmp    400fbe <phase_3+0x7b>
  400fa6:	b8 47 01 00 00       	mov    $0x147,%eax
  400fab:	eb 11                	jmp    400fbe <phase_3+0x7b>
  400fad:	e8 88 04 00 00       	callq  40143a <explode_bomb>
  400fb2:	b8 00 00 00 00       	mov    $0x0,%eax 
  400fb7:	eb 05                	jmp    400fbe <phase_3+0x7b>
  400fb9:	b8 37 01 00 00       	mov    $0x137,%eax 
  400fbe:	3b 44 24 0c          	cmp    0xc(%rsp),%eax # 第二个参数和不同的eax比较,若不同则Boom.
  400fc2:	74 05                	je     400fc9 <phase_3+0x86>
  400fc4:	e8 71 04 00 00       	callq  40143a <explode_bomb>
  400fc9:	48 83 c4 18          	add    $0x18,%rsp
  400fcd:	c3                   	retq   
```

`0x4025cf`是`"%d %d"`,输入两个整数.

`0x402470`内容如下

```asm
0x402470:       0x7c    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x402478:       0xb9    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x402480:       0x83    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x402488:       0x8a    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x402490:       0x91    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x402498:       0x98    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x4024a0:       0x9f    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
0x4024a8:       0xa6    0x0f    0x40    0x00    0x00    0x00    0x00    0x00
```

发现这是一个switch语句的跳转表,应该是分别跳转到

```asm
case 0:400f7c	$0xcf,%eax
case 1:400fb9
case 2:400f83
case 3:400f8a
case 4:400f91
case 5:400f98
case 6:400f9f
case 7:400fa6
```

`0 207`

### phase_4

```asm
000000000040100c <phase_4>:
  40100c:	48 83 ec 18          	sub    $0x18,%rsp
  401010:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
  401015:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx # sscanf要存入的地址
  40101a:	be cf 25 40 00       	mov    $0x4025cf,%esi # 输入两个整数到rdx,rcx
  40101f:	b8 00 00 00 00       	mov    $0x0,%eax
  401024:	e8 c7 fb ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  401029:	83 f8 02             	cmp    $0x2,%eax
  40102c:	75 07                	jne    401035 <phase_4+0x29> # 输入两个参数 rdx,rcx
  40102e:	83 7c 24 08 0e       	cmpl   $0xe,0x8(%rsp) 
  401033:	76 05                	jbe    40103a <phase_4+0x2e> # 第一个参数必须满足rdx<=0xe(unsigned),否则BOOM
  401035:	e8 00 04 00 00       	callq  40143a <explode_bomb>
  40103a:	ba 0e 00 00 00       	mov    $0xe,%edx # edx=0xe# 置edx为e
  40103f:	be 00 00 00 00       	mov    $0x0,%esi # esi=0
  401044:	8b 7c 24 08          	mov    0x8(%rsp),%edi # edi=第一个参数
  401048:	e8 81 ff ff ff       	callq  400fce <func4> # 调用前初始化了rdx rsi rdi 前三个参数寄存器
  40104d:	85 c0                	test   %eax,%eax
  40104f:	75 07                	jne    401058 <phase_4+0x4c> # func4返回值!=0:BOOM
  401051:	83 7c 24 0c 00       	cmpl   $0x0,0xc(%rsp) # 如果第二个参数==0,成功
  401056:	74 05                	je     40105d <phase_4+0x51>
  401058:	e8 dd 03 00 00       	callq  40143a <explode_bomb>
  40105d:	48 83 c4 18          	add    $0x18,%rsp # 成功:func4返回0且第二个参数==0
  401061:	c3                   	retq   

```

```asm
0000000000400fce <func4>:
# edi=第一个参数;esi=0;edx=0xe
  400fce:	48 83 ec 08          	sub    $0x8,%rsp
  400fd2:	89 d0                	mov    %edx,%eax # eax=0xe
  400fd4:	29 f0                	sub    %esi,%eax # eax=0xe-esi(0)
  400fd6:	89 c1                	mov    %eax,%ecx # ecx=eax(0xe)
  # 三个参数edx ecx esi;esi初始值=0
  400fd8:	c1 e9 1f             	shr    $0x1f,%ecx # ecx=ecx>>>0x1f;shr k,D D=D>>k.ecx右移31位,是eax的符号位(shr逻辑右移) 
  400fdb:	01 c8                	add    %ecx,%eax # eax=ecx+eax eax+0or+1
  400fdd:	d1 f8                	sar    %eax # 算数右移shift arithmetic right  右移一位的简写.sar %eax = sar $1, %eax
  400fdf:	8d 0c 30             	lea    (%rax,%rsi,1),%ecx # ecx=rax+rsi
  400fe2:	39 f9                	cmp    %edi,%ecx # ecx<=edi(第一个参数) 那么第一个参数作为循环计数, ecx=7
  400fe4:	7e 0c                	jle    400ff2 <func4+0x24> # 一定要到这
  400fe6:	8d 51 ff             	lea    -0x1(%rcx),%edx # edx=rcx-1
  400fe9:	e8 e0 ff ff ff       	callq  400fce <func4> # 递归
  400fee:	01 c0                	add    %eax,%eax # eax*=2
  400ff0:	eb 15                	jmp    401007 <func4+0x39> # 结束递归
  400ff2:	b8 00 00 00 00       	mov    $0x0,%eax # eax=0
  400ff7:	39 f9                	cmp    %edi,%ecx
  400ff9:	7d 0c                	jge    401007 <func4+0x39> # ecx>=edi 结束递归 且 eax=0. 直到ecx==edi
  400ffb:	8d 71 01             	lea    0x1(%rcx),%esi # esi=rcx+1
  400ffe:	e8 cb ff ff ff       	callq  400fce <func4> # 递归
  401003:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax # eax=2*rax+1
  401007:	48 83 c4 08          	add    $0x8,%rsp
  40100b:	c3                   	retq   

```

first 6 arguments

`%rdi %rsi %rdx %rcx %r8 %r9 `不必假设保存值可随意使用

```c
int sscanf(     const char *buffer,     const char *format, [ argument ] ...   ); 
```

scanf把读取的值按顺序放入上面的寄存器指向的地址

rdi:&buffer

rsi:&format

rdx,rcx,r8,r9:待输入的变量的地址

```asm
edi		esi		edx		eax		ecx		
x		0		0xe		7		7
#按顺序手动跑一次，到400fe2为止寄存器的值如上，那么分析下面的语句 知道x应该填7
#返回后第二个参数应为0 故答案 7 0 
```



`7 0`

### phase_5

```asm
0000000000401062 <phase_5>:
  401062:	53                   	push   %rbx
  401063:	48 83 ec 20          	sub    $0x20,%rsp #stack往下拉四个字
  401067:	48 89 fb             	mov    %rdi,%rbx
  40106a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401071:	00 00 
  401073:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
  401078:	31 c0                	xor    %eax,%eax
  40107a:	e8 9c 02 00 00       	callq  40131b <string_length>
  40107f:	83 f8 06             	cmp    $0x6,%eax
  401082:	74 4e                	je     4010d2 <phase_5+0x70>
  401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
  401089:	eb 47                	jmp    4010d2 <phase_5+0x70>
  #------------------------------------------------------------------
  40108b:	0f b6 0c 03          	movzbl (%rbx,%rax,1),%ecx # ecx=rbx+rax
  40108f:	88 0c 24             	mov    %cl,(%rsp)# cl:rcx的最低一位字节
  401092:	48 8b 14 24          	mov    (%rsp),%rdx
  401096:	83 e2 0f             	and    $0xf,%edx
  401099:	0f b6 92 b0 24 40 00 	movzbl 0x4024b0(%rdx),%edx
  4010a0:	88 54 04 10          	mov    %dl,0x10(%rsp,%rax,1)
  4010a4:	48 83 c0 01          	add    $0x1,%rax
  4010a8:	48 83 f8 06          	cmp    $0x6,%rax
  4010ac:	75 dd                	jne    40108b <phase_5+0x29># 循环0~5，6次
  #------------------------------------------------------------------
  4010ae:	c6 44 24 16 00       	movb   $0x0,0x16(%rsp)
  4010b3:	be 5e 24 40 00       	mov    $0x40245e,%esi
  4010b8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
  4010bd:	e8 76 02 00 00       	callq  401338 <strings_not_equal># 参数:rdi/rsi
  4010c2:	85 c0                	test   %eax,%eax
  4010c4:	74 13                	je     4010d9 <phase_5+0x77>
  4010c6:	e8 6f 03 00 00       	callq  40143a <explode_bomb>
  4010cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  4010d0:	eb 07                	jmp    4010d9 <phase_5+0x77>
  4010d2:	b8 00 00 00 00       	mov    $0x0,%eax
  4010d7:	eb b2                	jmp    40108b <phase_5+0x29>
  4010d9:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
  4010de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
  4010e5:	00 00 
  4010e7:	74 05                	je     4010ee <phase_5+0x8c>
  4010e9:	e8 42 fa ff ff       	callq  400b30 <__stack_chk_fail@plt>
  4010ee:	48 83 c4 20          	add    $0x20,%rsp
  4010f2:	5b                   	pop    %rbx
  4010f3:	c3                   	retq   
```

```asm
0x4024b0 <array.3449>:  0x6d    0x61    0x64    0x75    0x69    0x65    0x72    0x73
0x4024b8 <array.3449+8>:  0x6e    0x66    0x6f    0x74    0x76    0x62    0x79    0x6c
```

`maduiersnfotvbyl`





`rsi=0x40245e`=>

```asm
0x40245e:       0x66    0x6c    0x79    0x65    0x72    0x73    0x00    0x00
```

转成ascii:`flyers`



那么上面的下标对应应该为：

`9 15 14 5 6 7 `

又因为

```asm
  401096:	83 e2 0f             	and    $0xf,%edx
```



### phase_6

```asm
00000000004010f4 <phase_6>:
  4010f4:	41 56                	push   %r14
  4010f6:	41 55                	push   %r13
  4010f8:	41 54                	push   %r12
  4010fa:	55                   	push   %rbp
  4010fb:	53                   	push   %rbx
  4010fc:	48 83 ec 50          	sub    $0x50,%rsp#开了10个字
  401100:	49 89 e5             	mov    %rsp,%r13
  401103:	48 89 e6             	mov    %rsp,%rsi
  401106:	e8 51 03 00 00       	callq  40145c <read_six_numbers>
  40110b:	49 89 e6             	mov    %rsp,%r14
  40110e:	41 bc 00 00 00 00    	mov    $0x0,%r12d
  #--------------------------------------------------------------# 
  401114:	4c 89 ed             	mov    %r13,%rbp
  401117:	41 8b 45 00          	mov    0x0(%r13),%eax# 读入的第一个数
  40111b:	83 e8 01             	sub    $0x1,%eax
  40111e:	83 f8 05             	cmp    $0x5,%eax
  401121:	76 05                	jbe    401128 <phase_6+0x34># 读入的每一个数 满足<=6
  401123:	e8 12 03 00 00       	callq  40143a <explode_bomb>
  401128:	41 83 c4 01          	add    $0x1,%r12d #r12d:32位 0+1
  40112c:	41 83 fc 06          	cmp    $0x6,%r12d
  401130:	74 21                	je     401153 <phase_6+0x5f> 
  401132:	44 89 e3             	mov    %r12d,%ebx # ebx=1
  #--------------------------------------------------------------# 循环6次
  401135:	48 63 c3             	movslq %ebx,%rax # rax=rbx
  401138:	8b 04 84             	mov    (%rsp,%rax,4),%eax # rax=*(rsp+4*rax)
  40113b:	39 45 00             	cmp    %eax,0x0(%rbp) # 数字单调不减
  40113e:	75 05                	jne    401145 <phase_6+0x51>
  401140:	e8 f5 02 00 00       	callq  40143a <explode_bomb>
  401145:	83 c3 01             	add    $0x1,%ebx
  401148:	83 fb 05             	cmp    $0x5,%ebx
  40114b:	7e e8                	jle    401135 <phase_6+0x41>
  40114d:	49 83 c5 04          	add    $0x4,%r13
  401151:	eb c1                	jmp    401114 <phase_6+0x20>
  #--------------------------------------------------------------
  401153:	48 8d 74 24 18       	lea    0x18(%rsp),%rsi# rsi往回指3个字
  401158:	4c 89 f0             	mov    %r14,%rax # rax=rsp
  40115b:	b9 07 00 00 00       	mov    $0x7,%ecx # rcx=7
  #==============================================================
  401160:	89 ca                	mov    %ecx,%edx # rdx=rcx=7
  401162:	2b 10                	sub    (%rax),%edx # rdx=7-*(rax)
  401164:	89 10                	mov    %edx,(%rax)
  401166:	48 83 c0 04          	add    $0x4,%rax
  40116a:	48 39 f0             	cmp    %rsi,%rax
  40116d:	75 f1                	jne    401160 <phase_6+0x6c>#这一段把输入的六个整数全部-7
  #==============================================================
  40116f:	be 00 00 00 00       	mov    $0x0,%esi
  401174:	eb 21                	jmp    401197 <phase_6+0xa3>
  
  #**************************************************************
  401176:	48 8b 52 08          	mov    0x8(%rdx),%rdx # rdx往node后面找
  #from 4011a9,rax=1,rdx=0x6032d0,rcx=:"这个数字"
  40117a:	83 c0 01             	add    $0x1,%eax
  40117d:	39 c8                	cmp    %ecx,%eax#"这个数字"==2?以此类推
  40117f:	75 f5                	jne    401176 <phase_6+0x82>
  401181:	eb 05                	jmp    401188 <phase_6+0x94>
  #**************************************************************
  401183:	ba d0 32 60 00       	mov    $0x6032d0,%edx
  401188:	48 89 54 74 20       	mov    %rdx,0x20(%rsp,%rsi,2)
  # (rsp+2*rsi+0x20)=rdx=0x6032d0 从rsp+0x20开始一个字存一个0x6032d0+8*x(x=rcx-1)
  40118d:	48 83 c6 04          	add    $0x4,%rsi
  401191:	48 83 fe 18          	cmp    $0x18,%rsi # 六个数(0x18/0x4=6)
  401195:	74 14                	je     4011ab <phase_6+0xb7>
  401197:	8b 0c 34             	mov    (%rsp,%rsi,1),%ecx
  # from 401174,rsi=0,rsp=-7过的输入的第一个数字, ecx=*(rsp+rsi)
  40119a:	83 f9 01             	cmp    $0x1,%ecx #这个数字<=1
  40119d:	7e e4                	jle    401183 <phase_6+0x8f>
  
  # 若这个数>1:
  40119f:	b8 01 00 00 00       	mov    $0x1,%eax #rax=1
  4011a4:	ba d0 32 60 00       	mov    $0x6032d0,%edx #rdx=0x6032d0
  4011a9:	eb cb                	jmp    401176 <phase_6+0x82>
  #**************************************************************
  4011ab:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx 
  # from 401191,rbx从刚才的rsp+0x20开始
  4011b0:	48 8d 44 24 28       	lea    0x28(%rsp),%rax
  
  4011b5:	48 8d 74 24 50       	lea    0x50(%rsp),%rsi
  4011ba:	48 89 d9             	mov    %rbx,%rcx# rcx=rsp+0x20
  #>from 4011d0
  4011bd:	48 8b 10             	mov    (%rax),%rdx# rdx=第二个数指向的node
  4011c0:	48 89 51 08          	mov    %rdx,0x8(%rcx)# 栈中的node地址换成值
  4011c4:	48 83 c0 08          	add    $0x8,%rax
  4011c8:	48 39 f0             	cmp    %rsi,%rax# rax一个字一个字地加到0x50
  4011cb:	74 05                	je     4011d2 <phase_6+0xde>
  4011cd:	48 89 d1             	mov    %rdx,%rcx# rcx=链表的下一个地址
  4011d0:	eb eb                	jmp    4011bd <phase_6+0xc9>
  
  4011d2:	48 c7 42 08 00 00 00 	movq   $0x0,0x8(%rdx)
  4011d9:	00 
  4011da:	bd 05 00 00 00       	mov    $0x5,%ebp
  #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  4011df:	48 8b 43 08          	mov    0x8(%rbx),%rax
  4011e3:	8b 00                	mov    (%rax),%eax
  4011e5:	39 03                	cmp    %eax,(%rbx)# eax的值>=下一个数
  4011e7:	7d 05                	jge    4011ee <phase_6+0xfa>
  4011e9:	e8 4c 02 00 00       	callq  40143a <explode_bomb>
  4011ee:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
  4011f2:	83 ed 01             	sub    $0x1,%ebp
  4011f5:	75 e8                	jne    4011df <phase_6+0xeb>
  #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  4011f7:	48 83 c4 50          	add    $0x50,%rsp
  4011fb:	5b                   	pop    %rbx
  4011fc:	5d                   	pop    %rbp
  4011fd:	41 5c                	pop    %r12
  4011ff:	41 5d                	pop    %r13
  401201:	41 5e                	pop    %r14
  401203:	c3                   	retq   

```

```asm
0x6032d0 <node1>:       0x4c    0x01    0x00    0x00    0x01    0x00    0x00    0x00
0x6032d8 <node1+8>:     0xe0    0x32    0x60    0x00    0x00    0x00    0x00    0x00
0x6032e0 <node2>:       0xa8    0x00    0x00    0x00    0x02    0x00    0x00    0x00
0x6032e8 <node2+8>:     0xf0    0x32    0x60    0x00    0x00    0x00    0x00    0x00
0x6032f0 <node3>:       0x9c    0x03    0x00    0x00    0x03    0x00    0x00    0x00
0x6032f8 <node3+8>:     0x00    0x33    0x60    0x00    0x00    0x00    0x00    0x00
0x603300 <node4>:       0xb3    0x02    0x00    0x00    0x04    0x00    0x00    0x00
0x603308 <node4+8>:     0x10    0x33    0x60    0x00    0x00    0x00    0x00    0x00
0x603310 <node5>:       0xdd    0x01    0x00    0x00    0x05    0x00    0x00    0x00
0x603318 <node5+8>:     0x20    0x33    0x60    0x00    0x00    0x00    0x00    0x00
0x603320 <node6>:       0xbb    0x01    0x00    0x00    0x06    0x00    0x00    0x00
0x603328 <node6+8>:     0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x603330:       0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x603338:       0x00    0x00    0x00    0x00    0x00    0x00    0x00    0x00
0x603340 <host_table>:  0x29    0x26    0x40    0x00    0x00    0x00    0x00    0x00
0x603348 <host_table+8>:        0x43    0x26    0x40    0x00    0x00    0x00    0x00    0x00
```









### secret_phase

```asm
0000000000401242 <secret_phase>:
  401242:	53                   	push   %rbx
  401243:	e8 56 02 00 00       	callq  40149e <read_line>
  401248:	ba 0a 00 00 00       	mov    $0xa,%edx
  40124d:	be 00 00 00 00       	mov    $0x0,%esi
  401252:	48 89 c7             	mov    %rax,%rdi
  401255:	e8 76 f9 ff ff       	callq  400bd0 <strtol@plt>
  40125a:	48 89 c3             	mov    %rax,%rbx
  40125d:	8d 40 ff             	lea    -0x1(%rax),%eax
  401260:	3d e8 03 00 00       	cmp    $0x3e8,%eax
  401265:	76 05                	jbe    40126c <secret_phase+0x2a>
  401267:	e8 ce 01 00 00       	callq  40143a <explode_bomb>
  40126c:	89 de                	mov    %ebx,%esi
  40126e:	bf f0 30 60 00       	mov    $0x6030f0,%edi
  401273:	e8 8c ff ff ff       	callq  401204 <fun7>
  401278:	83 f8 02             	cmp    $0x2,%eax
  40127b:	74 05                	je     401282 <secret_phase+0x40>
  40127d:	e8 b8 01 00 00       	callq  40143a <explode_bomb>
  401282:	bf 38 24 40 00       	mov    $0x402438,%edi
  401287:	e8 84 f8 ff ff       	callq  400b10 <puts@plt>
  40128c:	e8 33 03 00 00       	callq  4015c4 <phase_defused>
  401291:	5b                   	pop    %rbx
  401292:	c3                   	retq   
  401293:	90                   	nop
  401294:	90                   	nop
  401295:	90                   	nop
  401296:	90                   	nop
  401297:	90                   	nop
  401298:	90                   	nop
  401299:	90                   	nop
  40129a:	90                   	nop
  40129b:	90                   	nop
  40129c:	90                   	nop
  40129d:	90                   	nop
  40129e:	90                   	nop
  40129f:	90                   	nop
```

```asm
0000000000401204 <fun7>:
  401204:	48 83 ec 08          	sub    $0x8,%rsp
  401208:	48 85 ff             	test   %rdi,%rdi
  40120b:	74 2b                	je     401238 <fun7+0x34>
  40120d:	8b 17                	mov    (%rdi),%edx
  40120f:	39 f2                	cmp    %esi,%edx
  401211:	7e 0d                	jle    401220 <fun7+0x1c>
  401213:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
  401217:	e8 e8 ff ff ff       	callq  401204 <fun7>
  40121c:	01 c0                	add    %eax,%eax
  40121e:	eb 1d                	jmp    40123d <fun7+0x39>
  401220:	b8 00 00 00 00       	mov    $0x0,%eax
  401225:	39 f2                	cmp    %esi,%edx
  401227:	74 14                	je     40123d <fun7+0x39>
  401229:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
  40122d:	e8 d2 ff ff ff       	callq  401204 <fun7>
  401232:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
  401236:	eb 05                	jmp    40123d <fun7+0x39>
  401238:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
  40123d:	48 83 c4 08          	add    $0x8,%rsp
  401241:	c3                   	retq   

```







`ionefg`

### hint:

`gdb`

有一个非常方便的单页[gdb摘要](http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Debug.html)

[x86-64 gdb总结](https://github.com/Qasak/all-about-csapp-labs/blob/master/bomblab/gdbnotes-x86-64.pdf)

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

