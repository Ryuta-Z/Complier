#ifndef LEXCIAL_H
#define LEXCIAL_H
#include<string>
#include<vector>
#include<fstream>
#include<windows.h>
#include<iostream>
using namespace std;
//标识
class Token
{
private:
    string name;
    string value;
    string type;
    string size; //数组长度
    string content; //数组类型
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

    string getContent() const { return content; }
    void setContent(const string &content_) { content = content_; }

    string getSize() const { return size; }
    void setSize(const string &size_) { size = size_; }
    int getSizei();
    string getOffset();
};
//标识符表
class TokenTable
{
    private:
    
        vector<Token> tokens;
    public:
        TokenTable();
        void add(const Token &);
        void show();
        int find(const string &) const;
        void updateValue(int ,const string &);
        void updateType(int ,const string &);
        void updateSize(int ,const string &);
        void updateContent(int, const string &);
        int getSize(int);
        Token getToken(int);
        string getType(int i);
        
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