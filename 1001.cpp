#include <iostream>
#include <string>
using namespace std;

const unsigned BASE_LENGTH=6;
const unsigned MIN_EXPONENT=1;
const unsigned MAX_EXPONENT=25;
const float MAX_BASE=99.998;
const int MAX_BASE_INTEGER_LENGTH=2;

string strMultiply(const string& lhs,const string& rhs){
    //this function no judge lhs's and rhs's validity,the work would be done by the calling
    string result,result2;
    int l_size=lhs.length();
    int r_size=rhs.length();
    if(l_size<1 || r_size<1) {
        result="erro";
        return result;
     }
    unsigned add_pos_start=0;
    for(int i=r_size-1;i>=0;i--){
        unsigned r=rhs.at(i)-'0';
        unsigned carry_bit=0;
        string one_mult;
        for(int j=l_size-1;j>=0;j--){
            unsigned tmp;
            tmp=r*(lhs.at(j)-'0')+carry_bit;
            one_mult.append(1,char('0'+tmp%10));
            carry_bit=int(tmp/10);
        }
        if(carry_bit>0)
            one_mult.append(1,char('0'+carry_bit));
        if(add_pos_start==0)
            result.append(one_mult);
        else{
            carry_bit=0;
            for(int i=0;i<one_mult.length();i++){
                unsigned tmp;
                if(add_pos_start+i<result.length()){
                    tmp=int((result.at(add_pos_start+i)-'0')+(one_mult.at(i)-'0'))+carry_bit;
                    result.at(add_pos_start+i)=char('0'+tmp%10);
                    carry_bit=int(tmp/10);
                }else{
                    tmp=int(one_mult.at(i)-'0')+carry_bit;
                    result.append(1,char('0'+tmp%10));
                    carry_bit=int(tmp/10);
                }
            }
            if(carry_bit>0)
                result.append(1,char('0'+carry_bit));
        }
        add_pos_start++;
    }
    for(string::reverse_iterator rit=result.rbegin(); rit!=result.rend(); ++rit)//reverse result for correct display
        result2.append(1,*rit);
    return result2;
}

string strPow(const string& base,const unsigned expon){
 //this function no judge base's,exponent's validity,the work would be done by the calling
    if(expon==0)
        return string(1,'1');
	if(base=="0") //but expon is not 0
		return string(1,'0'); 
    if(expon==1)
        return base;
    string tmp,tmp2;
    tmp=strPow(base,int(expon/2));
    tmp2=strMultiply(tmp,tmp);
    if(expon%2==1){
        tmp=strMultiply(tmp2,base);
        return tmp;
    }else
        return tmp2;
}

string realStrPow(const string& base,const unsigned expon,const unsigned decimal_length){
//this function no judge base's,exponent's,decimal_length's validity,the work would be done by the calling
    string result=strPow(base,expon);
    if(result.length()<decimal_length){//insert '.' at correct postion
        result=string(1,'.')+string(decimal_length-result.length(),'0')+result;}
    else if(decimal_length>0)
            result.insert(result.length()-decimal_length,1,'.');
    return result;
}

int main(){
    string s,result;
    int n;    
    while (cin>>s>>n) {
        bool isvalid=true;
        int dotpos=-1,valid_start=-1,valid_end=-1;
        string digit_str;
        int s_size=s.length();
        //begin:judge input validity
        if(n<MIN_EXPONENT|| n>MAX_EXPONENT){
            cout<<"input exponent shall be between "<<MIN_EXPONENT<<"and "<<MAX_EXPONENT<<endl;
            break;
        }
        if(s_size!=BASE_LENGTH){
            cout<<"input base length shall be "<<BASE_LENGTH<<endl;
            break;
        }
        for(int i=0;i<s_size;i++){
            char c=s.at(i);
            if(c!='.'){
                if(c-'0'<0 ||c-'0'>9){
                    isvalid=false;
                    cout<<"input base invalid"<<endl;
                    break;
                }else if(c!='0') {//if c is '1'-'9',getting the first and the last non-0 digit's positions
                    if(valid_start==-1)
                        valid_start=i;
                    if(valid_start!=-1)
                        valid_end=i;
                }
            }else{
                //if(i==0 || i==s_size-1 ||dotpos!=-1){
                if(dotpos!=-1){//input '.' more than one,exit
                    isvalid=false;
                    cout<<"input base invalid"<<endl;
                    break;
                }
                dotpos=i; //getting dot position in s
                valid_end=i-1;
            }
        }
        if(!isvalid) break;//input invalid,exit    
        if(valid_start<0){
        	cout<<"base cannot be 0 or 0.0, and so on"<<endl;
        	break;            
        }
        if(s.find('.')==string::npos){//if s not including '.'
        	if(s_size-valid_start>MAX_BASE_INTEGER_LENGTH && valid_start>0){
	            cout<<"base"<<s<<" larger than "<<MAX_BASE<<endl;
	            break;
        	}
        	valid_end=s_size-1;
        }
        if(s.compare("99.999")==0 ||(valid_start>=0 && dotpos-valid_start>MAX_BASE_INTEGER_LENGTH)){//input invalid,exit
            cout<<"base"<<s<<" larger than "<<MAX_BASE<<endl;
            break;
        }
        //end:judge input validity
        string result;
        unsigned decimal_length=0;
        if(valid_start<0){//if input base is 0 or 0.0
        	digit_str="0";
        	decimal_length=0;
        }else{
	        if(dotpos<valid_start || dotpos>valid_end)//getting valid digit sequence not including '.',leading or trailing 0
	            digit_str=s.substr(valid_start,valid_end-valid_start+1);
	        else
	            digit_str=s.substr(valid_start,dotpos-valid_start)+s.substr(dotpos+1,valid_end-dotpos);
	        if(dotpos>=0 && dotpos<valid_end)//calculating decimal length for inset '.' in result
	            decimal_length=n*(valid_end-dotpos);
	    }
        result=realStrPow(digit_str,n,decimal_length);//calling realStrPow function to claculate pow(s,n)
        cout<<result<<endl;
    }
    return 0;
}
