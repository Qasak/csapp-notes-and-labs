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