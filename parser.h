#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"

class Parser
{
private:
    Lexer lexer;
    string input;
    //获取下一个输入
    void getNextInput(istream &);
    //匹配一个终结符
    void Match(const string &);
public:
    Parser();

};

#endif