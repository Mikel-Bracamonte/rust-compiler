#ifndef GENCODE
#define GENCODE

#include <unordered_map>
#include <stack>

#include "exp.h"
#include "imp_value_visitor.h"
#include "environment.h"

using namespace std;

class GenCodeVisitor : public ImpValueVisitor {
private:
    Environment<ImpValue> env;
    int label_counter;
    int offset;
    stack<int> label_stack;
public:
    void gencode(Program* p);
    ImpValue visit(BinaryExp* exp) override;
    ImpValue visit(NumberExp* exp) override;
    ImpValue visit(BoolExp* exp) override;
    ImpValue visit(IdentifierExp* exp) override;
    ImpValue visit(IfExp* exp) override;
    ImpValue visit(FunctionCallExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(ReturnStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(ParamDec* stm) override;
    void visit(FunDec* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

class CheckVisitor : ImpValueVisitor {
private:
    Environment<ImpValue> env;
public:
    ImpValue visit(BinaryExp* exp) override;
    ImpValue visit(NumberExp* exp) override;
    ImpValue visit(BoolExp* exp) override;
    ImpValue visit(IdentifierExp* exp) override;
    ImpValue visit(IfExp* exp) override;
    ImpValue visit(FunctionCallExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(ReturnStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(ParamDec* stm) override;
    void visit(FunDec* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
    void check(Program* p);
};

#endif