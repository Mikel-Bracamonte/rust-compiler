#include "imp_visitor.h"


void CheckVisitor::check(Program* p) {
    env.clear();      
    env.add_level();
    cout<<"hola"<<endl;
    for(auto i : p->funs){
        i->accept(this);
    }
    cout<<"chau"<<endl;
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
    cout<<"probando assign"<<endl;
    if(!env.check(s->name)) {
        cout << "Error: varible '" << s->name << "' no declarada." << endl;
        exit(1);
    }
    ImpType type = env.lookup(s->name);
    ImpType exp = s->right->accept(this);
    if(!type.match(exp)){
        cout<<"Error: tipo incompatible de asignación con " << s->name<<"\n";
        exit(1);
    }
    }

void CheckVisitor::visit(PrintStatement* s) {
    s->exp->accept(this); // falt pero es pa que corra

}

void CheckVisitor::visit(IfStatement* s) {

}

void CheckVisitor::visit(WhileStatement* s) {

}

void CheckVisitor::visit(ForStatement* s) {
    
}

void CheckVisitor::visit(ReturnStatement* s) {
    
}

void CheckVisitor::visit(BreakStatement* s) {
    
}

void CheckVisitor::visit(ContinueStatement* s) {
    
}

void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        cout << "Error: variable '" << vd->name << "' ya declarada." << endl;
        exit(1);
    }

    ImpType type;
    if (vd->type == "i32") {
        type.set_basic_type(ImpType::INT);
    } else if (vd->type == "bool") {
        type.set_basic_type(ImpType::BOOL);
    } else {
        cout << "Tipo no reconocido: '" << vd->type << "'." << endl;
        exit(1);
    }

    env.add_var(vd->name, type);
}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    
}

void CheckVisitor::visit(ParamDec* vd) {

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
