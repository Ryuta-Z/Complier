#include"../include/lexer.h"
#include"../include/parser.h"
#include"../include/semlyzer.h"
#include<iostream>
#include<fstream>
#include<tuple>
using namespace std;


int main(){
    ifstream souceCode("source.ryu",ios::in);
    Semlyzer semer(souceCode);
    try
    {
            semer.Go();
            cout<<setColor(2) << "analyze success¡­¡­"<<cr;
    }
    catch(string& e)
    {
        std::cerr<<setColor(12)<< e << '\n';
        return 0;
    }
    

    cout <<setColor(4)<< "-----------ÖÐ¼ä´úÂë---------------"<<endl;
    semer.showMidCode();
    // Parser parser(souceCode);
    // parser.run();
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
    // TokenTable toks = lex.tokTable;
    // toks.show();
    return 0;
}