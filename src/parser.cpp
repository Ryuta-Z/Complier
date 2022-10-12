#include"../include/parser.h"

Parser::Parser(ifstream &file):lexer(file){input = pair<string,string>("","");}

void Parser::run() {
    parseP();
}

void Parser::getNextInput() {
    input = lexer.getNextToken();
}
void Parser::Match(const string &s) {
    if(input.first == ""){
        getNextInput();
        cout<<"[��]ʶ�𵽵���:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<endl;
    }
    if(input.first == s){
        cout<<"[��]�ɹ�ƥ�䵽:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<endl;
        getNextInput();
        cout<<"[��]ʶ�𵽵���:"<<"("
        <<input.first+","
        <<input.second
        <<")"<<endl;
        return;
    }
    throw("[��]ƥ��ʧ��!,target:"
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
    Match("�ֺ�");
    parseB();
}

void Parser::parseA() {
    Match("�ؼ���string");
    parseL();
}

void Parser::parseL() {
    Match("��ʶ��");
    parseLp();
}

void Parser::parseLp() {
    if(input.first == "����"){
        Match("����");
        Match("��ʶ��");
        parseLp();
    }
    // if(input.first != "�ֺ�")
    //     throw("[��]�ֺ�ȱʧ,At::["
    //     +to_string(lexer.getCurrentRow())
    //     +","+to_string(lexer.getCurrentColum())
    //     +"]");
}

void Parser::parseB() {
    parseS();
    Match("�ֺ�");
    parseBp();
}

void Parser::parseBp() {
    if(input.first == "��ʶ��"||input.first == "�ؼ���if"||input.first == "�ؼ���do"){
        parseS();
        Match("�ֺ�");
        parseBp();
    }
}

void Parser::parseS() {
    if(input.first == "��ʶ��")
        parseE();
    else if(input.first == "�ؼ���if")
        parseF();
    else if(input.first == "�ؼ���do")
        parseR();
}

void Parser::parseE() {
    Match("��ʶ��");
    Match("��ֵ�����");
    parseX();
}

void Parser::parseF() {
    Match("�ؼ���if");
    Match("������");
    parseQ();
    Match("������");
    parseG();
    Match("�ؼ���else");
    parseG();
}

void Parser::parseR() {
    Match("�ؼ���do");
    parseG();
    Match("�ؼ���while");
    Match("������");
    parseQ();
    Match("������");
}

void Parser::parseX() {
    parseT();
    parseXp();
}

void Parser::parseXp() {
    if(input.first == "���������"){
        Match("���������");
        parseT();
        parseXp();
    }

}

void Parser::parseT() {
    parseD();
    parseTp();
}

void Parser::parseTp() {
    if(input.first == "�ظ������"){
        Match("�ظ������");
        Match("����");
        parseTp();
    }
}

void Parser::parseD() {
    if(input.first == "�ַ���")
        Match("�ַ���");
    if(input.first == "��ʶ��")
        Match("��ʶ��");
    if(input.first == "������"){
        Match("������");
        parseX();
        Match("������");
    }
}

void Parser::parseQ() {
    parseX();
    Match("��ϵ�����");
    parseX();
}

void Parser::parseG() {
    if(input.first == "�ؼ���start")
        parseW();
    if(input.first == "�ؼ���if"||input.first == "�ؼ���do"||input.first == "��ʶ��")
        parseS();
}

void Parser::parseW() {
    Match("�ؼ���start");
    parseB();
    Match("�ؼ���end");
}

