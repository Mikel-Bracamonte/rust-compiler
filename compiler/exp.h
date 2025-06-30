#ifndef EXP_H
#define EXP_H
#include "imp_type.h"
#include <string>
#include <list>
#include "visitor.h"
using namespace std;

enum BinaryOp { PLUS_OP, MINUS_OP, MUL_OP, DIV_OP, LT_OP, LE_OP, EQ_OP, GT_OP, GE_OP, NEQ_OP, MOD_OP, AND_OP, OR_OP };

enum AssignOp { AS_PLUS_OP, AS_MINUS_OP, AS_MUL_OP, AS_DIV_OP, AS_MOD_OP, AS_ASSIGN_OP };

enum UnaryOp { U_NEG_OP, U_NOT_OP };

class Body;

class ImpVisitor;

class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ImpType accept(ImpVisitor* v) = 0;
    virtual ~Exp() = 0;
    bool hasParenthesis = false;
    static string binOpToChar(BinaryOp o);
    static string assignOpToChar(AssignOp o);
    static string unaryOpToChar(UnaryOp o);
};

class PostfixExp : public Exp {
public:
    Exp* left;
    string right;
    PostfixExp(Exp* l, string r);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~PostfixExp();
};

class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    BinaryExp(Exp* l, Exp* r, BinaryOp o);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~BinaryExp();
};

class UnaryExp : public Exp {
public:
    Exp* exp;
    UnaryOp op;
    UnaryExp(Exp* e, UnaryOp o);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~UnaryExp();
};

class NumberExp : public Exp {
public:
    int value;
    NumberExp(int v);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~NumberExp();
};

class BoolExp : public Exp {
public:
    int value;
    BoolExp(bool v);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~BoolExp();
};

class IdentifierExp : public Exp {
public:
    string name;
    bool borrow;
    IdentifierExp(string n, bool b);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~IdentifierExp();
};

class IfExp : public Exp {
public:
    Exp* condition;
    Exp* then;
    Exp* els;
    IfExp(Exp *c, Exp* t, Exp* e);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~IfExp();
};

class FunctionCallExp : public Exp {
public:
    string name;
    list<Exp*> argList;
    FunctionCallExp();
    void add(Exp* e);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~FunctionCallExp();
};

///////////////////////////////////////////////////////

class Stm {
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
    virtual void accept(ImpVisitor* v) = 0;
};


class AssignStatement : public Stm {
public:
    string name;
    Exp* right;
    AssignOp op;
    AssignStatement(string n, Exp* r, AssignOp o);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~AssignStatement();
};

class PrintStatement : public Stm {
public:
    Exp* exp;
    bool ln;
    PrintStatement(Exp* e, bool l);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~PrintStatement();
};

class IfStatement : public Stm {
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStatement(Exp* c, Body* t, Body* e);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~IfStatement();
};

class WhileStatement : public Stm {
public:
    Exp* condition;
    Body* body;
    WhileStatement(Exp* c, Body* b);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~WhileStatement();
};

class ForStatement : public Stm {
public:
    bool mut;
    string name;
    Exp* start;
    Exp* end;
    Body* body;
    ForStatement(bool m, string n, Exp* s, Exp* e, Body* b);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~ForStatement();
};

class ReturnStatement : public Stm {
public:
    Exp* exp;
    ReturnStatement(Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~ReturnStatement();
};

class BreakStatement : public Stm {
public:
    BreakStatement();
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~BreakStatement();
};

class ContinueStatement : public Stm {
public:
    ContinueStatement();
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~ContinueStatement();
};

class VarDec : public Stm {
public:
    string name;
    string type;
    bool isMut;
    Exp* exp;
    VarDec(string n, string t, bool m, Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~VarDec();
};

class FunctionCallStatement : public Stm {
public:
    string name;
    list<Exp*> argList;
    FunctionCallStatement();
    void add(Exp* e);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~FunctionCallStatement();
};

/////////////////////////////////////////

class ParamDec {
public:
    string name;
    string type;
    bool isMut;
    bool borrow;
    ParamDec(string n, string t, bool m, bool b);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~ParamDec();
};

class FunDec {
public:
    string name;
    string type; // default = 'void'
    list<ParamDec*> params;
    Body* body;
    FunDec();
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~FunDec();
};

//TODO constructors, accepts
class StructDec {
public:
    string name;
    list<AttrDec*> attrs;
    StructDec();
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~StructDec();
};

    //TODO constructors, accepts
class AttrDec {
public:
    string name;
    string type;
    AttrDec(string n, string t);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~AttrDec();
};

//TODO constructors, accepts
class StructExp : public Exp {
public:
    string name;
    list<StructExpAttr*> attrs;
    StructExp();
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~StructExp();
};

//TODO constructors, accepts
class StructExpAttr {
public:
    string name;
    Exp* exp;
    StructExpAttr(string n, Exp* e);
    int accept(Visitor* visitor);
    ImpType accept(ImpVisitor* v);
    ~StructExpAttr();
};

class StatementList {
public:
    list<Stm*> stms;
    StatementList();
    void add(Stm* s);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~StatementList();
};


class Body {
public:
    StatementList* stmList;
    Body(StatementList* s);
    int accept(Visitor* visitor);
    void accept(ImpVisitor* v);
    ~Body();
};


class Program {
public:
    list<FunDec*> funs;
    list<StructDec*> structs;
    Program();
    int accept(Visitor* v);
    void accept(ImpVisitor* v);
    ~Program();
};



#endif // EXP_H