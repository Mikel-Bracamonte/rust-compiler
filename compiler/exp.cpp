#include <iostream>
#include "exp.h"
using namespace std;
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
BoolExp::BoolExp(bool v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}

Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
NumberExp::~NumberExp() { }
BoolExp::~BoolExp() { }
IdentifierExp::~IdentifierExp() { }

AssignStatement::AssignStatement(const string& n, Exp* r, AssignOp o): name(n), right(r), op(o) {}
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



VarDec::VarDec(string n, string t, bool m, Exp* e): name(n), type(t), mut(m), exp(e) {}
VarDec::~VarDec() {
    delete exp;
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
    for (auto f : funs) {
        delete f;
    }
}

Stm::~Stm() {}
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
        default: c = "$";
    }
    return c;
}

string Exp::assignOpToChar(AssignOp op) {
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

