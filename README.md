# all-about-csapp-labs
Solutions of Computer Systems: A Programmer's Perspective's labs

CSAPP lab全部题解

> All solutions written by github user [Qasak](https://qasak.github.io/).
> as O'Hallaron said, whatever you do, don't cheat.
>

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

2.观察到用&和~可以实现映射`f=lambda x,y:~x&y`

`f(a,b)`即:在a和b的相同位上,将x为0且y为1的位置为1

那么`x^y`可描述为`f(a,b)|f(b,a)`

又 `a|b`可描述为`g=lambda a,b:~(~a&~b)`

综上,`x^y=g(f(x,y),f(y,x))`

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
### tmin
```c
/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
 int tmin(void) {
    int ans=1<<31;
    return ans;
}
```

### isTmax

判断有符号整数int是否是Tmax 0x7fffffff.

这道题不能用移位运算符

```c
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */

/*
这道题不能用移位运算符
    设x是Tmax,那么有t=x+1==Tmin.
    t^x==1111 1111 1111 1111
    !(~(t^x))==1
    
    满足(x+1)^x==1111 1111 1111 1111的x有且仅有两个
    x==-1
    x==0x7fffffff
    
    eg        x==1111 1111 1111 1111
            x+1==0000 0000 0000 0000
        x^(x+1)==1111 1111 1111 1111
    
    eg        x==0111 1111 1111 1111
            x+1==1000 0000 0000 0000
        x^(x+1)==1111 1111 1111 1111
    
    
    eg        x==0011 1111 1111 1111
            x+1==0100 0000 0000 0000
        x^(x+1)==0111 1111 1111 1111
        
    eg        x==0001 1111 1111 1111
            x+1==0010 0000 0000 0000
        x^(x+1)==0011 1111 1111 1111  
 */
int isTmax(int x) {
    int t=x+1;
    return !((~(t^x))|!t);
}
```



### allOddBits

```c
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */

/*
    如果x所有奇数位是1,返回1
    奇数位都是1的数|偶数位都是1数==0xffffffff
    eg
        0101
        1010 |
        ----
        1111
        
        若x任意奇数位为0,则
        ~((x)|(偶数位都是1的数))!=0
        故
        return !(~((x)|(偶数位都是1的数)))
    
    由于常数范围[0,0xff]
    故用4个0x55凑成0x55555555
    0x55555555== 0101 0101 0101 0101 0101 0101 0101 0101 
    
 */
int allOddBits(int x) {
    int v0=0x55;
    int v1=0x55|v0<<8;
    int v2=0x55|v1<<8;
    int v3=0x55|v2<<8;
  return !(~(x|v3));
}
```



### negate

```c
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
 
/*
    负
 */
 
int negate(int x) {
  return (~x+1);
}
```



### isAsciiDigit

```c
/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
 
/*
    key0是0x30的反,即
     0x30=0000 0000 0000 ... 0011 0000
    ~0x30=1111 1111 1111 ... 1100 1111
    取x的前28位
    x&~0xf
    则x需要满足0x3*的形式
    即
    x=    0000 0000 0000 ... 0011 xxxx
    然后x的后四位<10即可
 */
int isAsciiDigit(int x) {
  int key0=~0x30;
  int part0=x&~0xf;
  int part1=x&0xf;
  return ((!(~(key0^part0)))&((part1+(~10+1))>>31&1));
}
```



### conditional

```c
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
 
/*
    模拟C的条件运算符?:
    先把x转成逻辑01
    即int t=!x
    x==0    t==1
    x==1    t==0
    当x==0   !x==1   t==1    !t==0
    当x==1   !x==0   t==0    !t==1
    t和!t总是相反
    那么与运算符两边总有一个是0,这就起到了选择的作用
    再利用C算数右移的性质,用0xffffffff还原出z/y
    
*/
int conditional(int x, int y, int z) {
    int t=!x;
    return (((t<<31)>>31)&z)|((((!t)<<31)>>31)&y);
}
```

### isLessOrEqual

```c
/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
    /* x<=y
       <=> !(y<x) ,在(y+(-x))不溢出的情况下
       <=> !(y+(-x)<0)
       <=> !(((y+(~x+1))>>31)&1) 即:
                                    两数和为负则return 0,
                                          为正则return 1.
                                          
           溢出时:两个参数彼此同号,其和与二者异号
           <=> ((!(((y>>31)&1)^(((~x+1)>>31)&1))) ^ (((y+(~x+1))>>31)&1))
            溢出的情况下:
            1) 和溢出为负,y,(-x)是两个正数.此时一定有x<y成立 return 1
            2) 和溢出为正,y,(-x)是两个负数.此时一定有x>y成立 return 0
            那么有真值表

                    是否溢出        和是否为负   返回值
            y+(-x)  0               0            1
                    0               1            0
                    1               0            0
                    1               1            1


       综上:
    */
int isLessOrEqual(int x, int y) {
    int sign_y=(y>>31)&1;
    int sign_m_x=((~x+1)>>31)&1;
    int sum=y+(~x+1);
    int sign_sum=(sum>>31)&1;
    int is_same_sign=(!(sign_y^sign_m_x));
    int of_flag=is_same_sign&(sign_y^sign_sum);
    return ((!(of_flag^sign_sum))|(!(~((~(1<<31))^x))));//注意当x==Tmin,x>0,(即两数异号不会溢出的情况)由于Tmin=-Tmin,会导致y+(-x)永远<0(而两数均为Tmin时==0)
                                                        //故判定失效,此时需要将x==Tmin特殊处理,即x==Tmin时总是返回1.
}

```

### logicalNeg

```c
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
 
/*
    实现!逻辑非运算符
    利用-0的符号位也是0的性质
    若x!=0
    x符号位是0，则-x符号位是1
    x符号位是1，则-x符号位是0(Tmin和-Tmin符号位总是1)
    
*/
int logicalNeg(int x) {
    return (((((~x+1)>>31)|x>>31)&1)^1);
}
```

### howManyBits

```c
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */

/*
   dec         bin                                      bits
   fffffff1    1111 1111 1111 1111 1111 1111 1111 0001  5
   fffffff2    1111 1111 1111 1111 1111 1111 1111 0010  5
   fffffff3    1111 1111 1111 1111 1111 1111 1111 0011  5
   fffffff4    1111 1111 1111 1111 1111 1111 1111 0100  5
   fffffff5    1111 1111 1111 1111 1111 1111 1111 0101  5
   fffffff6    1111 1111 1111 1111 1111 1111 1111 0110  5
   fffffff7    1111 1111 1111 1111 1111 1111 1111 0111  5
   fffffff8    1111 1111 1111 1111 1111 1111 1111 1000  4
   fffffff9    1111 1111 1111 1111 1111 1111 1111 1001  4
   fffffffa    1111 1111 1111 1111 1111 1111 1111 1010  4
   fffffffb    1111 1111 1111 1111 1111 1111 1111 1011  4
   fffffffc    1111 1111 1111 1111 1111 1111 1111 1100  3
   fffffffd    1111 1111 1111 1111 1111 1111 1111 1101  3
   fffffffe    1111 1111 1111 1111 1111 1111 1111 1110  2
   ffffffff    1111 1111 1111 1111 1111 1111 1111 1111  1
   0           0000 0000 0000 0000 0000 0000 0000 0000  1
   1           0000 0000 0000 0000 0000 0000 0000 0001  2
   2           0000 0000 0000 0000 0000 0000 0000 0010  3
   3           0000 0000 0000 0000 0000 0000 0000 0011  3
   4           0000 0000 0000 0000 0000 0000 0000 0100  4
   5           0000 0000 0000 0000 0000 0000 0000 0101  4
   6           0000 0000 0000 0000 0000 0000 0000 0110  4
   7           0000 0000 0000 0000 0000 0000 0000 0111  4
   8           0000 0000 0000 0000 0000 0000 0000 1000  5
   9           0000 0000 0000 0000 0000 0000 0000 1001  5
   a           0000 0000 0000 0000 0000 0000 0000 1010  5
   b           0000 0000 0000 0000 0000 0000 0000 1011  5
   c           0000 0000 0000 0000 0000 0000 0000 1100  5
   d           0000 0000 0000 0000 0000 0000 0000 1101  5
   e           0000 0000 0000 0000 0000 0000 0000 1110  5
   
   思路:
   对于符号位为1的数:nbits=从左往右第一个0所在位+1
   对于符号位为0的数:nbits=从左往右第一个1所在位+1
   观察可知对于所需位数关于符号位是0或1是对称的.
   那么对于符号位是1的数,转换成符号位是1的数,直接取反,这里可以用前面的conditional做选择
   对于符号位是0的数,将最高位的1扩散到剩余所有位,然后数1的数量,再+1就得到了表达该数至少需要的bit数
   那么如何数1的个数呢？
   可以用分治法
   eg:
        (1 1) (0 1) (1 0) (0 1)
        (2   +   1) (1   +   1)
             3     +     2
                   5
*/
int howManyBits(int x) {
    int t=((x>>31)&1);
    int n,v0,v1,p0,p1,q0,q1,fac_1,fac_2,fac_3,fac_4,fac_5;
    x=((((!t)<<31)>>31)&x)|(((t<<31)>>31)&~x);

    x=x|x>>1;
    x=x|x>>2;
    x=x|x>>4;
    x=x|x>>8;
    x=x|x>>16;

    n=x;
    v0=0x55;
    v1=v0|(v0<<8);
    fac_1=v1|(v1<<16);

    p0=0x33;
    p1=p0|(p0<<8);
    fac_2=p1|(p1<<16);

    q0=0x0f;
    q1=q0|(q0<<8);
    fac_3=q1|(q1<<16);

    fac_4=0xff|(0xff<<16);

    fac_5=0xff|(0xff<<8);

    n=(n&fac_1)+((n>>1)&fac_1);
    n=(n&fac_2)+((n>>2)&fac_2);
    n=(n&fac_3)+((n>>4)&fac_3);
    n=(n&fac_4)+((n>>8)&fac_4);
    n=(n&fac_5)+((n>>16)&fac_5);

    return (n+1);//ops 58
}




//以下是枚举打表用的函数
int _howManyBits(int x){
    int ans=0;
    if((x>>31)&1)
    {
        int cnt=1;
        int pos_0=0;
        int t=x;
        while(cnt<=32)
        {
            if(!(t&1)) pos_0=cnt;
            t>>=1;
            cnt++;
        }
        ans=pos_0+1;
    }
    else
    {
        int cnt=1;
        int pos_1=0;
        int t=x;
        while(cnt<=32)
        {
            if((t&1)) pos_1=cnt;
            t>>=1;
            cnt++;
        }
        ans=pos_1+1;
    }
    return ans;
}

int showbits(int x)
{
    int i=32;
    while(i--)
    {
        cout<<((x>>31)&1);
        x<<=1;
        cout<<((!(i%4))?" ":"");
    }
    cout<<" ";
}
int main() {
    //freopen("out.txt","w",stdout);
    int n=0xff;
    start=clock();
    for(int i=0x0;i<n;i++)
    {
        int r1=i;
        cout<<hex<<"   "<<r1<<"    ";
        showbits(r1);
        cout<<_howManyBits(r1)<<endl;
    }
  return 0;
}
```

### floatScale2

```c
/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
 
/*
 *floatScale2-返回与表达式2*f位级别等价的值

*浮点参数f。

*参数和结果都作为无符号int传递，但是它们将被解释为单精度浮点值。

*当参数为NaN时，返回参数

*合法操作,任何整数/无符号操作,包括||,&&,if,while

*最大操作数：30 
*/
unsigned floatScale2(unsigned uf) {
    int test_0=uf<<1;
    int e=(uf>>23)&0xff;
    int sign=(uf>>31)&1;
    if(!(test_0))   return uf;
    if((!(~(e^(~0xff))))) return uf;
    if(e)    return ((uf+(0x80<<16)));
    return ((uf<<1)|(sign<<31));//ops 15
}
//注意Tmin/0,NaN,inf这几个特殊值的处理,以及normalized,denormalized的区分

```

### floatFloat2Int

```c
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */

/*
    实现强制类型转换(int) float
    注意幂指数为正负时对应左移右移M
 */
 
int floatFloat2Int(unsigned uf) {
    int ans,sign,e,m,bias,exp,point_r=0,point_l=0;
    unsigned u;
    u=1u<<31;
    if((uf>>31)&1) sign=-1;
    else sign=1;
    e=(uf>>23)&0xff;
    m=uf&(~((1<<31)>>8));
    bias=127;
    exp=e-bias;

    if(exp>30) return u;
    if(exp<0) return 0;
    else if(exp<23) {point_l=1<<exp;point_r=m>>(23-exp);}
    else {point_l=1<<exp;point_r=m<<(exp-23);}
    ans=((point_l+point_r)*(sign));
    return ans;//ops 22
}
```



### floatPower2

```c
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */

/*
    返回2.0的x次幂(x是32位整数)
    如果结果太小而不能表示为denorm,返回0
    如果太大,返回+inf
 */
 
//这个版本if太多,被卡10s,TIME_OUT_LIMIT调到13s勉强能过
unsigned floatPower2(int x) {
    int t;
    if(x>128) x=128;
    if(x<=-150) x=-150;
    t=(1<<7)-1;//exp=t-bias==127-127=0
    t=t+x;
    if(t<=0)
    {
        t=((1<<22)>>-t);
    }
    else
    {

        t=t<<23;
    }
    return t;
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

