#include "Type.hpp"

Product_Type::Product_Type(Type* T1,Type* T2)
{
    NameofClass=string("Product_Type");
    eles.push_back(T1);
    eles.push_back(T2);
    value="<"+T1->getvalue()+","+T2->getvalue()+">";
}

Product_Type::Product_Type(Type* T1,Type* T2,int n)
{
    NameofClass=string("Product_Type");
    eles.push_back(T1);
    value="<"+T1->getvalue();
    while(--n>=0){
        eles.push_back(T2);
        value+=","+T2->getvalue();
    }
    value+=">";
    
}

Array_Type::Array_Type(int I,BasicType& T)
{
    NameofClass=string("Array_Type");
    value=T.getvalue()+"["+to_string(I)+"]";
    lev=1;
    elements_nums.push_back(I);
    basictype=T;
}

Array_Type::Array_Type(int I,Array_Type& T)
{
    NameofClass=string("Array_Type");
    value=T.getvalue()+"["+to_string(I)+"]";
    lev=T.lev+1;
    elements_nums=T.elements_nums;
    elements_nums.push_back(I);
    basictype=T.basictype;
}

Fun_Type::Fun_Type(BasicType& R,vector<Type *> T)
{
    NameofClass=string("Fun_Type");
    args.assign(T.begin(),T.end());
    basictype=R;
    value=R.getvalue()+"(";
    for(auto &ptr:T){
        value+=ptr->getvalue()+",";
    }
    value.pop_back();
    value+=")";
}




