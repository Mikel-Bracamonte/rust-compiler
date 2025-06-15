#include <iostream>
#include "exp.h"
using namespace std;
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
BoolExp::BoolExp(bool v):value(v) {}
IdentifierExp::IdentifierExp(string n):name(n) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
BoolExp::~BoolExp() { }
IdentifierExp::~IdentifierExp() { }

AssignStatement::AssignStatement(string n, Exp* r, AssignOp o) {
    name = n;
    right = r;
    op = o;
}
AssignStatement::~AssignStatement() {
    delete right;
}
PrintStatement::PrintStatement(Exp* e): exp(e) {}
PrintStatement::~PrintStatement() {
    delete exp;
}

IfExp::IfExp(Exp *c, Exp *t, Exp *e) {
    condition = c;
    then = t;
    els = e;
}

IfExp::~IfExp() {
    delete condition;
    delete then;
    delete els;
}

IfStatement::IfStatement(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    delete then;
    delete els;
}
WhileStatement::WhileStatement(Exp* c, Body* b): condition(c), body(b) {}
WhileStatement::~WhileStatement() {
    delete condition;
    delete body;
}

FunctionCallExp::FunctionCallExp() {}

FunctionCallExp::~FunctionCallExp(){
    for(auto i : argList){
        delete i;
    }
}

VarDec::VarDec(string n, string t, Exp* e) {
    name = n;
    type = t;
    exp = e;
}
VarDec::~VarDec() {
    delete exp;
}

ParamDec::ParamDec(std::string n, std::string t) {
    name = n;
    type = t;
}

ParamDec::~ParamDec() {}


StatementList::StatementList(): stms() {}
void StatementList::add(Stm* s) {
    stms.push_back(s);
}

StatementList::~StatementList() {
    for (auto s: stms) {
        delete s;
    }
}

Body::Body(StatementList* s): stmList(s) {}

Body::~Body() {
    delete stmList;
}

ForStatement::ForStatement(std::string n, Exp *s, Exp *e, Body *b) {
    name = n;
    start = s;
    end = e;
    body = b;
}

ForStatement::~ForStatement() {
    delete start;
    delete end;
    delete body;
}



Program::Program() {

}
Program::~Program() {
    for(auto i : funs){
        delete i;
    }
}

Program::~Program() {
    delete body;
}
Stm::~Stm() {}
string Exp::binOpToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        case GT_OP: c = ">"; break;
        case GE_OP: c = ">="; break;
        case NEQ_OP: c = "!="; break;
        case MOD_OP: c = "%"; break;
        default: c = "$";
    }
    return c;
}

string Exp::assignOpToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+="; break;
        case MINUS_OP: c = "-="; break;
        case MUL_OP: c = "*="; break;
        case DIV_OP: c = "/="; break;
        case MOD_OP: c = "%="; break;
        case ASSIGN_OP: c = "="; break;
        default: c = "$";
    }
    return c;
}

ReturnStatement::ReturnStatement(Exp *e) {
    exp=e;
}
ReturnStatement::~ReturnStatement(){
    delete exp;
}

FunDec::FunDec() {

}

FunDec::~FunDec() {
    delete body;
}