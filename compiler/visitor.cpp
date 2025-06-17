#include <iostream>
#include "exp.h"
#include "visitor.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int UnaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdentifierExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IfExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int FunctionCallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ForStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ReturnStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ParamDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int FunDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int StatementList::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
int Body::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////

string PrintVisitor::get_spaces() {
    return string(' ', offset*4);
}

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << " " << Exp::binOpToChar(exp->op) << " ";
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(UnaryExp* exp) {
    cout << " " << Exp::unaryOpToChar(exp->op);
    exp->exp->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->value) cout << "true";
    else cout << "false";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    cout << exp->name;
    return 0;
}

int PrintVisitor::visit(IfExp* exp) {
    cout << "if ";
    exp->condition->accept(this);
    cout << " { ";
    exp->then->accept(this);
    cout << " } else { ";
    exp->els->accept(this);
    cout << " }";
    return 0;
}

int PrintVisitor::visit(FunctionCallExp* exp) {
    cout << exp->name;
    cout << "(";
    for(auto it = exp->argList.begin(); it != exp->argList.end(); ++it) {
        (*it)->accept(this);
        if(next(it) != exp->argList.end()) {
            cout << ", ";
        }
    }
    cout << ")";
    return 0;
}

void PrintVisitor::visit(AssignStatement* stm) {
    cout << get_spaces() << stm->name << " " << Exp::assignOpToChar(stm->op) << " ";
    stm->right->accept(this);
    cout << ";";
}

void PrintVisitor::visit(PrintStatement* stm) {
    cout << get_spaces() << "println!(\"{}\", ";
    stm->exp->accept(this);
    cout << ");";
}

void PrintVisitor::visit(IfStatement* stm) {
    cout << get_spaces() << "if ";
    stm->condition->accept(this);
    cout << " {" << endl;
    ++offset;
    stm->then->accept(this);
    --offset;
    cout << get_spaces() << "} ";
    if(stm->els){
        cout << "else {" << endl;
        ++offset;
        stm->els->accept(this);
        --offset;
        cout << get_spaces() << "}";
    }
}

void PrintVisitor::visit(WhileStatement* stm){
    cout << get_spaces() << "while ";
    stm->condition->accept(this);
    cout << " {" << endl;
    ++offset;
    stm->body->accept(this);
    --offset;
    cout << get_spaces() << "}";
}

void PrintVisitor::visit(ForStatement* stm){
    cout << get_spaces() << "for " << stm->name << " in ";
    stm->start->accept(this);
    cout << "..";
    stm->end->accept(this);
    cout << " {" << endl;
    ++offset;
    stm->body->accept(this);
    --offset;
    cout << get_spaces() << "}";
}

void PrintVisitor::visit(ReturnStatement* stm){
    cout << get_spaces() << "return ";
    stm->exp->accept(this);
    cout << ";";
}

void PrintVisitor::visit(VarDec* stm){
    cout << get_spaces() << "let ";
    if(stm->isMut) {
        cout << "mut ";
    }
    cout << stm->name << " = ";
    stm->exp->accept(this);
    cout << ": " << stm->type << ";";
}

void PrintVisitor::visit(ParamDec* stm){
    cout << stm->name << ": " << stm->type;
}

void PrintVisitor::visit(FunDec* stm){
    cout << get_spaces() << "fn ";
    cout << stm->name;
    cout << "(";
    for(auto it = stm->params.begin(); it != stm->params.end(); ++it){
        (*it)->accept(this);
        if(next(it) != stm->params.end()) {
            cout << ", ";
        }
    }
    cout << "{" << endl;
    ++offset;
    stm->body->accept(this);
    --offset;
    cout << get_spaces() << "}";
}

void PrintVisitor::visit(StatementList* stm){
    for(auto i: stm->stms){
        i->accept(this);
        cout << endl;
    }
}

void PrintVisitor::visit(Body* stm){
    stm->stmList->accept(this);
}

void PrintVisitor::print(Program* program){
    offset = 0;
    for(auto f : program->funs) {
        f->accept(this);
    }
};
