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
protected:
    string NameofClass;
};

//基础类型T，包括 int、float、bolean、void，还有ok、type_error
class BasicType :public Type
{
public:
    BasicType() {NameofClass=string("BasicType");};
    ~BasicType() {};
    BasicType(string type) {NameofClass=string("BasicType");value=type;}
    string& getvalue() {return value;}
private:
    string value;
};

//积类型，如<T1,T2>，T1可以是基础类型，也可以是积类型等
class Product_Type :public Type
{
public:
    Product_Type() {NameofClass=string("Product_Type");};
    Product_Type(Type* t1,Type* t2);
    Product_Type(Type* t1,Type* t2,int n);
    ~Product_Type() {};
    vector<Type *> getvalue() {return value;}
private:
    vector<Type *> value;
};

//指针类型，如pointer(T)
class Pointer_Type:public Type
{
 public:
    Pointer_Type() {NameofClass=string("Pointer_Type");};
    Pointer_Type(Type& t) {NameofClass=string("Pointer_Type");value=&t;};
    ~Pointer_Type() {};
    Type* getvalue() {return value;}
private:
    Type* value;
};
