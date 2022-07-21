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
    bool is_BasicType() {return NameofClass.compare("BasicType")==0?true:false;}
    bool is_Product_Type() {return NameofClass.compare("Product_Type")==0?true:false;}
    bool is_Array_Type() {return NameofClass.compare("Array_Type")==0?true:false;}
    bool is_Fun_Type() {return NameofClass.compare("Fun_Type")==0?true:false;}
    bool is_int();
    bool is_float();
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
    void setvalue(string s){value=s;}
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
    bool sameas(vector<Type *> &t);//比较两个积类型是否相同
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
    void setBasicType(BasicType& t);//设置basictype
    void set_elements_nums(int index,int value);//设置elements_nums[index]=value，同时更新value
    int bytes_occupied();          //数组占用的字节数
    Type* Lower_one_level();      //数组降低一层
    Type* Lower_one_level_forward();//数组降低一层，但是是elements_num从前往后降低一层
    int lev=0;                    //数组的层数
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
    void updatevalue();
    int num_args=0;     //参数的个数    
    BasicType basictype;//函数的返回值类型
    vector<Type *> args;//参数的类型
private:
};

#endif