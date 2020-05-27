

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

### level 1

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





