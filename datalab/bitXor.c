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