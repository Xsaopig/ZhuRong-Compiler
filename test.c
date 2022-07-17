//test domain of global var define and local define
int a = 3;
float b = 5;

int main(){
    int a = 5;
    if(a>5)
      a=a+1;
    else
      b=b+1;
    return a + b;
}