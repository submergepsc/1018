#pragma once
#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <unordered_map>
using namespace std;
typedef pair<char,int> PII;
class AlgorithmExpression{
    public:
    unordered_map<char,int> Flag;//符号优先级映射表
       stack<PII> o_sta;//符号栈
       stack<int>  n_sta;//数值栈
       AlgorithmExpression(){
           Flag['+']=0;Flag['-']=0;Flag['*']=1;Flag['/']=1;Flag['^']=2;
       };
         ~AlgorithmExpression(){};
         int calculate_expression(string s);//计算字符串的值

};
