#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"
#include<iostream>
class Parser
{
protected:
    Lexer lexer;
    pair<string,string> input;
    //��ȡ��һ������
    void getNextInput();
    //ƥ��һ���ս��
    void Match(const string &);
    /*�﷨�����ӳ���*/
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
};

#endif