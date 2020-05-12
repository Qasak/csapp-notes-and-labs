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
    int sign_sum=((y+(~x+1))>>31)&1;
    int is_same_sign=(!(sign_y^sign_m_x));
    int of_flag=is_same_sign&(sign_y^sign_sum);
    return (!(of_flag^sign_sum));
}
