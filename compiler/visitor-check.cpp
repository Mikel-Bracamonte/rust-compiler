#include "imp_visitor.h"


void CheckVisitor::check(Program* p) {
    env.clear();      
    env.add_level();
    for(auto i : p->funs){
        i->accept(this);
    }
}

ImpType CheckVisitor::visit(BinaryExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(UnaryExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(NumberExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(BoolExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(IdentifierExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(IfExp* e) {

    return ImpType();
}

ImpType CheckVisitor::visit(FunctionCallExp* e) {

    return ImpType();
}

void CheckVisitor::visit(AssignStatement* s) { 
    if(!env.check(s->name)) {
        errorHandler.error("Error: varible '" + s->name + "' no declarada.");
    }
    if(env.lookup(s->name).ttype == ImpType::INT) {
        s->right->accept(this);
    } else if(env.lookup(s->name).ttype == ImpType::BOOL) {
        s->right->accept(this);
    }else {
        errorHandler.error("No coincide la asignación");
    }
}

void CheckVisitor::visit(PrintStatement* s) {
    s->exp->accept(this); 
}

void CheckVisitor::visit(IfStatement* s) {
    if (s->condition->accept(this).ttype == ImpType::BOOL) {
        s->then->accept(this);
        if (s->els) s->els->accept(this);
    } else{
        errorHandler.error("Error: la condición del if debe ser bool.");
    }
    
}

void CheckVisitor::visit(WhileStatement* stm) {
    if(stm->condition->accept(this).ttype == ImpType::BOOL) {
        stm->body->accept(this);
    }
    else {
        errorHandler.error("Error: la condición del while debe ser bool.");
    }

}

void CheckVisitor::visit(ForStatement* s) {
    if(s->start->accept(this).ttype == ImpType::INT){
        if(s->end->accept(this).ttype == ImpType::INT ){
            s->body->accept(this);
        } else {
            errorHandler.error("Error: el final del for debe ser un entero.");
        }
    } else {
        errorHandler.error("Error: el inicio del for debe ser un entero.");
    }
}

void CheckVisitor::visit(ReturnStatement* s) {
    
}

void CheckVisitor::visit(BreakStatement* s) {
    
}

void CheckVisitor::visit(ContinueStatement* s) {
    
}

void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        errorHandler.error("Error: variable '" + vd->name + "' ya declarada.");
    }

    ImpType type;
    // long int?
    if (vd->type == "i32") {
        type.set_basic_type(ImpType::INT);
        env.add_var(vd->name,type);
        
    } else if (vd->type == "bool") {
        type.set_basic_type(ImpType::BOOL);
        env.add_var(vd->name, type);
        
    } else {
        errorHandler.error("Error: tipo de variable no reconocido: '" + vd->type + "'.");
    }

}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    
}

void CheckVisitor::visit(ParamDec* vd) {
    ImpType t;
    env.add_level();
    if (vd->type == "i32") {
        t.set_basic_type(ImpType::INT);
        env.add_var(vd->name, t);
    } else if (vd->type == "bool") {
        t.set_basic_type(ImpType::BOOL);
        env.add_var(vd->name, t);
    } else {
        errorHandler.error("Error: tipo de parámetro no reconocido: '" + vd->type + "'.");
    }
    env.remove_level();
}

void CheckVisitor::visit(FunDec* vd) {
    ImpType ftype;
    list<string> argTypes;
    
    for (auto p : vd->params)
        argTypes.push_back(p->type);
    ftype.set_fun_type(argTypes, vd->type);
    env.add_var(vd->name, ftype);

    for (auto p : vd->params)
        p->accept(this);
    
    vd->body->accept(this); 
}

void CheckVisitor::visit(StatementList* s) {
     for (auto stmt : s->stms) {
        stmt->accept(this); 
    }
}

void CheckVisitor::visit(Body* b) {
    env.add_level();
    b->stmList->accept(this);
    env.remove_level();
}
