#include"lexer.h"
#include<iostream>
#include<fstream>
#include<tuple>
using namespace std;


int main(){
    Lexer lex;
    ifstream souceCode("code.ryta",ios::in);
    try
    {
        pair<string,string> token;
        while (token.first!="#")
        {
            token =   lex.getNextToken(souceCode);
            cout<<"("<<token.first<<","<<token.second<<")"<<endl; 
        }
        
   
        
    }
    catch(string e)
    {
        std::cerr << e << '\n';
    }
    TokenTable toks = lex.getTokenTable();
    toks.show();
    return 0;
}