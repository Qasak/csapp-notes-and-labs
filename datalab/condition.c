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