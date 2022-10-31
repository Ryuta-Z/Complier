#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semlyzer.h"
#include <iostream>
#include <fstream>
#define SOURCE "source.ryu"
#define DEBUGFILE "debug.txt"
#define OUTFILE "out.txt"
using namespace std;

int main(int argc, char *argv[])
{
    ifstream souceCode(SOURCE, ios::in);
    if (argc == 1)
    {
        //词法分析&&语法分析&&语义分析
        Semlyzer semer(souceCode);
        try
        {
            semer.Go();
            cout << setColor(2) << "analyze success......" << cr;

            semer.showMidCode();
        }
        catch (string &e)
        {
            std::cerr << setColor(12) << e << '\n';
            return 0;
        }
        ofstream debugDataOutPut(DEBUGFILE, ios::out);
        semer.writeDebugData(debugDataOutPut);
        ofstream Midcode(OUTFILE, ios::out);
        semer.writeData(Midcode);
    }
    else if (string(argv[1]) == "lex")
    {
        //词法分析only
        Lexer lex(souceCode);
        pair<string, string> temp;
        try
        {
            while (temp.first != "#")
            {
                temp = lex.getNextToken();
                cout << setColor(9) << "(" << temp.first << "," << temp.second << ")" << endl;
            }
        }
        catch (string &e)
        {
            std::cerr << e << '\n';
        }
    }
    else if (string(argv[1]) == "par")
    {
        //语法分析only
        Parser par(souceCode);
        try
        {
            par.run();   
        }
        catch(string &e)
        {
            std::cerr << e << '\n';
        }
        
    }
    else
    {
        cout << "请输入参数\"lex\"或\"par\"" << endl;
    }
    return 0;
}