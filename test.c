/*这里是测试语法树输出的文件*/
int m=0;
int main(int c,int b)
{
    int a=1;
    a=1;
    main(a+1,c);
    // ( ( (a[3]) [1]) [2])=1;
    // t0=3*15
    // t0=t0+1
    // t1=t0*20
    // t1=t1+2
    // t2=a基地址
    // t3=4*t1
    // t4=t2[t3]
    return a;
}
int main1(int c,int b)
{
    int a=1;
    a=1;
    // ( ( (a[3]) [1]) [2])=1;
    // t0=3*15
    // t0=t0+1
    // t1=t0*20
    // t1=t1+2
    // t2=a基地址
    // t3=4*t1
    // t4=t2[t3]
    return a;
}
