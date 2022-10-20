#include"../include/semlyzer.h"

Semlyzer::Semlyzer(ifstream &file)
    : Parser(file)
{
    NXQ = 0;
}

void Semlyzer::showMidCode() {
    for (int i = 0; i < datas.size(); i++)
    {
        cout << "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            << endl;
    }
    
}

string Semlyzer::getRow() {
    return to_string(lexer.getCurrentRow());
}

string Semlyzer::getColum() {
    return to_string(lexer.getCurrentColum());
}

string Semlyzer::SyntaxErrorGener(const string &type) {
    if(type == "δ����")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"δ������ʶ��\""+input.second+"\"";
    else if(type == "�ظ�����")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"�ظ�������ʶ��\""+input.second+"\"";
    else if(type == "������Χ")
        return string("over_range:")+"["+getRow()+","+getColum()+"]"+"��Ч�������������鷶Χ!";
    return "~undefined_error~";
}

void Semlyzer::generate(const string &what,const string &leftvalue,const string &rightvalue,const string &target) {
    datas.push_back(metadata(what,leftvalue,rightvalue,target));
    NXQ ++;
}

Token Semlyzer::getTempVar() {
    int number = tempvar.size();
    tempvar.push_back(Token(string("T")+to_string(number),"",""));
    return tempvar[number];
}

void Semlyzer::BackPath(const int &offset,const int &target) {
    datas[offset].target = to_string(target);
}

void Semlyzer::Go() {
    parseP();
}

void Semlyzer::parseP() {
    parseA();
    Match("�ֺ�");
    parseB();
}

void Semlyzer::parseA() {
        Match("�ؼ���string");
        parseL("string");
}

void Semlyzer::parseL(const string &type) {
    if(input.first == "��ʶ��"){
        int idx = lexer.tokTable.find(input.second);
        Match("��ʶ��");
        lexer.tokTable.updateContent(idx,type);
        Token H  = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }
}

void Semlyzer::parseLp(const string &type) {
    if(input.first == "����"){
        Match("����");
        int idx = lexer.tokTable.find(input.second);
        Match("��ʶ��");
        lexer.tokTable.updateContent(idx,type);
        Token H = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }else{}
}

void Semlyzer::parseB() {
    parseS();
    Match("�ֺ�");
    parseBp();
}

void Semlyzer::parseBp() {
    if(input.first == "��ʶ��"||input.first == "�ؼ���if"||input.first == "�ؼ���do"){
        parseS();
        Match("�ֺ�");
        parseBp();
    }
}

void Semlyzer::parseS() {
    if(input.first == "��ʶ��")
        parseE();
    else if(input.first == "�ؼ���if")
        parseF();
    else if(input.first == "�ؼ���do")
        parseR();
}

void Semlyzer::parseE() {
    int idx = lexer.tokTable.find(input.second);
    string name = input.second;
    if(idx == -1)throw(SyntaxErrorGener("δ����"));
    Match("��ʶ��");
    Token H = parseH(lexer.tokTable.getType(idx));
    if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
        throw(SyntaxErrorGener("������Χ"));
    Match("��ֵ�����");
    Token X = parseX();
    if(H.getType() != "array")
        generate("=",X.getName(),"null",name);
    else
        generate("=",X.getName(),"null",name+"["+H.getOffset()+"]");
}

void Semlyzer::parseF() {
    Match("�ؼ���if");
    Match("������");
    Token Q = parseQ();
    Match("������");
    generate("jnz",Q.getName(),"null",to_string(NXQ+2));
    int Exit1 = NXQ;
    generate("j","null","null","");
    parseG();
    int Exit2 = NXQ;
    generate("j","null","null","");
    BackPath(Exit1,NXQ);
    Match("�ؼ���else");
    parseG();
    BackPath(Exit2,NXQ);
}

void Semlyzer::parseR() {
    Match("�ؼ���do");
    int position = NXQ;
    parseG();
    Match("�ؼ���while");
    Match("������");
    Token Q = parseQ();
    Match("������");
    generate("jnz",Q.getName(),"null",to_string(position));
}

Token Semlyzer::parseX() {
    Token T =parseT();
    return parseXp(T);
}

Token Semlyzer::parseXp(const Token &T) {
    if(input.first == "���������"){
        Match("���������");
        Token T1 = parseT();
        Token Xp1 = getTempVar();
        Xp1.setType(T1.getType());
        generate("+",T.getName(),T1.getName(),Xp1.getName());
        return parseXp(Xp1);
    }
    return T;
}

Token Semlyzer::parseT() {
    Token D = parseD();
    return parseTp(D);
}
Token Semlyzer::parseTp(const Token &D) {
    if(input.first == "�ظ������"){
        Match("�ظ������");
        string name = input.second;
        Match("����");
        Token Tp = getTempVar();
        Tp.setType(D.getType());
        generate("*",D.getName(),name,Tp.getName());
        return parseTp(Tp);
    }
    return D;
}

Token Semlyzer::parseD() {
    if(input.first == "�ַ���"){
        string s = input.second;
        Match("�ַ���");
        return Token(s,"string",s);
    }
    if(input.first == "��ʶ��"){
        int idx = lexer.tokTable.find(input.second);
        string name = input.second;
        Match("��ʶ��");
        if(idx == -1)throw(SyntaxErrorGener("δ����"));
        Token H = parseH(lexer.tokTable.getType(idx));
        if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
            throw(SyntaxErrorGener("������Χ"));
        if(H.getType() == "array")
            H.setName(name+"["+H.getOffset()+"]");
        else H.setName(name);
        return H;
    }
    if(input.first == "������"){
        Match("������");
        Token X = parseX();
        Match("������");
        return X;
    }
    return Token("error_in_D()","","");
}

Token Semlyzer::parseQ() {
    Token X1 = parseX();
    string ope = input.second;
    Match("��ϵ�����");
    Token X2 = parseX();
    Token Q = getTempVar();
    Q.setType("bool");
    generate(ope,X1.getName(),X2.getName(),Q.getName());
    return Q;
}

void Semlyzer::parseG() {
    if(input.first == "�ؼ���start")
        parseW();
    if(input.first == "�ؼ���if"||input.first == "�ؼ���do"||input.first == "��ʶ��")
        parseS();
}

void Semlyzer::parseW() {
    Match("�ؼ���start");
    parseB();
    Match("�ؼ���end");
}

Token Semlyzer::parseH(const string &type){
    Token H("",type,"");
    if(input.first == "��������"){
        Match("��������");
        H.setSize(input.second);
        Match("����");
        Match("��������");
        H.setType("array");
        return H;
    }
    return H;
}
