#include <iostream>
#include "exp.h"
using namespace std;

Exp::~Exp() {}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
BinaryExp::~BinaryExp() { delete left; delete right; }

NumberExp::NumberExp(int v):value(v) {}
NumberExp::~NumberExp() { }

BoolExp::BoolExp(bool v):value(v) {}
BoolExp::~BoolExp() { }

IdentifierExp::IdentifierExp(string n):name(n) {}
IdentifierExp::~IdentifierExp() { }

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

FunctionCallExp::FunctionCallExp() {}
FunctionCallExp::~FunctionCallExp(){
    for(auto i : argList){
        delete i;
    }
}

//////////////////////////////////////////////////

Stm::~Stm() {}

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

ReturnStatement::ReturnStatement(Exp *e) {
    exp=e;
}
ReturnStatement::~ReturnStatement(){
    delete exp;
}

VarDec::VarDec(string n, string t, Exp* e) {
    name = n;
    type = t;
    exp = e;
}
VarDec::~VarDec() {
    delete exp;
}

////////////////////////////////////////////////////

ParamDec::ParamDec(std::string n, std::string t) {
    name = n;
    type = t;
}
ParamDec::~ParamDec() {}

FunDec::FunDec() {}

FunDec::~FunDec() {
    delete body;
}

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

Program::Program() {}
Program::~Program() {
    for(auto i : funs){
        delete i;
    }
}

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


