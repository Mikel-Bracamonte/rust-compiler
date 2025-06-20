#ifndef IMP_VALUE_VISITOR
#define IMP_VALUE_VISITOR

#include "exp.h"
#include "imp_value.h"

class ImpValueVisitor {
public:
    virtual ImpValue visit(BinaryExp* exp) = 0;
    virtual ImpValue visit(UnaryExp* exp) = 0;
    virtual ImpValue visit(NumberExp* exp) = 0;
    virtual ImpValue visit(BoolExp* exp) = 0;
    virtual ImpValue visit(IdentifierExp* exp) = 0;
    virtual ImpValue visit(IfExp* exp) = 0;
    virtual ImpValue visit(FunctionCallExp* exp) = 0;
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


#endif
