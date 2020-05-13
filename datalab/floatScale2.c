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
