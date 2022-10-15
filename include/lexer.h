#ifndef LEXCIAL_H
#define LEXCIAL_H
#include<string>
#include<list>
#include<fstream>
using namespace std;
//标识
class Token
{
private:
    string name;
    string value;
    string type;
public:
    Token();

    string getName() const { return name; }
    void setName(const string &name_) { name = name_; }

    string getType() const { return type; }
    void setType(const string &type_) { type = type_; }

    Token(string n,string t,string value);

    string getValue() const { return value; }
    void setValue(const string &value_) { value = value_; }
    
    string toString();
    
};
//标识符表
class TokenTable
{
    private:
    
        list<Token> tokens;
    public:
        TokenTable();
        void add(const Token &);
        void show();
        bool find(const string &s)const;
        
};
//词法分析
class Lexer
{
private:
    int currentRow;
    int currentColum;
    char pointer;
    ifstream *SourceCodeStream;
public:
    Lexer(ifstream &);
    TokenTable tokTable;
    pair<string,string> getNextToken();
    static bool isDigit(char &);
    static bool isCharacter(char &);
    int getCurrentRow()const;
    int getCurrentColum()const;
};

#endif