#ifndef EXP_H
#define EXP_H
#include "imp_value.hh"
#include "imp_type.hh"
#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP, GT_OP, GE_OP, NEQ_OP, MOD_OP };

enum AssignOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, MOD_OP, ASSIGN_OP }

class Body;
class ImpValueVisitor;

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ImpValue accept(ImpValueVisitor* v) = 0;
    virtual ~Exp() = 0;
    static string binOpToChar(BinaryOp o);
};

class BinaryExp : public Exp {
public:
    Exp* left; 
    Exp* right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp o);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~BinaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    string name;
    IdentifierExp(string n);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~IdentifierExp();
};

class IfExp : public Exp {
public:
    Exp* condition;
    Exp* then; 
    Exp* els;
    IFExp(Exp *cond, Exp* t, Exp* e);
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~IFExp();
};

class FunctionCallExp : public Exp {
public: 
    string name;
    list<Exp*> argList;
    FunctionCallExp();
    int accept(Visitor* visitor);
    ImpValue accept(ImpValueVisitor* v);
    ~FunctionCallExp();
};

///////////////////////////////////////////////////////


class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
    virtual void accept(ImpValueVisitor* v) = 0;
};


class AssignStatement : public Stm {
public:
    string name;
    AssignOp op;
    Exp* right;
    AssignStatement(string n, Exp* r, AssignOp o);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* exp;
    PrintStatement(Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~PrintStatement();
};

class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* c, Body* t, Body* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* body;
    WhileStatement(Exp* c, Body* b);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~WhileStatement();
};

class ForStatement : public Stm {
public:
    string name;
    Exp* start;
    Exp* end;
    Body* body;
    ForStatement(string n, Exp* s, Exp* e, Body* b);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~ForStatement();
};

class ReturnStatement : public Stm {
public:
    Exp* exp;
    ReturnStatement(Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~ReturnStatement();
};

class VarDec : public Stm {
public:
    string name;
    string type;
    Exp* exp;
    VarDec(string n, string t, Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~VarDec();
};

/////////////////////////////////////////

class ParamDec {
public: 
    string name;
    string type;
    ParamDec(string n, string t);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~ParamDec();
}

class FunDec {
public:
    string name;
    string type; // default = 'void'
    list<ParamDec> params;
    Body* body;
    FunDec();
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~FunDec();
}

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* s);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~StatementList();
};


class Body {
public:
    StatementList* stmList;
    Body(StatementList* s);
    int accept(Visitor* visitor);
    void accept(ImpValueVisitor* v);
    ~Body();
};


class Program {
public:
    list<FunDec*> funs;
    Program();
    int accept(Visitor* v);
    void accept(ImpValueVisitor* v);
    ~Program();
};



#endif // EXP_H