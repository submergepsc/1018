#include "polynomial.h"
Polynomial::Polynomial(string s){  
    //对于字符串的输入，我们定义系数与指数之间用^连接，项与项之间用+连接，例如3^0+2^1+1^2表示多项式3+2x+1x^2
    int val=0,exp=0,flag=1;//val表示系数，exp表示指数，flag表示当前读取的是指数还是系数，指数为0，正系数为1，负系数为-1
    for(int i=0;s[i];i++){
        if(s[i]=='+')//遇到+号，说明前一项读取完毕
        {
           // cout<<val<<" ";
            List[exp]=val;
            val=0;exp=0;flag=1;//清零val和exp，准备读取下一项
            n++;
        }
        else if(s[i]=='-')//遇到-号，说明接下来读取的是负系数
        {
            List[exp]=val;
            flag=-1;
            val=0;exp=0;//清零val和exp，准备读取下一项
            n++;
        }   
        else if(s[i]=='^')//遇到^号，说明接下来读取的是指数
        {
            val=flag*val;//将读取到的系数存入val中
            //cout<<val<<" ";
            flag=0;
        }
        else//读取数字
        {
            if(flag==0)//读取指数
            {

                exp=exp*10+(s[i]-'0');
            }
            else//读取系数
            {
                val=val*10+(s[i]-'0');
            }
        }
    }
     //由于最后一项后面没有+或-号，因此需要在循环结束后将最后一项加入多项式中
        List[exp]=val;
        size=exp;
        if(s[0]!='+' && s[0]!='-')n++;//如果多项式的第一项不是正负号开头，则项数加1
        return ;
}
Polynomial::~Polynomial() {
    // 如果有动态分配资源，在这里释放
    return ;
}   
Polynomial::Polynomial() {
    //默认构造函数
    return;
}
Polynomial::Polynomial(const Polynomial &p){
    for(int i=0;i<=p.size;i++){
        List[i]=p.List[i];
    }
    size=p.size;
    n=p.n;
}
int Polynomial::calculate_val(int x){
    if(x==0)return List[0];
    int ans=0;
    for(int i=0;i<size;i++){
        if(List[i]!=0)ans+=List[i]*pow(x,i);
    }
    return ans;
}
void Polynomial::output(){
    //按照n,val1,exp1,val2,exp2,...的格式输出多项式:先输出多项式的项数n，之后依次输入每一项的系数与指数
    //按照降序输出
    if(n==0){
        cout<<"0,0,"<<endl;
        return ;
    }
    cout<<n<<",";
    for(int i=size;i>=0;i--){
        if(List[i]==0)continue;
        cout<<List[i]<<","<<i<<",";
    }
    cout<<endl;
    return ;
}
Polynomial SumOfPolynomial(Polynomial p1,Polynomial p2){
    Polynomial ans;
    int size=max(p1.size,p2.size),k=0;
    for(int i=0;i<=size;i++){
        if(p1.List[i]||p2.List[i]){
            k=p1.List[i]+p2.List[i];//k表示当前指数下的系数和
            if(k!=0){//如果系数和不为0，则将该项加入结果多项式中,但是结果为0，则表示该项被抵消，不加入结果多项式
                ans.List[i]=k;
                ans.size=i;
                ans.n++;
            }
        }
    }
    return ans;
}
Polynomial SubOfPolynomial(Polynomial p1,Polynomial p2){
    Polynomial ans;

    int size=max(p1.size,p2.size);
    int k=0;
    for(int i=0;i<=size;i++){
        if(p1.List[i]||p2.List[i]){
            k=p1.List[i]-p2.List[i];//k表示当前指数下的系数差
            if(k!=0){//如果系数和不为0，则将该项加入结果多项式中,但是结果为0，则表示该项被抵消，不加入结果多项式
                ans.List[i]=k;
                ans.n++;
                ans.size=i;
            }
        }
    }
    return ans;
}
Polynomial MulOfPolynomial(Polynomial p1,Polynomial p2){
    Polynomial ans;
    int exp,val;
    int size=p1.size+p2.size;
    ans.size=size;
    for(int i=0;i<=p1.size;i++){
        for(int j=0;j<=p2.size;j++){
            if(!p1.List[i] || !p2.List[j]) continue; // 如果系数为0，则跳过
            exp=i+j;
            val=p1.List[i]*p2.List[j];
            ans.List[exp]+=val;
        }
    }
    for(int i=0;i<=ans.size;i++){//统计项数
        if(ans.List[i]!=0)ans.n++;
    }
    return ans;
}
Polynomial DerOfPolynomial(Polynomial p){
    Polynomial ans;
   if(p.size==0){//常数项求导为0
        ans.size=0;
        ans.n=0;
        return ans;
   }
   ans.size=p.size-1;
   if(ans.List[0]!=0)ans.n=p.n-1;
    else ans.n=p.n;
    for(int i=1;i<=p.size;i++){
        ans.List[i-1]=p.List[i]*i;
    }
    return ans;
}
