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

Fun_Type::Fun_Type(Type* T1)
{
    NameofClass=string("Fun_Type");
    value.push_back(T1);
}

Fun_Type::Fun_Type(Type* T1,Type* T2)
{
    NameofClass=string("Fun_Type");
    value.push_back(T1);
    value.push_back(T2);
}

Fun_Type::Fun_Type(Type* T1,Type* T2,Type *T3)
{
    NameofClass=string("Fun_Type");
    value.push_back(T1);
    value.push_back(T2);
    value.push_back(T3);
}

Fun_Type::Fun_Type(Type* T1,Type* T2,Type *T3,Type* T4)
{
    NameofClass=string("Fun_Type");
    value.push_back(T1);
    value.push_back(T2);
    value.push_back(T3);
    value.push_back(T4);
}




