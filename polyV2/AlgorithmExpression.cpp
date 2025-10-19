#include "AlgorithmExpression.h"
int AlgorithmExpression::calculate_expression(string s){
    n_sta.push(0);//防止空栈错误，也防止出现负数的情况
    int flag=0;//flag表示增加的优先级
    for(int i=0;s[i];i++){

       if(s[i]>='0'&&s[i]<='9'){
        int g=1;
        if(i>1&&s[i-1]=='-'&&!(s[i-2]>='0'&&s[i-2]<='9')){//处理负数的情况
           g=-1;
        }
        int num=s[i]-'0';
        while(s[i+1]>='0'&&s[i+1]<='9'){//当碰到数字时候，持续读入需要的数字串
            num=num*10+s[i+1]-'0';
            i++;
        }
        n_sta.push(num*g);//将数字压入数值栈
       }
       else if(s[i]=='('){
        flag+=100;//括号里面的内容优先级更高，增加100的优先级
        o_sta.push(PII{s[i],flag+3});
       }
       else if(s[i]==')'){
        flag-=100;//遇到右括号时候，优先级降低，并且将括号内的内容计算完毕
        while(!o_sta.empty()&&o_sta.top().first!='('){
            char ch=o_sta.top().first;
            o_sta.pop();
            int b=n_sta.top();n_sta.pop();
            int a=n_sta.top();n_sta.pop();
            int res=0;
            if(ch=='+') res=a+b;
            else if(ch=='-') res=a-b;
            else if(ch=='*') res=a*b;
            else if(ch=='/') res=a/b;//此处要防止b为0的情况
            else if(ch=='^') res=pow(a,b);
            n_sta.push(res);
        }
       }
       else if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/'||s[i]=='^'){
       if(s[i]=='+'||s[i]=='-'){
        while(!o_sta.empty()&&o_sta.top().second>=flag+0){
            char ch=o_sta.top().first;
            o_sta.pop();
            int b=n_sta.top();n_sta.pop();
            int a=n_sta.top();n_sta.pop();
            int res=0;//由于此时为加减法，所以优先级最低
            if(ch=='+') res=a+b;
            else if(ch=='-') res=a-b;
            else if(ch=='*') res=a*b;
            else if(ch=='/') res=a/b;
            else if(ch=='^') res=pow(a,b);
            n_sta.push(res);
        }
        o_sta.push(PII{s[i],flag+0});
       }
       else if(s[i]=='*'||s[i]=='/'){
        while(!o_sta.empty()&&o_sta.top().second>=flag+1){
            char ch=o_sta.top().first;
            o_sta.pop();
            int b=n_sta.top();n_sta.pop();
            int a=n_sta.top();n_sta.pop();
            int res=0;//由于此时为乘除法，所以优先级次之
            if(ch=='*') res=a*b;
            else if(ch=='/') res=a/b;
            else if(ch=='^') res=pow(a,b);
            n_sta.push(res);
        }
         o_sta.push(PII{s[i],flag+1});
       }
       else if(s[i]=='^'){
        while(!o_sta.empty()&&o_sta.top().second>=flag+2){
            char ch=o_sta.top().first;
            o_sta.pop();
            int b=n_sta.top();n_sta.pop();
            int a=n_sta.top();n_sta.pop();
            int res=0;//由于此时为指数，所以优先级次之
            if(ch=='^') res=pow(a,b);
            n_sta.push(res);
        }
         o_sta.push(PII{s[i],flag+2});
       }
    }
}
     while(!o_sta.empty()){//将剩余的符号全部计算完毕
          char ch=o_sta.top().first;
          o_sta.pop();
          int b=n_sta.top();n_sta.pop();
          int a=n_sta.top();n_sta.pop();
          int res=0;
          if(ch=='+') res=a+b;
          else if(ch=='-') res=a-b;
          else if(ch=='*') res=a*b;
          else if(ch=='/') res=a/b;
          else if(ch=='^') res=pow(a,b);
          n_sta.push(res);
     }
     return n_sta.top();//最终数值栈中剩下的数值即为表达式的值
}
   
