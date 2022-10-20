#include"../include/parser.h"

Parser::Parser(ifstream &file):lexer(file){input = pair<string,string>("","");}

void Parser::run() {
    parseP();
}

void Parser::getNextInput() {
    input = lexer.getNextToken();
}

Color setColor(DWORD fc, DWORD bg){
    return {fc % 16,bg % 16*16};
}

template<typename T1,typename T2>
std::basic_ostream<T1,T2>& operator<<(std::basic_ostream<T1,T2> &_os,Color col){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle,col.fc|col.bg);
    return _os;
}

template<typename T1,typename T2>
std::basic_ostream<T1,T2>& cr(std::basic_ostream<T1,T2> &_os){
    _os.put('\n');
    _os.flush();
    return _os;
}
void Parser::Match(const string &s) {
    if(input.first == ""){
        getNextInput();
        cout<<setColor(9)<<"[´Ê]Ê¶±ðµ½µ¥´Ê:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<cr;
    }
    if(input.first == s){
        cout<<setColor(2)<<"[Óï]³É¹¦Æ¥Åäµ½:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<endl;
        getNextInput();
        cout<<setColor(9)<<"[´Ê]Ê¶±ðµ½µ¥´Ê:"<<"("
        <<input.first+","
        <<input.second
        <<")"<<cr;
        return;
    }
    throw("[Óï]Æ¥ÅäÊ§°Ü!,target:"
    +s+"~pointer:"
    +"("+input.first
    +","+input.second+")"
    +"At::["+to_string(lexer.getCurrentRow())
    +","+to_string(lexer.getCurrentColum())
    +"]"
    );

}
void Parser::parseP() {
    parseA();
    Match("·ÖºÅ");
    parseB();
}

void Parser::parseA() {
    Match("¹Ø¼ü×Östring");
    parseL();
}

void Parser::parseL() {
    Match("±êÊ¶·û");
    parseH();
    parseLp();
}

void Parser::parseLp() {
    if(input.first == "¶ººÅ"){
        Match("¶ººÅ");
        Match("±êÊ¶·û");
        parseH();
        parseLp();
    }
    // if(input.first != "·ÖºÅ")
    //     throw("[Óï]·ÖºÅÈ±Ê§,At::["
    //     +to_string(lexer.getCurrentRow())
    //     +","+to_string(lexer.getCurrentColum())
    //     +"]");
}

void Parser::parseB() {
    parseS();
    Match("·ÖºÅ");
    parseBp();
}

void Parser::parseBp() {
    if(input.first == "±êÊ¶·û"||input.first == "¹Ø¼ü×Öif"||input.first == "¹Ø¼ü×Ödo"){
        parseS();
        Match("·ÖºÅ");
        parseBp();
    }
}

void Parser::parseS() {
    if(input.first == "±êÊ¶·û")
        parseE();
    else if(input.first == "¹Ø¼ü×Öif")
        parseF();
    else if(input.first == "¹Ø¼ü×Ödo")
        parseR();
}

void Parser::parseE() {
    Match("±êÊ¶·û");
    parseH();
    Match("¸³ÖµÔËËã·û");
    parseX();
}

void Parser::parseF() {
    Match("¹Ø¼ü×Öif");
    Match("×óÀ¨ºÅ");
    parseQ();
    Match("ÓÒÀ¨ºÅ");
    parseG();
    Match("¹Ø¼ü×Öelse");
    parseG();
}

void Parser::parseR() {
    Match("¹Ø¼ü×Ödo");
    parseG();
    Match("¹Ø¼ü×Öwhile");
    Match("×óÀ¨ºÅ");
    parseQ();
    Match("ÓÒÀ¨ºÅ");
}

void Parser::parseX() {
    parseT();
    parseXp();
}

void Parser::parseXp() {
    if(input.first == "Á¬½ÓÔËËã·û"){
        Match("Á¬½ÓÔËËã·û");
        parseT();
        parseXp();
    }

}

void Parser::parseT() {
    parseD();
    parseTp();
}

void Parser::parseTp() {
    if(input.first == "ÖØ¸´ÔËËã·û"){
        Match("ÖØ¸´ÔËËã·û");
        Match("Êý×Ö");
        parseTp();
    }
}

void Parser::parseD() {
    if(input.first == "×Ö·û´®")
        Match("×Ö·û´®");
    if(input.first == "±êÊ¶·û"){
        Match("±êÊ¶·û");
        parseH();
    }
    if(input.first == "×óÀ¨ºÅ"){
        Match("×óÀ¨ºÅ");
        parseX();
        Match("ÓÒÀ¨ºÅ");
    }
}

void Parser::parseQ() {
    parseX();
    Match("¹ØÏµÔËËã·û");
    parseX();
}

void Parser::parseG() {
    if(input.first == "¹Ø¼ü×Östart")
        parseW();
    if(input.first == "¹Ø¼ü×Öif"||input.first == "¹Ø¼ü×Ödo"||input.first == "±êÊ¶·û")
        parseS();
}

void Parser::parseW() {
    Match("¹Ø¼ü×Östart");
    parseB();
    Match("¹Ø¼ü×Öend");
}

void Parser::parseH() {
    if(input.first == "ÖÐÀ¨ºÅ×ó"){
        Match("ÖÐÀ¨ºÅ×ó");
        Match("Êý×Ö");
        Match("ÖÐÀ¨ºÅÓÒ");
    }
}

