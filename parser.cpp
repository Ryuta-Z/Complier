#include"parser.h"

Parser::Parser(){}

void Parser::getNextInput(istream &file) {
    input = lexer.getNextToken(file);
}
void Parser::Match(const string &) {
    
}
