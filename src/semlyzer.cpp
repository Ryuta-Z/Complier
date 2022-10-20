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
    if(type == "未声明")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"未声明标识符\""+input.second+"\"";
    else if(type == "重复声明")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"重复声明标识符\""+input.second+"\"";
    else if(type == "超出范围")
        return string("over_range:")+"["+getRow()+","+getColum()+"]"+"无效索引，超出数组范围!";
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
    Match("分号");
    parseB();
}

void Semlyzer::parseA() {
        Match("关键字string");
        parseL("string");
}

void Semlyzer::parseL(const string &type) {
    if(input.first == "标识符"){
        int idx = lexer.tokTable.find(input.second);
        Match("标识符");
        lexer.tokTable.updateContent(idx,type);
        Token H  = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }
}

void Semlyzer::parseLp(const string &type) {
    if(input.first == "逗号"){
        Match("逗号");
        int idx = lexer.tokTable.find(input.second);
        Match("标识符");
        lexer.tokTable.updateContent(idx,type);
        Token H = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }else{}
}

void Semlyzer::parseB() {
    parseS();
    Match("分号");
    parseBp();
}

void Semlyzer::parseBp() {
    if(input.first == "标识符"||input.first == "关键字if"||input.first == "关键字do"){
        parseS();
        Match("分号");
        parseBp();
    }
}

void Semlyzer::parseS() {
    if(input.first == "标识符")
        parseE();
    else if(input.first == "关键字if")
        parseF();
    else if(input.first == "关键字do")
        parseR();
}

void Semlyzer::parseE() {
    int idx = lexer.tokTable.find(input.second);
    string name = input.second;
    if(idx == -1)throw(SyntaxErrorGener("未声明"));
    Match("标识符");
    Token H = parseH(lexer.tokTable.getType(idx));
    if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
        throw(SyntaxErrorGener("超出范围"));
    Match("赋值运算符");
    Token X = parseX();
    if(H.getType() != "array")
        generate("=",X.getName(),"null",name);
    else
        generate("=",X.getName(),"null",name+"["+H.getOffset()+"]");
}

void Semlyzer::parseF() {
    Match("关键字if");
    Match("左括号");
    Token Q = parseQ();
    Match("右括号");
    generate("jnz",Q.getName(),"null",to_string(NXQ+2));
    int Exit1 = NXQ;
    generate("j","null","null","");
    parseG();
    int Exit2 = NXQ;
    generate("j","null","null","");
    BackPath(Exit1,NXQ);
    Match("关键字else");
    parseG();
    BackPath(Exit2,NXQ);
}

void Semlyzer::parseR() {
    Match("关键字do");
    int position = NXQ;
    parseG();
    Match("关键字while");
    Match("左括号");
    Token Q = parseQ();
    Match("右括号");
    generate("jnz",Q.getName(),"null",to_string(position));
}

Token Semlyzer::parseX() {
    Token T =parseT();
    return parseXp(T);
}

Token Semlyzer::parseXp(const Token &T) {
    if(input.first == "连接运算符"){
        Match("连接运算符");
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
    if(input.first == "重复运算符"){
        Match("重复运算符");
        string name = input.second;
        Match("数字");
        Token Tp = getTempVar();
        Tp.setType(D.getType());
        generate("*",D.getName(),name,Tp.getName());
        return parseTp(Tp);
    }
    return D;
}

Token Semlyzer::parseD() {
    if(input.first == "字符串"){
        string s = input.second;
        Match("字符串");
        return Token(s,"string",s);
    }
    if(input.first == "标识符"){
        int idx = lexer.tokTable.find(input.second);
        string name = input.second;
        Match("标识符");
        if(idx == -1)throw(SyntaxErrorGener("未声明"));
        Token H = parseH(lexer.tokTable.getType(idx));
        if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
            throw(SyntaxErrorGener("超出范围"));
        if(H.getType() == "array")
            H.setName(name+"["+H.getOffset()+"]");
        else H.setName(name);
        return H;
    }
    if(input.first == "左括号"){
        Match("左括号");
        Token X = parseX();
        Match("右括号");
        return X;
    }
    return Token("error_in_D()","","");
}

Token Semlyzer::parseQ() {
    Token X1 = parseX();
    string ope = input.second;
    Match("关系运算符");
    Token X2 = parseX();
    Token Q = getTempVar();
    Q.setType("bool");
    generate(ope,X1.getName(),X2.getName(),Q.getName());
    return Q;
}

void Semlyzer::parseG() {
    if(input.first == "关键字start")
        parseW();
    if(input.first == "关键字if"||input.first == "关键字do"||input.first == "标识符")
        parseS();
}

void Semlyzer::parseW() {
    Match("关键字start");
    parseB();
    Match("关键字end");
}

Token Semlyzer::parseH(const string &type){
    Token H("",type,"");
    if(input.first == "中括号左"){
        Match("中括号左");
        H.setSize(input.second);
        Match("数字");
        Match("中括号右");
        H.setType("array");
        return H;
    }
    return H;
}
