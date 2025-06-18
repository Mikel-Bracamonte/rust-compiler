#include <iostream>
#include "exp.h"
using namespace std;

Exp::~Exp() {}

BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
BinaryExp::~BinaryExp() { delete left; delete right; }

UnaryExp::UnaryExp(Exp* e, UnaryOp o):exp(e),op(o) {}
UnaryExp::~UnaryExp() { delete exp; }

NumberExp::NumberExp(int v):value(v) {}
NumberExp::~NumberExp() { }

BoolExp::BoolExp(bool v):value(v) {}
BoolExp::~BoolExp() { }

IdentifierExp::IdentifierExp(std::string n):name(n) {}
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
void FunctionCallExp::add(Exp* e) {
    argList.push_back(e);
}
FunctionCallExp::~FunctionCallExp(){
    for(auto i : argList){
        delete i;
    }
}

//////////////////////////////////////////////////

Stm::~Stm() {}

AssignStatement::AssignStatement(std::string n, Exp* r, AssignOp o) {
    name = n;
    right = r;
    op = o;
}
AssignStatement::~AssignStatement() {
    delete right;
}

PrintStatement::PrintStatement(Exp* e, bool l): exp(e), ln(l) {}
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

ForStatement::ForStatement(bool m, std::string n, Exp *s, Exp *e, Body *b) {
    mut = m;
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

VarDec::VarDec(std::string n, std::string t, bool m, Exp* e) {
    name = n;
    type = t;
    isMut = m;
    exp = e;
}
VarDec::~VarDec() {
    delete exp;
}

FunctionCallStatement::FunctionCallStatement() {}
void FunctionCallStatement::add(Exp* e) {
    argList.push_back(e);
}
FunctionCallStatement::~FunctionCallStatement(){
    for(auto i : argList){
        delete i;
    }
}

////////////////////////////////////////////////////

ParamDec::ParamDec(std::string n, std::string t, bool m) {
    name = n;
    type = t;
    isMut = m;
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
        case BinaryOp::PLUS_OP: c = "+"; break;
        case BinaryOp::MINUS_OP: c = "-"; break;
        case BinaryOp::MUL_OP: c = "*"; break;
        case BinaryOp::DIV_OP: c = "/"; break;
        case BinaryOp::LT_OP: c = "<"; break;
        case BinaryOp::LE_OP: c = "<="; break;
        case BinaryOp::EQ_OP: c = "=="; break;
        case BinaryOp::GT_OP: c = ">"; break;
        case BinaryOp::GE_OP: c = ">="; break;
        case BinaryOp::NEQ_OP: c = "!="; break;
        case BinaryOp::MOD_OP: c = "%"; break;
        case BinaryOp::AND_OP: c = "&&"; break;
        case BinaryOp::OR_OP: c = "||"; break;
        default: c = "$";
    }
    return c;
}

string Exp::assignOpToChar(AssignOp op) {
    string  c;
    switch(op) {
        case AssignOp::AS_PLUS_OP: c = "+="; break;
        case AssignOp::AS_MINUS_OP: c = "-="; break;
        case AssignOp::AS_MUL_OP: c = "*="; break;
        case AssignOp::AS_DIV_OP: c = "/="; break;
        case AssignOp::AS_MOD_OP: c = "%="; break;
        case AssignOp::AS_ASSIGN_OP: c = "="; break;
        default: c = "$";
    }
    return c;
}

string Exp::unaryOpToChar(UnaryOp op) {
    string c;
    switch(op) {
        case UnaryOp::U_NEG_OP: c = "-"; break;
        case UnaryOp::U_NOT_OP: c = "!"; break;
        default: c = "$";
    }
    return c;
}


