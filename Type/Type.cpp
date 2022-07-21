#include "Type.hpp"
bool Type::is_int() {
    if(is_BasicType()) return value.compare("int")==0?true:false;
    else if(is_Array_Type()) return ((static_cast<Array_Type *>(this))->basictype.value.compare("int")==0)?true:false;
    else if(is_Fun_Type()) return ((static_cast<Fun_Type *>(this))->basictype.value.compare("int")==0)?true:false;
    else return false;
};
bool Type::is_float() {
    if(is_BasicType()) return value.compare("float")==0?true:false;
    else if(is_Array_Type()) return ((static_cast<Array_Type *>(this))->basictype.value.compare("float")==0)?true:false;
    else if(is_Fun_Type()) return ((static_cast<Fun_Type *>(this))->basictype.value.compare("float")==0)?true:false;
    else return false;
};
Product_Type::Product_Type(Type* T1,Type* T2)
{
    NameofClass=string("Product_Type");
    if(T1->is_Product_Type()){
        auto& t1=static_cast<Product_Type*>(T1)->eles;
        eles.insert(eles.end(),t1.begin(),t1.end());
        value="<"+T1->getvalue().substr(1,T1->getvalue().size()-2);
    }
    else{
        eles.push_back(T1);
        value="<"+T1->getvalue();
    }
    if(T2->is_Product_Type()){
        auto& t2=static_cast<Product_Type*>(T2)->eles;
        eles.insert(eles.end(),t2.begin(),t2.end());
        value=","+T2->getvalue().substr(1,T2->getvalue().size()-2);
    }
    else{
        eles.push_back(T2);
        value+=","+T2->getvalue();
    }
    value+=">";
}

Product_Type::Product_Type(Type* T1,Type* T2,int n)
{
    NameofClass=string("Product_Type");
    if(T1->is_Product_Type()){
        auto& t1=static_cast<Product_Type*>(T1)->eles;
        eles.insert(eles.end(),t1.begin(),t1.end());
        value="<"+T1->getvalue().substr(1,T1->getvalue().size()-2);
    }
    else{
        eles.push_back(T1);
        value="<"+T1->getvalue();
    }
    while(--n>=0){
        if(T2->is_Product_Type()){
            auto& t2=static_cast<Product_Type*>(T2)->eles;
            eles.insert(eles.end(),t2.begin(),t2.end());
            value=","+T2->getvalue().substr(1,T2->getvalue().size()-2);
        }
        else{
            eles.push_back(T2);
            value+=","+T2->getvalue();
        }
    }
    value+=">";
    
}

bool Product_Type::sameas(vector<Type *> &t)
{
    if(t.size()!=eles.size()) return false;
    for(int i=0;i<eles.size();i++){
        if(eles[i]->getvalue().compare(t[i]->getvalue())!=0)
            return false;
    }
    return true;
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

void Array_Type::setBasicType(BasicType& t)
{
    value.replace(0,basictype.getvalue().size(),t.getvalue());
    basictype=t;
}

void Array_Type::set_elements_nums(int index,int value){
    elements_nums[index]=value;
    this->value=basictype.getvalue();
    for(int i=0;i<elements_nums.size();i++){
        this->value=this->value+"["+to_string(elements_nums[i])+"]";
    }
}

int Array_Type::bytes_occupied()
{
    int res=4;
    for(int i=0;i<elements_nums.size();i++){
        res*=elements_nums[i];
    }
    return res;
}

Type* Array_Type::Lower_one_level()
{
    if(lev==1) return new BasicType(basictype);
    else{
        auto ptr=new Array_Type();
        ptr->basictype=basictype;
        ptr->elements_nums=elements_nums;
        ptr->elements_nums.pop_back();
        ptr->lev=lev-1;
        ptr->value=basictype.getvalue();
        for(int i=0;i<ptr->lev;i++)
            ptr->value+="["+to_string(ptr->elements_nums[i])+"]";
        return ptr;
    }
}

Type* Array_Type::Lower_one_level_forward()
{
    if(lev==1) return new BasicType(basictype);
    else{
        auto ptr=new Array_Type();
        ptr->basictype=basictype;
        ptr->elements_nums=elements_nums;
        ptr->elements_nums.erase(ptr->elements_nums.begin());
        ptr->lev=lev-1;
        ptr->value=basictype.getvalue();
        for(int i=0;i<ptr->lev;i++)
            ptr->value+="["+to_string(ptr->elements_nums[i])+"]";
        return ptr;
    }
}

Fun_Type::Fun_Type(BasicType& R,vector<Type *> T)
{
    NameofClass=string("Fun_Type");
    args.assign(T.begin(),T.end());
    basictype=R;
    value=R.getvalue()+"(";
    for(auto &ptr:T){
        num_args++;
        value+=ptr->getvalue()+",";
    }
    if(value[value.size()-1]==',') value.pop_back();
    value+=")";
}

void Fun_Type::updatevalue()
{
    value=basictype.getvalue()+"(";
    for(auto &ptr:args){
        num_args++;
        value+=ptr->getvalue()+",";
    }
    if(value[value.size()-1]==',') value.pop_back();
    value+=")";
}



