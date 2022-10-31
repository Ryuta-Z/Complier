#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"
#include<iostream>
class Parser
{
protected:
    Lexer lexer;
    pair<string,string> input;
    vector<string> productionSequence;
    //获取下一个输入
    void getNextInput();
    //匹配一个终结符
    void Match(const string &);
    /*语法分析子程序*/
private:
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
    void parseH();

public:
    Parser(ifstream &);
    void run();
    void push(const string &);
};
//输出颜色改变
//设置（字体色，背景色）
struct Color{DWORD fc; DWORD bg;};
Color setColor(DWORD  = 7, DWORD  = 0);
template<typename T1,typename T2>
std::basic_ostream<T1,T2>& operator<<(std::basic_ostream<T1,T2> &,Color);
template<typename T1,typename T2>
std::basic_ostream<T1,T2>& cr(std::basic_ostream<T1,T2> &);
#endif