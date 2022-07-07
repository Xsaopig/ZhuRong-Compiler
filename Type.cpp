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

