#include"../include/semlyzer.h"

Semlyzer::Semlyzer(ifstream &file)
    : Parser(file)
{
    
}

void Semlyzer::parseP() {
    parseA();
    Match("分号");
    parseB();
}

void Semlyzer::parseA() {
    if(input.first == "关键字string"){
        string type = input.second;
        Match("关键字string");
        parseL(type);
    }
}

void Semlyzer::parseL(const string &type) {
    if(input.first == "标识符"){
        int idx = lexer.tokTable.find(input.second);
        if(idx == -1){throw("syntax_error:未声明标识符\""+input.second+"\"");}
        // lexer.tokTable.updateType(idx,type);
        parseLp(type);
    }
}

void Semlyzer::parseLp(const string &type) {
    if(input.first == "逗号"){
        Match("逗号");
        Match("标识符");
        int idx = lexer.tokTable.find(input.second);
        if(idx == -1){throw("syntax_error:未声明标识符\""+input.second+"\"");}
        // lexer.tokTable.updateType(idx,type);
        parseLp(type);
    }else{}
}

void Semlyzer::parseB() {
    
}

void Semlyzer::parseBp() {
    
}

void Semlyzer::parseS() {
    
}

void Semlyzer::parseE() {
    
}

void Semlyzer::parseF() {
    
}

void Semlyzer::parseR() {
    
}

void Semlyzer::parseX() {
    
}

void Semlyzer::parseXp() {
    
}

void Semlyzer::parseT() {
    
}

void Semlyzer::parseTp() {
    
}

void Semlyzer::parseD() {
    
}

void Semlyzer::parseQ() {
    
}

void Semlyzer::parseG() {
    
}

void Semlyzer::parseW() {
    
}
