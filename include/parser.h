#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"
#include<iostream>
class Parser
{
private:
    Lexer lexer;
    pair<string,string> input;
    //获取下一个输入
    void getNextInput();
    //匹配一个终结符
    void Match(const string &);
    /*语法分析子程序*/
    void parseP();
    void parseA();
    void parseL();
    void parseLp();
    void parseB();
    void parseBp();
    void parseS();
    void parseE();
    void parseF();
    void parseR();
    void parseX();
    void parseXp();
    void parseT();
    void parseTp();
    void parseD();
    void parseQ();
    void parseG();
    void parseW();

public:
    Parser(ifstream &);
    void run();
};

#endif