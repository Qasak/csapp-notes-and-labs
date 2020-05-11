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