#include "imp_visitor.h"


void CheckVisitor::check(Program* p) {
    env.clear();      
    env.add_level();
    for(auto i : p->funs){
        i->accept(this);
    }
}

ImpType CheckVisitor::visit(BinaryExp* exp) {
    ImpType v1 = exp->left->accept(this);
    ImpType v2 = exp->right->accept(this);
    // si es void o notype -> ignorar
    if(v1.ttype == ImpType::VOID || v1.ttype == ImpType::NOTYPE || v2.ttype == ImpType::NOTYPE || v2.ttype == ImpType::VOID) {
        errorHandler.error("Operando inválido.");
    }

    switch (exp->op) {
        case PLUS_OP: case MINUS_OP: case MUL_OP: case DIV_OP: // long int
            if (v1.ttype == ImpType::INT && v2.ttype == ImpType::INT) return ImpType("int");
            else {
                errorHandler.error("Operación aritmética requiere enteros (int).");
            }
        case LT_OP: case LE_OP: case GT_OP: case GE_OP: // long int
            if (v1.ttype == ImpType::INT && v2.ttype == ImpType::INT) return ImpType("bool"); 
            else {
                errorHandler.error("Operación de comparación requiere enteros (int).");
            }
        case EQ_OP: case NEQ_OP:
            if(v1.ttype ==v2.ttype) return ImpType("bool"); 
            else{
                errorHandler.error("Operación requiere de igualdad enteros (int) o booleanos (bool).");
            }
        case MOD_OP: // long int
            if (v1.ttype == ImpType::INT && v2.ttype == ImpType::INT) return ImpType(ImpType::INT); 
            else{
                errorHandler.error("Operación de módulo requiere enteros (int).");
            }           
        case AND_OP: case OR_OP:
            if (v1.ttype == ImpType::BOOL && v2.ttype == ImpType::BOOL) return ImpType(ImpType::BOOL);
            else {
                errorHandler.error("Operación lógica requiere booleanos (bool).");
            }
        default:
            errorHandler.error("Operador binario no reconocido: " + Exp::binOpToChar(exp->op));
    }
    return ImpType();
}

ImpType CheckVisitor::visit(UnaryExp* exp) {
    ImpType e = exp->exp->accept(this);
    ImpType result;

    result.type = e.type;

    switch(exp->op) {
        case: U_NEG_OP:

        

    if (exp->op == NOT_OP) {
        if(result.type == ImpType::BOOL){
            return result;
        }
        else{
            cout<<"Debería ser un booleano";
            exit(0);
        }
    }
    
    
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
        errorHandler.error("varible '" + s->name + "' no declarada.");
    }
    ImpType target = env.lookup(s->name);
    ImpType src = s->right->accept(this);
    if (!target.match(src)) {
        errorHandler.error("Tipo incompatible en asignación a '" + s->name + "'.");
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
        errorHandler.error("La condición del if debe ser bool.");
    }
    
}

void CheckVisitor::visit(WhileStatement* stm) {
    if(stm->condition->accept(this).ttype == ImpType::BOOL) {
        stm->body->accept(this);
    }
    else {
        errorHandler.error("La condición del while debe ser bool.");
    }
}

void CheckVisitor::visit(ForStatement* s) {
    // tiene que ser mut, int    
    if(!s->mut){
        errorHandler.error("Error: el for debe ser mut.");
    }

    ImpType startT = s->start->accept(this);
    if (startT.ttype != ImpType::INT) {
        errorHandler.error("El inicio del for debe ser un entero.");
    }
    ImpType endT = s->end->accept(this);
    if (endT.ttype != ImpType::INT) {
        errorHandler.error("El final del for debe ser un entero.");
    }

}

void CheckVisitor::visit(ReturnStatement* s) {

}

void CheckVisitor::visit(BreakStatement* s) {
    // No hay chequeo necesario, solo se asegura que el break no esté fuera de un loop
    // crear el env in loop
    /*
    if (!env.in_loop()) {
        errorHandler.error("Error: 'break' fuera de un bucle.");
    }
    */
    
}

void CheckVisitor::visit(ContinueStatement* s) {
    // No hay chequeo necesario, solo se asegura que el continue no esté fuera de un loop
    // crear el env in loop
    /*
    if (!env.in_loop()) {
        errorHandler.error("Error: 'continue' fuera de un bucle.");
    }
    */
    
}

void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        errorHandler.error("Variable '" + vd->name + "' ya declarada.");
    }

    ImpType type;
    // long int?
    // mut?
    // mapa con variables mutables
    
    if (vd->type == "i32") {
        type.set_basic_type(ImpType::INT);
        env.add_var(vd->name,type);
        
    } else if (vd->type == "bool") {
        type.set_basic_type(ImpType::BOOL);
        env.add_var(vd->name, type);
    } else {
        errorHandler.error("Tipo de variable no reconocido: '" + vd->type + "'.");
    }

}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    
}

void CheckVisitor::visit(ParamDec* vd) {
    ImpType t;
    
    if (vd->type == "i32") {
        t.set_basic_type(ImpType::INT);
        env.add_var(vd->name, t);
    } else if (vd->type == "bool") {
        t.set_basic_type(ImpType::BOOL);
        env.add_var(vd->name, t);
    } else {
        errorHandler.error("Tipo de parámetro no reconocido: '" + vd->type + "'.");
    }
}

void CheckVisitor::visit(FunDec* vd) {
    ImpType ftype;
    list<string> argTypes;
    env.add_level();
    
    for (auto p : vd->params)
        argTypes.push_back(p->type);
    ftype.set_fun_type(argTypes, vd->type);
    env.add_var(vd->name, ftype);

    for (auto p : vd->params)
        p->accept(this);
    
    vd->body->accept(this); 
    env.remove_level();
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
