#pragma once   
#include <iostream>
#include <string>
#include <cmath>
using namespace std; 
class Polynomial{
    public:
        int List[10000]={0};//下标表示指数，元素的值表示系数
        int size=0;//多项式的最高次数
        int n=0;//多项式的项数
        Polynomial(string s);//采用字符串构造多项式
        Polynomial();//默认构造函数
        Polynomial(const Polynomial &p);//拷贝构造函数
        ~Polynomial();//销毁线性表
        int calculate_val(int x);//计算多项式在x处的值
        void output();//输出多项式
};
 Polynomial SumOfPolynomial(Polynomial p1,Polynomial p2);//两个多项式相加
 Polynomial SubOfPolynomial(Polynomial p1,Polynomial p2);//两个多项式相减
 Polynomial  MulOfPolynomial(Polynomial p1,Polynomial p2);//两个多项式相乘
 Polynomial DerOfPolynomial(Polynomial p);//多项式求导
