#include"../include/lexer.h"
#include<iostream>

/***�궨������ 
 * tips:�ϳ��궨������һ��Ҫ�ô�����װ������Ȼ����ּӷ��������εĹ�Ȧ***/
#define NEXT_CHAR {SourceCodeStream->get(pointer);currentColum++;if(pointer == '\n'){ currentRow++; currentColum = 0;}}

#define GO_TO_STATE_(sta) {state=sta;NEXT_CHAR;continue;} 

#define EXCEPTION_OR_RETURN(type,name) {\
                    if(isCharacter(pointer) || isDigit(pointer))throw("lexcial_error_in["\
                    +to_string(currentRow)\
                    +","+to_string(currentColum)+"]"\
                    +"::��ʶ�������Թؼ���Ϊǰ׺");\
                    if(!tokTable.find(name)&&type == "��ʶ��")tokTable.add(Token(name,type,""));\
                    return pair<string,string>(type,name);}

#define _RETURN_(type,name) {\
    if(!tokTable.find(name))tokTable.add(Token(name,type,""));\
    return pair<string,string>(type,name);} 




/*-----------------��ʶ��---------------*/
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
    cout<<"----------------��ʶ����-------------------"<<endl;
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

/*---------------------------�ʷ�������----------------------*/
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
    /*���ж϶�ͷ���Ƿ����ַ�,���û���ȶ�һ����Ϊ��ʼ
        ����Ϊ�˱���������һ�η������������ַ�
    */
    if(pointer == '$') NEXT_CHAR;
    //�����ո�
    while(pointer == ' '||pointer == '\n'){
        NEXT_CHAR;
    }
    

    while (pointer != '#')
    {       
            if(isCharacter(pointer)||isDigit(pointer)||pointer == '"')StringBuilder.push_back(pointer);
            //��ʼ״̬
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
            //string��̬����
            if(state == 1){EXCEPTION_OR_RETURN("�ؼ���string","string")}
            //start��̬����
            if(state == 2){EXCEPTION_OR_RETURN("�ؼ���start","start")}
            //if��̬����    
            if(state == 3){EXCEPTION_OR_RETURN("�ؼ���if","if")}
            //else��̬����
            if(state == 4){EXCEPTION_OR_RETURN("�ؼ���else","else")}
            //end��̬����
            if(state == 5){EXCEPTION_OR_RETURN("�ؼ���end","end")}
            //do��̬����
            if(state == 6){EXCEPTION_OR_RETURN("�ؼ���do","do")}
            //while��̬����
            if(state == 7){EXCEPTION_OR_RETURN("�ؼ���while","while")}
            //��ʶ������
            if(state == 8){
                if(isDigit(pointer)||isCharacter(pointer))GO_TO_STATE_(8)
                _RETURN_("��ʶ��",StringBuilder)
            }
            //����
            if(state == 20){
                if(isDigit(pointer))GO_TO_STATE_(20)
                _RETURN_("����",StringBuilder)
            }
            //�ָ�������
            if(state == 23){_RETURN_("����",",")}
            //�ָ���;
            if(state == 22){_RETURN_("�ֺ�",";")}
            //�����+
            if(state == 9){_RETURN_("���������","+")}
            //�����*
            if(state == 10){_RETURN_("�ظ������","*")}
            //��ϵ�����<
            if(state == 11){
                if(pointer == '=')GO_TO_STATE_(12)
                if(pointer == '>')GO_TO_STATE_(13)
                _RETURN_("��ϵ�����","<")
                }
            //��ϵ�����>
            if(state == 14){
                if(pointer == '=')GO_TO_STATE_(15)
                _RETURN_("��ϵ�����",">")
            }
            //��ֵ�����=
            if(state == 16){
                if(pointer == '=')GO_TO_STATE_(17)
                _RETURN_("��ֵ�����","=")
            }
            //��ϵ�����==
            if(state == 17){_RETURN_("��ϵ�����","==")}
            //��ϵ�����<>
            if(state == 13){_RETURN_("��ϵ�����","<>")}
            //��ϵ�����<=
            if(state == 12){_RETURN_("��ϵ�����","<=")}
            //��ϵ�����>=
            if(state == 15){_RETURN_("��ϵ�����",">=")}
            //������
            if(state == 18){_RETURN_("������","(")}
            //������
            if(state == 19){_RETURN_("������",")")}
            //�ַ�������
            if(state == 21){_RETURN_("�ַ���",StringBuilder)}
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
                if(pointer == ' '){StringBuilder.push_back(' ');GO_TO_STATE_(211)} //�ո���
                if(pointer == ';')throw("lexcial_error_in["
                    +to_string(currentRow)
                    +","+to_string(currentColum/2)+"]"
                    +"::�ַ���ȱʧ������");
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