#include"../include/lexer.h"
#include"../include/parser.h"
#include<iostream>
#include<fstream>
#include<tuple>
using namespace std;


int main(){
    ifstream souceCode("code.ryta",ios::in);
    Parser parser(souceCode);
    parser.run();
    // Lexer lex(souceCode);
    // try
    // {
    //     pair<string,string> token;
    //     while (token.first!="#")
    //     {
    //         token =   lex.getNextToken();
    //         cout<<"("<<token.first<<","<<token.second<<")"<<endl; 
    //     }
        
   
        
    // }
    // catch(string e)
    // {
    //     std::cerr << e << '\n';
    // }
    // TokenTable toks = lex.getTokenTable();
    // toks.show();
    return 0;
}