#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"

class Parser
{
private:
    Lexer lexer;
    string input;
    //��ȡ��һ������
    void getNextInput(istream &);
    //ƥ��һ���ս��
    void Match(const string &);
public:
    Parser();

};

#endif