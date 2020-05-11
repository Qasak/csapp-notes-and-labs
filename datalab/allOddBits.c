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