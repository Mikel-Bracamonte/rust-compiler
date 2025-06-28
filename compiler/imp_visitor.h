#ifndef IMP_VALUE_VISITOR
#define IMP_VALUE_VISITOR

#include "exp.h"
#include "imp_type.h"
#include "environment.h"
#include "error_handler.h"

#include <unordered_map>
#include <stack>

class ImpVisitor {
public:
    virtual ImpType visit(BinaryExp* exp) = 0;
    virtual ImpType visit(UnaryExp* exp) = 0;
    virtual ImpType visit(NumberExp* exp) = 0;
    virtual ImpType visit(BoolExp* exp) = 0;
    virtual ImpType visit(IdentifierExp* exp) = 0;
    virtual ImpType visit(IfExp* exp) = 0;
    virtual ImpType visit(FunctionCallExp* exp) = 0;
    virtual ImpType visit(StructExp* exp) = 0;
    virtual ImpType visit(StructExpAttr* exp) = 0;
    virtual ImpType visit(PostfixExp* exp) = 0;
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
    virtual void visit(StructDec* stm) = 0;
    virtual void visit(AttrDec* stm) = 0;
    virtual void visit(StatementList* stm) = 0;
    virtual void visit(Body* b) = 0;
};

struct StructInfo {
    int size;
    unordered_map<string, int> offsets;
};

class GenCodeVisitor : public ImpVisitor {
private:
    Environment<tuple<ImpType, int>> env;
    stack<int> label_stack;
    std::ostream& out;
public:
    GenCodeVisitor(std::ostream& out, unordered_map<string, int> m) : out(out) {
        errorHandler = ErrorHandler("GenCodeVisitor");
        reserva_function = m;
    }
    unordered_map<string, int> reserva_function;
    unordered_map<string, StructInfo> structs_info;
    ErrorHandler errorHandler;
    unordered_map<string, int> memoria;
    int offset = -8;
    int label_counter = 0;
    bool entornoFuncion = false;
    string nombreFuncion;
    string struct_name;

    stack<string> nombreLoop;

    void gencode(Program* p);
    ImpType visit(BinaryExp* exp) override;
    ImpType visit(UnaryExp* exp) override;
    ImpType visit(NumberExp* exp) override;
    ImpType visit(BoolExp* exp) override;
    ImpType visit(IdentifierExp* exp) override;
    ImpType visit(IfExp* exp) override;
    ImpType visit(FunctionCallExp* exp) override;
    ImpType visit(StructExp* exp) override;
    ImpType visit(StructExpAttr* exp) override;
    ImpType visit(PostfixExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(ReturnStatement* stm) override;
    void visit(BreakStatement* stm) override;
    void visit(ContinueStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(FunctionCallStatement* stm) override;
    void visit(ParamDec* stm) override;
    void visit(FunDec* stm) override;
    void visit(StructDec* stm) override;
    void visit(AttrDec* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};

class CheckVisitor : public ImpVisitor {
private:
    Environment<ImpType> env;
    // siempre guarda los types como 'i32'
public:
    CheckVisitor(){}
    ErrorHandler errorHandler = ErrorHandler("CheckVisitor");
    string getType(ImpType imp){
        if(imp.ttype == "i32") return "int";
        return imp.ttype;
    }
    string getType(string type){
        if(type == "i32") return "int";
        return type;
    }
    
    unordered_map<string, StructInfo> structs_info;
    unordered_map<string, ImpType> functions_info;
    int numberLoop = 0;
    ImpType returnType = ImpType();
    void check(Program* p);
    ImpType visit(BinaryExp* exp) override;
    ImpType visit(UnaryExp* exp) override;
    ImpType visit(NumberExp* exp) override;
    ImpType visit(BoolExp* exp) override;
    ImpType visit(IdentifierExp* exp) override;
    ImpType visit(IfExp* exp) override;
    ImpType visit(FunctionCallExp* exp) override;
    ImpType visit(StructExp* exp) override;
    ImpType visit(StructExpAttr* exp) override;
    ImpType visit(PostfixExp* exp) override;
    void visit(AssignStatement* stm) override;
    void visit(PrintStatement* stm) override;
    void visit(IfStatement* stm) override;
    void visit(WhileStatement* stm) override;
    void visit(ForStatement* stm) override;
    void visit(ReturnStatement* stm) override;
    void visit(BreakStatement* stm) override;
    void visit(ContinueStatement* stm) override;
    void visit(VarDec* stm) override;
    void visit(FunctionCallStatement* stm) override;
    void visit(ParamDec* stm) override;
    void visit(FunDec* stm) override;
    void visit(StructDec* stm) override;
    void visit(AttrDec* stm) override;
    void visit(StatementList* stm) override;
    void visit(Body* b) override;
};


#endif
