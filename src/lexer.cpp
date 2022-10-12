#include"../include/lexer.h"
#include<iostream>

/***宏定义代码段 
 * tips:较长宏定义代码块一定要用大括号装起来不然会出现加法被算两次的怪圈***/
#define NEXT_CHAR {SourceCodeStream->get(pointer);currentColum++;if(pointer == '\n'){ currentRow++; currentColum = 0;}}

#define GO_TO_STATE_(sta) {state=sta;NEXT_CHAR;continue;} 

#define EXCEPTION_OR_RETURN(type,name) {\
                    if(isCharacter(pointer) || isDigit(pointer))throw("lexcial_error_in["\
                    +to_string(currentRow)\
                    +","+to_string(currentColum)+"]"\
                    +"::标识符不能以关键字为前缀");\
                    if(!tokTable.find(name)&&type == "标识符")tokTable.add(Token(name,type,""));\
                    return pair<string,string>(type,name);}

#define _RETURN_(type,name) {\
    if(!tokTable.find(name))tokTable.add(Token(name,type,""));\
    return pair<string,string>(type,name);} 




/*-----------------标识符---------------*/
Token::Token(string n,string t,string value){
    name = n;
    type = "";
    value = "";
}
string Token::toString(){
    return "("+(name==""?"NULL":name)+","+(type==""?"NULL":type)+","+(value==""?"NULL":value)+")";
}
TokenTable::TokenTable(){}
void TokenTable::add(const Token &token) {
    tokens.emplace_back(token);
}

void TokenTable::show() {
    cout<<"----------------标识符表-------------------"<<endl;
    for (auto &t:tokens)
    {
        cout<<t.toString()<<endl;
    }
    
}
bool TokenTable::find(const string &s)const{
    for (auto &t:tokens)
        if(t.getName() == s)
            return true;
    return false;
}

/*---------------------------词法分析器----------------------*/
Lexer::Lexer(ifstream &file){
    currentRow = 1;currentColum = 1; pointer = '$';
    SourceCodeStream = &file;    
}

bool Lexer::isDigit(char &ch){return ch >= '0' && ch <= '9';}
bool Lexer::isCharacter(char &ch){return ch >= 'a' && ch <= 'z';}
int Lexer::getCurrentColum()const{return currentColum;}
int Lexer::getCurrentRow()const{return currentRow;}
TokenTable Lexer::getTokenTable()const{return tokTable;}
pair<string,string> Lexer::getNextToken(){
    int state = 0;
    string StringBuilder;
    /*先判断读头中是否有字符,如果没有先读一个作为起始
        这是为了避免错过从上一次分析最后读到的字符
    */
    if(pointer == '$') NEXT_CHAR;
    //跳过空格
    while(pointer == ' '||pointer == '\n'){
        NEXT_CHAR;
    }
    

    while (pointer != '#')
    {       
            if(isCharacter(pointer)||isDigit(pointer)||pointer == '"')StringBuilder.push_back(pointer);
            //初始状态
            if(state == 0){

                if(isCharacter(pointer)){
                    if(pointer == 's')GO_TO_STATE_(111)
                    if(pointer == 'i')GO_TO_STATE_(331)
                    if(pointer == 'e')GO_TO_STATE_(441)
                    if(pointer == 'd')GO_TO_STATE_(661)
                    if(pointer == 'w')GO_TO_STATE_(771)
                    GO_TO_STATE_(8)
                }
                if(isDigit(pointer))GO_TO_STATE_(20)
             
                if(pointer == ',')GO_TO_STATE_(23)
                if(pointer == '+')GO_TO_STATE_(9)
                if(pointer == '*')GO_TO_STATE_(10)
                if(pointer == '<')GO_TO_STATE_(11)
                if(pointer == '>')GO_TO_STATE_(14)
                if(pointer == '=')GO_TO_STATE_(16)
                if(pointer == '(')GO_TO_STATE_(18)
                if(pointer == ')')GO_TO_STATE_(19)
                if(pointer == ';')GO_TO_STATE_(22)
                if(pointer == '"')GO_TO_STATE_(211)
            }
            //string终态出口
            if(state == 1){EXCEPTION_OR_RETURN("关键字string","string")}
            //start终态出口
            if(state == 2){EXCEPTION_OR_RETURN("关键字start","start")}
            //if终态出口    
            if(state == 3){EXCEPTION_OR_RETURN("关键字if","if")}
            //else终态出口
            if(state == 4){EXCEPTION_OR_RETURN("关键字else","else")}
            //end终态出口
            if(state == 5){EXCEPTION_OR_RETURN("关键字end","end")}
            //do终态出口
            if(state == 6){EXCEPTION_OR_RETURN("关键字do","do")}
            //while终态出口
            if(state == 7){EXCEPTION_OR_RETURN("关键字while","while")}
            //标识符出口
            if(state == 8){
                if(isDigit(pointer)||isCharacter(pointer))GO_TO_STATE_(8)
                _RETURN_("标识符",StringBuilder)
            }
            //数字
            if(state == 20){
                if(isDigit(pointer))GO_TO_STATE_(20)
                _RETURN_("数字",StringBuilder)
            }
            //分隔符逗号
            if(state == 23){_RETURN_("逗号",",")}
            //分隔符;
            if(state == 22){_RETURN_("分号",";")}
            //运算符+
            if(state == 9){_RETURN_("连接运算符","+")}
            //运算符*
            if(state == 10){_RETURN_("重复运算符","*")}
            //关系运算符<
            if(state == 11){
                if(pointer == '=')GO_TO_STATE_(12)
                if(pointer == '>')GO_TO_STATE_(13)
                _RETURN_("关系运算符","<")
                }
            //关系运算符>
            if(state == 14){
                if(pointer == '=')GO_TO_STATE_(15)
                _RETURN_("关系运算符",">")
            }
            //赋值运算符=
            if(state == 16){
                if(pointer == '=')GO_TO_STATE_(17)
                _RETURN_("赋值运算符","=")
            }
            //关系运算符==
            if(state == 17){_RETURN_("关系运算符","==")}
            //关系运算符<>
            if(state == 13){_RETURN_("关系运算符","<>")}
            //关系运算符<=
            if(state == 12){_RETURN_("关系运算符","<=")}
            //关系运算符>=
            if(state == 15){_RETURN_("关系运算符",">=")}
            //左括号
            if(state == 18){_RETURN_("左括号","(")}
            //右括号
            if(state == 19){_RETURN_("右括号",")")}
            //字符串出口
            if(state == 21){_RETURN_("字符串",StringBuilder)}
            if(state == 111){
                if(pointer == 't')GO_TO_STATE_(112)
                GO_TO_STATE_(8)
            }
            if(state == 112){
                if(pointer == 'r')GO_TO_STATE_(113)
                if(pointer == 'a')GO_TO_STATE_(223)
                GO_TO_STATE_(8)
            }
            if(state == 113){
                if(pointer == 'i')GO_TO_STATE_(114)
                GO_TO_STATE_(8)
            }
            if(state == 114){
                if(pointer == 'n')GO_TO_STATE_(115)
                GO_TO_STATE_(8)
            }

            if(state == 115){
                if(pointer == 'g')GO_TO_STATE_(1)
                GO_TO_STATE_(8)
            }
            if(state == 211){
                if(isCharacter(pointer)||isDigit(pointer))GO_TO_STATE_(211);
                if(pointer == ' '){StringBuilder.push_back(' ');GO_TO_STATE_(211)} //空格保留
                if(pointer == ';')throw("lexcial_error_in["
                    +to_string(currentRow)
                    +","+to_string(currentColum/2)+"]"
                    +"::字符串缺失右引号");
                if(pointer == '"')GO_TO_STATE_(21)
            }
            if(state == 223){
                if(pointer == 'r')GO_TO_STATE_(224)
                GO_TO_STATE_(8)
            }
            
            if(state == 224){
                if(pointer == 't')GO_TO_STATE_(2)
                GO_TO_STATE_(8)
            }
        
            if(state == 331){
                if(pointer == 'f')GO_TO_STATE_(3)
                GO_TO_STATE_(8)
            }
            if(state == 441){
                if(pointer == 'l')GO_TO_STATE_(442)
                if(pointer == 'n')GO_TO_STATE_(552)
                GO_TO_STATE_(8)
            }
            if(state == 442){
                if(pointer == 's')GO_TO_STATE_(443)
                GO_TO_STATE_(8)
            }

            if(state == 443){
                if(pointer == 'e')GO_TO_STATE_(4)
                GO_TO_STATE_(8)
            }

            if(state == 552){
                if(pointer == 'd')GO_TO_STATE_(5)
                GO_TO_STATE_(8)
            }

            if(state == 661){
                if(pointer == 'o')GO_TO_STATE_(6)
                GO_TO_STATE_(8)
            }
            if(state == 771){
                if(pointer == 'h')GO_TO_STATE_(772)
                GO_TO_STATE_(8)
            }
            if(state == 772){
                if(pointer == 'i')GO_TO_STATE_(773)
                GO_TO_STATE_(8)
            }
            if(state == 773){
                if(pointer == 'l')GO_TO_STATE_(774)
                GO_TO_STATE_(8)
            }

            if(state == 774){
                if(pointer == 'e')GO_TO_STATE_(7)
                GO_TO_STATE_(8)
            }


            
    }
             return pair<string,string>("#","#");   

}