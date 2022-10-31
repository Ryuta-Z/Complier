#include"../include/semlyzer.h"

Semlyzer::Semlyzer(ifstream &file)
    : Parser(file)
{
    NXQ = 0;
}

void Semlyzer::showMidCode() {
    cout << setColor(5) << "identifiers: ";
    for(int i = 0; i < lexer.tokTable.length(); i++){
        cout << lexer.tokTable.getToken(i).getName() <<  ";";
    }
    cout << setColor(5) << "\ntempvar: ";
    for(auto &c : tempvar){
        cout << c.getName() << ";";
    }
    cout << endl;
    for (int i = 0; i < datas.size(); i++)
    {
        cout <<setColor(5)<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            <<endl;
    }
    cout << setColor(0) << cr;
}

string Semlyzer::getRow() {
    return to_string(lexer.getCurrentRow());
}

string Semlyzer::getColum() {
    return to_string(lexer.getCurrentColum());
}

string Semlyzer::SyntaxErrorGener(const string &type) {
    if(type == "δ����")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"δ������ʶ��\""+input.second+"\"";
    else if(type == "�ظ�����")
        return string("syntax_error:")+"["+getRow()+","+getColum()+"]"+"�ظ�������ʶ��\""+input.second+"\"";
    else if(type == "������Χ")
        return string("over_range:")+"["+getRow()+","+getColum()+"]"+"��Ч�������������鷶Χ!";
    return "~undefined_error~";
}

void Semlyzer::generate(const string &what,const string &leftvalue,const string &rightvalue,const string &target) {
    datas.push_back(metadata(what,leftvalue,rightvalue,target));
    NXQ ++;
}

Token Semlyzer::getTempVar() {
    int number = tempvar.size();
    tempvar.push_back(Token(string("T")+to_string(number),"",""));
    return tempvar[number];
}

void Semlyzer::BackPath(const int &offset,const int &target) {
    datas[offset].target = to_string(target);
}

void Semlyzer::Go() {
    parseP();
}
void Semlyzer::writeData(ofstream &file) {
    for (int i = 0; i < datas.size(); i++)
    {
        file<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            << endl;
    }
}
void Semlyzer::writeDebugData(ofstream &file) {
    /***�ʷ�������Ԫʽ���***/
    file << "--------------�ʷ�������Ԫʽ------------------" << endl;
    lexer.writeDebugData(file);
    /***�﷨��������ʽ���***/
    file << "--------------�﷨��������ʽ------------------" << endl;
    for(string &s:productionSequence){file << s << endl;}
    /***���������Ԫʽ���***/
    file << "--------------���������Ԫʽ------------------" << endl;
    for (int i = 0; i < datas.size(); i++)
    {
        file<< "(" << i
            << ")" << "("
            << datas[i].what<<","
            << datas[i].leftvalue << ","
            << datas[i].rightvalue << ","
            << datas[i].target << ")"
            << endl;
    }
    /***��ʶ�������****/
    file << "--------------��ʶ����------------------" << endl;
    file << "identifiers: ";
    for(int i = 0; i < lexer.tokTable.length(); i++){
        file << lexer.tokTable.getToken(i).getName() <<  ";";
    }
    /****��ʱ����****/
    file << "\n--------------��ʱ������------------------" << endl;
    file << "tempvar: ";
    for(auto &c : tempvar){
        file << c.getName() << ";";
    }
    file << endl;
}

void Semlyzer::parseP() {
    push("<����> -> <����˵������>;<��䲿��>");
    parseA();
    Match("�ֺ�");
    parseB();
}

void Semlyzer::parseA() {
        push("<����˵������> -> <����˵��><��ʶ���б�>");
        Match("�ؼ���string");
        parseL("string");
}

void Semlyzer::parseL(const string &type) {
    push("<��ʶ���б�> -> <��ʶ��><�����ʶ><��ʶ���б�p>");
    if(input.first == "��ʶ��"){
        int idx = lexer.tokTable.find(input.second);
        Match("��ʶ��");
        lexer.tokTable.updateContent(idx,type);
        Token H  = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }
}

void Semlyzer::parseLp(const string &type) {
    push("<��ʶ���б�p> -> ,<��ʶ��><�����ʶ><��ʶ���б�p>");
    if(input.first == "����"){
        Match("����");
        int idx = lexer.tokTable.find(input.second);
        Match("��ʶ��");
        lexer.tokTable.updateContent(idx,type);
        Token H = parseH(type);
        lexer.tokTable.updateSize(idx,H.getSize());
        lexer.tokTable.updateType(idx,H.getType());
        parseLp(type);
    }else{}
}

void Semlyzer::parseB() {
    push("<��䲿��> -> <���>;<��䲿��p>");
    parseS();
    Match("�ֺ�");
    parseBp();
}

void Semlyzer::parseBp() {
    push("<��䲿��p> -> <���>;<��䲿��>");
    if(input.first == "��ʶ��"||input.first == "�ؼ���if"||input.first == "�ؼ���do"){
        parseS();
        Match("�ֺ�");
        parseBp();
    }
}

void Semlyzer::parseS() {
    push("<���> -> <��ֵ���>|<�������>|<ѭ�����>");
    if(input.first == "��ʶ��")
        parseE();
    else if(input.first == "�ؼ���if")
        parseF();
    else if(input.first == "�ؼ���do")
        parseR();
}

void Semlyzer::parseE() {
    push("<��ֵ���> -> <��ʶ��><�����ʶ>=<���ʽ>");
    int idx = lexer.tokTable.find(input.second);
    string name = input.second;
    if(idx == -1)throw(SyntaxErrorGener("δ����"));
    Match("��ʶ��");
    Token H = parseH(lexer.tokTable.getType(idx));
    if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
        throw(SyntaxErrorGener("������Χ"));
    Match("��ֵ�����");
    Token X = parseX();
    if(H.getType() != "array")
        generate("=",X.getName(),"null",name);
    else
        generate("=",X.getName(),"null",name+"["+H.getOffset()+"]");
}

void Semlyzer::parseF() {
    push("<�������> -> if(<����>)<Ƕ�����>else<Ƕ�����>");
    Match("�ؼ���if");
    Match("������");
    Token Q = parseQ();
    Match("������");
    generate("jnz",Q.getName(),"null",to_string(NXQ+2));
    int Exit1 = NXQ;
    generate("j","null","null","");
    parseG();
    int Exit2 = NXQ;
    generate("j","null","null","");
    BackPath(Exit1,NXQ);
    Match("�ؼ���else");
    parseG();
    BackPath(Exit2,NXQ);
}

void Semlyzer::parseR() {
    push("<ѭ�����> -> do<Ƕ�����>while(<����>)");
    Match("�ؼ���do");
    int position = NXQ;
    parseG();
    Match("�ؼ���while");
    Match("������");
    Token Q = parseQ();
    Match("������");
    generate("jnz",Q.getName(),"null",to_string(position));
}

Token Semlyzer::parseX() {
    push("<���ʽ> -> <��><���ʽp>");
    Token T =parseT();
    return parseXp(T);
}

Token Semlyzer::parseXp(const Token &T) {
    push("<���ʽp> -> +<��><���ʽp>");
    if(input.first == "���������"){
        Match("���������");
        Token T1 = parseT();
        Token Xp1 = getTempVar();
        Xp1.setType(T1.getType());
        generate("+",T.getName(),T1.getName(),Xp1.getName());
        return parseXp(Xp1);
    }
    return T;
}

Token Semlyzer::parseT() {
    push("<��> -> <����><��p>");
    Token D = parseD();
    return parseTp(D);
}
Token Semlyzer::parseTp(const Token &D) {
    push("<��p> -> *<����><��p>");
    if(input.first == "�ظ������"){
        Match("�ظ������");
        string name = input.second;
        Match("����");
        Token Tp = getTempVar();
        Tp.setType(D.getType());
        generate("*",D.getName(),name,Tp.getName());
        return parseTp(Tp);
    }
    return D;
}

Token Semlyzer::parseD() {
    push("<����> -> <�ַ���>|<��ʶ��><�����ʶ>|(<���ʽ>)");
    if(input.first == "�ַ���"){
        string s = input.second;
        Match("�ַ���");
        return Token(s,"string",s);
    }
    if(input.first == "��ʶ��"){
        int idx = lexer.tokTable.find(input.second);
        string name = input.second;
        Match("��ʶ��");
        if(idx == -1)throw(SyntaxErrorGener("δ����"));
        Token H = parseH(lexer.tokTable.getType(idx));
        if(H.getType()=="array"&&(H.getSizei() < 0||H.getSizei() > lexer.tokTable.getSize(idx)-1))
            throw(SyntaxErrorGener("������Χ"));
        if(H.getType() == "array")
            H.setName(name+"["+H.getOffset()+"]");
        else H.setName(name);
        return H;
    }
    if(input.first == "������"){
        Match("������");
        Token X = parseX();
        Match("������");
        return X;
    }
    return Token("error_in_D()","","");
}

Token Semlyzer::parseQ() {
    push("<����> -> <���ʽ><��ϵ�����><���ʽ>");
    Token X1 = parseX();
    string ope = input.second;
    Match("��ϵ�����");
    Token X2 = parseX();
    Token Q = getTempVar();
    Q.setType("bool");
    generate(ope,X1.getName(),X2.getName(),Q.getName());
    return Q;
}

void Semlyzer::parseG() {
    push("<Ƕ�����> -> <���>|<�������>");
    if(input.first == "�ؼ���start")
        parseW();
    if(input.first == "�ؼ���if"||input.first == "�ؼ���do"||input.first == "��ʶ��")
        parseS();
}

void Semlyzer::parseW() {
    push("<�������>->start<��䲿��>end");
    Match("�ؼ���start");
    parseB();
    Match("�ؼ���end");
}

Token Semlyzer::parseH(const string &type){
    push("<�ַ�����ʶ> -> [<����>]|��");
    Token H("",type,"");
    if(input.first == "��������"){
        Match("��������");
        H.setSize(input.second);
        Match("����");
        Match("��������");
        H.setType("array");
        return H;
    }
    return H;
}
