#include"../include/parser.h"

Parser::Parser(ifstream &file):lexer(file){input = pair<string,string>("","");}

void Parser::run() {
    parseP();
}

void Parser::push(const string &form) {
    productionSequence.push_back(form);
}

void Parser::getNextInput() {
    input = lexer.getNextTokenAndSave();
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
        cout<<setColor(9)<<"[��]ʶ�𵽵���:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<cr;
    }
    if(input.first == s){
        cout<<setColor(2)<<"[��]�ɹ�ƥ�䵽:"<<"("
        <<input.first<<","
        <<input.second
        <<")"<<endl;
        getNextInput();
        cout<<setColor(9)<<"[��]ʶ�𵽵���:"<<"("
        <<input.first+","
        <<input.second
        <<")"<<cr;
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
    push("<����> -> <����˵������>;<��䲿��>");
    parseA();
    Match("�ֺ�");
    parseB();
}

void Parser::parseA() {
    push("<����˵������> -> <����˵��><��ʶ���б�>");
    Match("�ؼ���string");
    parseL();
}

void Parser::parseL() {
    push("<��ʶ���б�> -> <��ʶ��><�����ʶ><��ʶ���б�p>");
    Match("��ʶ��");
    parseH();
    parseLp();
}

void Parser::parseLp() {
    push("<��ʶ���б�p> -> ,<��ʶ��><�����ʶ><��ʶ���б�p>");
    if(input.first == "����"){
        Match("����");
        Match("��ʶ��");
        parseH();
        parseLp();
    }
    // if(input.first != "�ֺ�")
    //     throw("[��]�ֺ�ȱʧ,At::["
    //     +to_string(lexer.getCurrentRow())
    //     +","+to_string(lexer.getCurrentColum())
    //     +"]");
}

void Parser::parseB() {
    push("<��䲿��> -> <���>;<��䲿��p>");
    parseS();
    Match("�ֺ�");
    parseBp();
}

void Parser::parseBp() {
    push("<��䲿��p> -> <���>;<��䲿��>");
    if(input.first == "��ʶ��"||input.first == "�ؼ���if"||input.first == "�ؼ���do"){
        parseS();
        Match("�ֺ�");
        parseBp();
    }
}

void Parser::parseS() {
    push("<���> -> <��ֵ���>|<�������>|<ѭ�����>");
    if(input.first == "��ʶ��")
        parseE();
    else if(input.first == "�ؼ���if")
        parseF();
    else if(input.first == "�ؼ���do")
        parseR();
}

void Parser::parseE() {
    push("<��ֵ���> -> <��ʶ��><�����ʶ>=<���ʽ>");
    Match("��ʶ��");
    parseH();
    Match("��ֵ�����");
    parseX();
}

void Parser::parseF() {
    push("<�������> -> if(<����>)<Ƕ�����>else<Ƕ�����>");
    Match("�ؼ���if");
    Match("������");
    parseQ();
    Match("������");
    parseG();
    Match("�ؼ���else");
    parseG();
}

void Parser::parseR() {
    push("<ѭ�����> -> do<Ƕ�����>while(<����>)");
    Match("�ؼ���do");
    parseG();
    Match("�ؼ���while");
    Match("������");
    parseQ();
    Match("������");
}

void Parser::parseX() {
    push("<���ʽ> -> <��><���ʽp>");
    parseT();
    parseXp();
}

void Parser::parseXp() {
    push("<���ʽp> -> +<��><���ʽp>");
    if(input.first == "���������"){
        Match("���������");
        parseT();
        parseXp();
    }

}

void Parser::parseT() {
    push("<��> -> <����><��p>");
    parseD();
    parseTp();
}

void Parser::parseTp() {
    push("<��p> -> *<����><��p>");
    if(input.first == "�ظ������"){
        Match("�ظ������");
        Match("����");
        parseTp();
    }
}

void Parser::parseD() {
    push("<����> -> <�ַ���>|<��ʶ��><�����ʶ>|(<���ʽ>)");
    if(input.first == "�ַ���")
        Match("�ַ���");
    if(input.first == "��ʶ��"){
        Match("��ʶ��");
        parseH();
    }
    if(input.first == "������"){
        Match("������");
        parseX();
        Match("������");
    }
}

void Parser::parseQ() {
    push("<����> -> <���ʽ><��ϵ�����><���ʽ>");
    parseX();
    Match("��ϵ�����");
    parseX();
}

void Parser::parseG() {
    push("<Ƕ�����> -> <���>|<�������>");
    if(input.first == "�ؼ���start")
        parseW();
    if(input.first == "�ؼ���if"||input.first == "�ؼ���do"||input.first == "��ʶ��")
        parseS();
}

void Parser::parseW() {
    push("<�������>->start<��䲿��>end");
    Match("�ؼ���start");
    parseB();
    Match("�ؼ���end");
}

void Parser::parseH() {
    push("<�ַ�����ʶ> -> [<����>]|��");
    if(input.first == "��������"){
        Match("��������");
        Match("����");
        Match("��������");
    }
}

