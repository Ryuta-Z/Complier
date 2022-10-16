#ifndef SEMLYZER_H
#define SEMLYZER_H
#include"parser.h"

//ËÄÔªÊ½
struct metadata
{
    string what;
    string leftvalue;
    string rightvalue;
    string target;
    metadata(string wh,string left,string right,string targ)
        :what(wh),leftvalue(left),rightvalue(right),target(targ){}
};

class Semlyzer:protected Parser 
{
private:
    vector<metadata> datas;
    vector<Token> tempvar;
    int NXQ;
    void parseP();
    void parseA();
    void parseL(const string &);
    void parseLp(const string &);
    void parseB();
    void parseBp();
    void parseS();
    void parseE();
    void parseF();
    void parseR();
    Token parseX();
    Token parseXp(const Token &);
    Token parseT();
    Token parseTp(const Token &);
    Token parseD();
    Token parseQ();
    void parseG();
    void parseW(); 
    Token parseH(const string &type);
protected:
    void generate(const string &,const string &,const string &,const string &);
    string SyntaxErrorGener(const string &type);
    Token getTempVar();
    void BackPath(const int &,const int &); 
public:
    Semlyzer(ifstream &file);
    void showMidCode();
    string getRow();
    string getColum();
    void Go();   
};

    
#endif