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




//枚举打表用的函数
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