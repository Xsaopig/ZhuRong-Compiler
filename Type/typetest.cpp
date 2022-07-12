#include <iostream>
#include "Type.hpp"
#define TYPE dynamic_cast<Type*>
using namespace std;
int main(){
    BasicType INT("INT"),FLOAT("FLOAT"),VOID("VOID");
    
    Product_Type p1(TYPE(&INT),TYPE(&FLOAT));
    cout<<p1.getvalue()<<endl;

    Product_Type p2(TYPE(&INT),TYPE(&VOID),2);
    cout<<p2.getvalue()<<endl;

    Product_Type p3(TYPE(&INT),TYPE(&INT));
    cout<<p3.getvalue()<<endl;

    Array_Type arr1(3,INT);
    cout<<arr1.getvalue()<<endl;

    Array_Type arr2(4,arr1);
    cout<<arr2.getvalue()<<endl;

    vector<Type*> x={TYPE(&INT),TYPE(&FLOAT),TYPE(&INT)};
    Fun_Type func1(INT,x);
    cout<<func1.getvalue()<<endl;

}