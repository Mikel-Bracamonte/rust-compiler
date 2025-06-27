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

int StructExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int StructExpAttr::accept(Visitor* visitor) {
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

int BreakStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int ContinueStatement::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int VarDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int FunctionCallStatement::accept(Visitor* visitor) {
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

int StructDec::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int AttrDec::accept(Visitor* visitor) {
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
    return string(4 * offset, ' ');
}

int PrintVisitor::visit(BinaryExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    exp->left->accept(this);
    cout << " " << Exp::binOpToChar(exp->op) << " ";
    exp->right->accept(this);
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(UnaryExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    cout << Exp::unaryOpToChar(exp->op);
    exp->exp->accept(this);
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    cout << exp->value;
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(BoolExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    if(exp->value) cout << "true";
    else cout << "false";
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(IdentifierExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    cout << exp->name;
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(IfExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    cout << "if ";
    exp->condition->accept(this);
    cout << " { ";
    exp->then->accept(this);
    cout << " } else { ";
    exp->els->accept(this);
    cout << " }";
    if(exp->hasParenthesis) cout << ")";
    return 0;
}

int PrintVisitor::visit(StructExp* exp) {
    //TODO
    cout << exp->name << " { ";
    bool first = true;
    for (auto attr : exp->attrs) {
        if (!first) std::cout << ", ";
        first = false;
        attr->accept(this);
    }
    cout << " }";
    return 0;
}

int PrintVisitor::visit(StructExpAttr* attr) {
    //TODO
    cout << attr->name << ": ";
    if (attr->exp) {
        attr->exp->accept(this);  // imprime el valor de la expresión
    } else {
        cout << "null";
    }
    return 0;
}

int PrintVisitor::visit(FunctionCallExp* exp) {
    if(exp->hasParenthesis) cout << "(";
    cout << exp->name;
    cout << "(";
    for(auto it = exp->argList.begin(); it != exp->argList.end(); ++it) {
        (*it)->accept(this);
        if(next(it) != exp->argList.end()) {
            cout << ", ";
        }
    }
    cout << ")";
    if(exp->hasParenthesis) cout << ")";
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
    cout << get_spaces() << "for ";
    if(stm->mut) {
        cout << "mut ";
    }
    cout << stm->name << " in ";
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

void PrintVisitor::visit(BreakStatement* stm){
    cout << get_spaces() << "break;";
}

void PrintVisitor::visit(ContinueStatement* stm){
    cout << get_spaces() << "continue;";
}

void PrintVisitor::visit(VarDec* stm){
    cout << get_spaces() << "let ";
    if(stm->isMut) {
        cout << "mut ";
    }
    cout << stm->name << ": " << stm->type;
    if(stm->exp != nullptr){
        cout <<" = ";
        stm->exp->accept(this);
    }
    cout<< ";";
}

void PrintVisitor::visit(FunctionCallStatement* stm){
    cout << get_spaces() << stm->name << "(";
    for(auto it = stm->argList.begin(); it != stm->argList.end(); ++it) {
        (*it)->accept(this);
        if(next(it) != stm->argList.end()) {
            cout << ", ";
        }
    }
    cout << ")";
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
    cout << ") ";
    if(stm->type != "") {
        cout << "-> " << stm->type << " ";
    }
    cout << "{" << endl;
    ++offset;
    stm->body->accept(this);
    --offset;
    cout << get_spaces() << "}" << endl;
}

void PrintVisitor::visit(StructDec* stm) {
    // TODO
    cout << "struct " << stm->name << " {\n";
    for (auto attr : stm->attrs) {
        cout << "  ";
        attr->accept(this);
        cout << ",\n";
    }
    cout << "}";
}

void PrintVisitor::visit(AttrDec* attr) {
    // TODO
    cout << attr->name << ": " << attr->type;

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
