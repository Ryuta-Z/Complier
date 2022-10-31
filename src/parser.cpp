#include"../include/parser.h"

Parser::Parser(ifstream &file):lexer(file){input = pair<string,string>("","");}

void Parser::run() {
    parseP();
}

void Parser::push(const string &form) {
    productionSequence.push_back(form);
}

void Parser::getNextInput() {
    input = lexer.getNextTokenAndSave();
}

Color setColor(DWORD fc, DWORD bg){
    return {fc % 16,bg % 16*16};
}

template<typename T1,typename T2>
std::basic_ostream<T1,T2>& operator<<(std::basic_ostream<T1,T2> &_os,Color col){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle,col.fc|col.bg);
    return _os;
}

template<typename T1,typename T2>
std::basic_ostream<T1,T2>& cr(std::basic_ostream<T1,T2> &_os){
    _os.put('\n');
    _os.flush();
    return _os;
}
void Parser::Match(const string &s) {
    if(input.first == ""){
        getNextInput();
        cout<<setColor(9)<<"[词]识别到单词:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<cr;
    }
    if(input.first == s){
        cout<<setColor(2)<<"[语]成功匹配到:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<endl;
        getNextInput();
        cout<<setColor(9)<<"[词]识别到单词:"<<"("
        <<input.first+","
        <<input.second
        <<")"<<cr;
        return;
    }
    throw("[语]匹配失败!,target:"
    +s+"~pointer:"
    +"("+input.first
    +","+input.second+")"
    +"At::["+to_string(lexer.getCurrentRow())
    +","+to_string(lexer.getCurrentColum())
    +"]"
    );

}
void Parser::parseP() {
    push("<程序> -> <变量说明部分>;<语句部分>");
    parseA();
    Match("分号");
    parseB();
}

void Parser::parseA() {
    push("<变量说明部分> -> <变量说明><标识符列表>");
    Match("关键字string");
    parseL();
}

void Parser::parseL() {
    push("<标识符列表> -> <标识符><数组标识><标识符列表p>");
    Match("标识符");
    parseH();
    parseLp();
}

void Parser::parseLp() {
    push("<标识符列表p> -> ,<标识符><数组标识><标识符列表p>");
    if(input.first == "逗号"){
        Match("逗号");
        Match("标识符");
        parseH();
        parseLp();
    }
    // if(input.first != "分号")
    //     throw("[语]分号缺失,At::["
    //     +to_string(lexer.getCurrentRow())
    //     +","+to_string(lexer.getCurrentColum())
    //     +"]");
}

void Parser::parseB() {
    push("<语句部分> -> <语句>;<语句部分p>");
    parseS();
    Match("分号");
    parseBp();
}

void Parser::parseBp() {
    push("<语句部分p> -> <语句>;<语句部分>");
    if(input.first == "标识符"||input.first == "关键字if"||input.first == "关键字do"){
        parseS();
        Match("分号");
        parseBp();
    }
}

void Parser::parseS() {
    push("<语句> -> <赋值语句>|<条件语句>|<循环语句>");
    if(input.first == "标识符")
        parseE();
    else if(input.first == "关键字if")
        parseF();
    else if(input.first == "关键字do")
        parseR();
}

void Parser::parseE() {
    push("<赋值语句> -> <标识符><数组标识>=<表达式>");
    Match("标识符");
    parseH();
    Match("赋值运算符");
    parseX();
}

void Parser::parseF() {
    push("<条件语句> -> if(<条件>)<嵌套语句>else<嵌套语句>");
    Match("关键字if");
    Match("左括号");
    parseQ();
    Match("右括号");
    parseG();
    Match("关键字else");
    parseG();
}

void Parser::parseR() {
    push("<循环语句> -> do<嵌套语句>while(<条件>)");
    Match("关键字do");
    parseG();
    Match("关键字while");
    Match("左括号");
    parseQ();
    Match("右括号");
}

void Parser::parseX() {
    push("<表达式> -> <项><表达式p>");
    parseT();
    parseXp();
}

void Parser::parseXp() {
    push("<表达式p> -> +<项><表达式p>");
    if(input.first == "连接运算符"){
        Match("连接运算符");
        parseT();
        parseXp();
    }

}

void Parser::parseT() {
    push("<项> -> <因子><项p>");
    parseD();
    parseTp();
}

void Parser::parseTp() {
    push("<项p> -> *<数字><项p>");
    if(input.first == "重复运算符"){
        Match("重复运算符");
        Match("数字");
        parseTp();
    }
}

void Parser::parseD() {
    push("<因子> -> <字符串>|<标识符><数组标识>|(<表达式>)");
    if(input.first == "字符串")
        Match("字符串");
    if(input.first == "标识符"){
        Match("标识符");
        parseH();
    }
    if(input.first == "左括号"){
        Match("左括号");
        parseX();
        Match("右括号");
    }
}

void Parser::parseQ() {
    push("<条件> -> <表达式><关系运算符><表达式>");
    parseX();
    Match("关系运算符");
    parseX();
}

void Parser::parseG() {
    push("<嵌套语句> -> <语句>|<符合语句>");
    if(input.first == "关键字start")
        parseW();
    if(input.first == "关键字if"||input.first == "关键字do"||input.first == "标识符")
        parseS();
}

void Parser::parseW() {
    push("<复合语句>->start<语句部分>end");
    Match("关键字start");
    parseB();
    Match("关键字end");
}

void Parser::parseH() {
    push("<字符串标识> -> [<数字>]|ε");
    if(input.first == "中括号左"){
        Match("中括号左");
        Match("数字");
        Match("中括号右");
    }
}

