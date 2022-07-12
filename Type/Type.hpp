#ifndef    HEADER_TYPE
#define    HEADER_TYPE

#include <vector>
#include <iostream>
using namespace std;

//基类
class Type
{
public:
    Type() {};
    ~Type() {};
    string getNameofClass() {return NameofClass;}//获取当前对象的类名，类型转换时要用
    string getvalue() {return value;}
protected:
    string NameofClass;
    string value;
};

//基础类型T，包括 int、float、void
class BasicType :public Type
{
public:
    BasicType() {NameofClass=string("BasicType");};
    ~BasicType() {};
    BasicType(string type) {NameofClass=string("BasicType");value=type;}
private:
};

//积类型，如<T1,T2>，T1可以是基础类型，也可以是积类型等
class Product_Type :public Type
{
public:
    Product_Type() {NameofClass=string("Product_Type");};
    Product_Type(Type* t1,Type* t2);
    Product_Type(Type* t1,Type* t2,int n);
    ~Product_Type() {};
    vector<Type *> eles;//每个元素的类型
private:
};

//数组类型，如Array(I,T),Array(I,array(I,T))
class Array_Type :public Type
{
public:
    Array_Type() {NameofClass=string("Array_Type");};
    Array_Type(int i,BasicType& t);
    Array_Type(int i,Array_Type& t);
    ~Array_Type() {};
    int lev;                    //数组的层数
    vector<int> elements_nums;  //数组每层元素的个数，最里层元素在第一个
    BasicType basictype;        //数组的基本类型    
private:

};

//函数类型，如int fun(T)
class Fun_Type :public Type
{
public:
    Fun_Type() {NameofClass=string("Fun_Type");};
    Fun_Type(BasicType& R,vector<Type *> T);
    ~Fun_Type() {};
    string getvalue() {return value;}
    BasicType basictype;//函数的返回值类型    
    vector<Type *> args;//参数的类型
private:
};

#endif