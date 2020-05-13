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