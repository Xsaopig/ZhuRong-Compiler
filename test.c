/*这里是测试语法树输出的文件*/

int main()
{
    int a[10][15][20];
    a[3][1][2]=1;
    int c;
    // ( ( (a[3]) [1]) [2])=1;
    // t0=3*15
    // t0=t0+1
    // t1=t0*20
    // t1=t1+2
    // t2=a基地址
    // t3=4*t1
    
    return 0;
}
