#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"

class BinaryExp;
class UnaryExp;
class NumberExp;
class BoolExp;
class IdentifierExp;
class IfExp;
class FunctionCallExp;
class AssignStatement;
class PrintStatement;
class IfStatement;
class WhileStatement;
class ForStatement;
class ReturnStatement;
class BreakStatement;
class ContinueStatement;
class VarDec;
class FunctionCallStatement;
class ParamDec;
class FunDec;
class StatementList;
class Body;
class Program;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(UnaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(BoolExp* exp) = 0;
    virtual int visit(IdentifierExp* exp) = 0;
    virtual int visit(IfExp* exp) = 0;
    virtual int visit(FunctionCallExp* exp) = 0;
    virtual void visit(AssignStatement* stm) = 0;
    virtual void visit(PrintStatement* stm) = 0;
    virtual void visit(IfStatement* stm) = 0;
    virtual void visit(WhileStatement* stm) = 0;
    virtual void visit(ForStatement* stm) = 0;
    virtual void visit(ReturnStatement* stm) = 0;
    virtual void visit(BreakStatement* stm) = 0;
    virtual void visit(ContinueStatement* stm) = 0;
    virtual void visit(VarDec* stm) = 0;
    virtual void visit(FunctionCallStatement* stm) = 0;
    virtual void visit(ParamDec* stm) = 0;
    virtual void visit(FunDec* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
};

class PrintVisitor : public Visitor {
private:
    int offset = 0;
    string get_spaces();
public:
    void print(Program* p);
    int visit(BinaryExp* exp) override;
    int visit(UnaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(BoolExp* exp) override;
    int visit(IdentifierExp* exp) override;
    int visit(IfExp* exp) override;
    int visit(FunctionCallExp* exp) override;
    void visit(AssignSftypetatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(ReturnStatement* stm) override;
    void visit(BreakStatement* stm) override;
    void visit(ContinueStatement* stm) override;
    void visit(FunctionCallStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(ParamDec* stm) override;
    void visit(FunDec* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

#endif