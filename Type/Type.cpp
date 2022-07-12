#include "Type.hpp"

Product_Type::Product_Type(Type* T1,Type* T2)
{
    NameofClass=string("Product_Type");
    value.push_back(T1);
    value.push_back(T2);
}

Product_Type::Product_Type(Type* T1,Type* T2,int n)
{
    NameofClass=string("Product_Type");
    value.push_back(T1);
    while(--n>=0)
        value.push_back(T2);
    
}

Array_Type:: Array_Type(int I,BasicType T)
{
    NameofClass=string("Array_Type");
    value="("+to_string(I)+","+T.getvalue()+")";
}

Array_Type::  Array_Type(int I,Array_Type T)
{
    NameofClass=string("Array_Type");
    value="("+to_string(I)+","+T.getvalue()+")";
}

Fun_Type::Fun_Type(vector<Type *> T)
{
    NameofClass=string("Fun_Type");
    value.assign(T.begin(),T.end());
}




