/*这里是测试语法树输出的文件*/
// #include "stdio.h"
// #define A (a=a+c)
int main()
{
    int a,b[20],c;
    // int 0b;
    b[0]=10;
    b[1]=0x11;
    b[2]=017;
    b[3]=1.1e11;
    b[4]=1.1e-11;
    b[5]=1.1e+11;
    b[6]=0x1.1p2f;
    b[7]=0x1.1p-2f;
    b[8]=0x1.1p+2f;
    a=get(a,b[10],c);
    return 0;
}