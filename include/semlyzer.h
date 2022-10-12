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
    list<metadata> datas;

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
public:
    Semlyzer(ifstream &file);
    
};

    
#endif