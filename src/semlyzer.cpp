#include"../include/semlyzer.h"

Semlyzer::Semlyzer(ifstream &file)
    : Parser(file)
{
    NXQ = 0;
}

void Semlyzer::showMidCode() {
    cout << setColor(5) << "identifiers: ";
    for(int i = 0; i < lexer.tokTable.length(); i++){
        cout << lexer.tokTable.getToken(i).getName() <<  ";";
    }
    cout << setColor(5) << "\ntempvar: ";
    for(auto &c : tempvar){
        cout << c.getName() << ";";
    }
    cout << endl;
    for (int i = 0; i < datas.size(); i++)
    {
        cout <<setColor(5)<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            <<endl;
    }
    cout << setColor(0) << cr;
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
void Semlyzer::writeData(ofstream &file) {
    for (int i = 0; i < datas.size(); i++)
    {
        file<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            << endl;
    }
}
void Semlyzer::writeDebugData(ofstream &file) {
    /***词法分析二元式输出***/
    file << "--------------词法分析二元式------------------" << endl;
    lexer.writeDebugData(file);
    /***语法分析产生式输出***/
    file << "--------------语法分析产生式------------------" << endl;
    for(string &s:productionSequence){file << s << endl;}
    /***语义分析四元式输出***/
    file << "--------------语义分析四元式------------------" << endl;
    for (int i = 0; i < datas.size(); i++)
    {
        file<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            << endl;
    }
    /***标识符表输出****/
    file << "--------------标识符表------------------" << endl;
    file << "identifiers: ";
    for(int i = 0; i < lexer.tokTable.length(); i++){
        file << lexer.tokTable.getToken(i).getName() <<  ";";
    }
    /****临时变量****/
    file << "\n--------------临时变量表------------------" << endl;
    file << "tempvar: ";
    for(auto &c : tempvar){
        file << c.getName() << ";";
    }
    file << endl;
}

void Semlyzer::parseP() {
    push("<程序> -> <变量说明部分>;<语句部分>");
    parseA();
    Match("分号");
    parseB();
}

void Semlyzer::parseA() {
        push("<变量说明部分> -> <变量说明><标识符列表>");
        Match("关键字string");
        parseL("string");
}

void Semlyzer::parseL(const string &type) {
    push("<标识符列表> -> <标识符><数组标识><标识符列表p>");
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
    push("<标识符列表p> -> ,<标识符><数组标识><标识符列表p>");
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
    push("<语句部分> -> <语句>;<语句部分p>");
    parseS();
    Match("分号");
    parseBp();
}

void Semlyzer::parseBp() {
    push("<语句部分p> -> <语句>;<语句部分>");
    if(input.first == "标识符"||input.first == "关键字if"||input.first == "关键字do"){
        parseS();
        Match("分号");
        parseBp();
    }
}

void Semlyzer::parseS() {
    push("<语句> -> <赋值语句>|<条件语句>|<循环语句>");
    if(input.first == "标识符")
        parseE();
    else if(input.first == "关键字if")
        parseF();
    else if(input.first == "关键字do")
        parseR();
}

void Semlyzer::parseE() {
    push("<赋值语句> -> <标识符><数组标识>=<表达式>");
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
    push("<条件语句> -> if(<条件>)<嵌套语句>else<嵌套语句>");
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
    push("<循环语句> -> do<嵌套语句>while(<条件>)");
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
    push("<表达式> -> <项><表达式p>");
    Token T =parseT();
    return parseXp(T);
}

Token Semlyzer::parseXp(const Token &T) {
    push("<表达式p> -> +<项><表达式p>");
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
    push("<项> -> <因子><项p>");
    Token D = parseD();
    return parseTp(D);
}
Token Semlyzer::parseTp(const Token &D) {
    push("<项p> -> *<数字><项p>");
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
    push("<因子> -> <字符串>|<标识符><数组标识>|(<表达式>)");
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
    push("<条件> -> <表达式><关系运算符><表达式>");
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
    push("<嵌套语句> -> <语句>|<符合语句>");
    if(input.first == "关键字start")
        parseW();
    if(input.first == "关键字if"||input.first == "关键字do"||input.first == "标识符")
        parseS();
}

void Semlyzer::parseW() {
    push("<复合语句>->start<语句部分>end");
    Match("关键字start");
    parseB();
    Match("关键字end");
}

Token Semlyzer::parseH(const string &type){
    push("<字符串标识> -> [<数字>]|ε");
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
