#include "imp_visitor.h"


void CheckVisitor::check(Program* p) {
    for(auto i : p->funs){
        i->accept(this);
    }
}

ImpType CheckVisitor::visit(BinaryExp* exp) {
    ImpType v1 = exp->left->accept(this);
    ImpType v2 = exp->right->accept(this);
    // si es void -> ignorar
    if(v1.ttype != v2.ttype) {
        errorHandler.error("Operación binaria debe ser entre el mismo tipo");
    }
    /*
    if(v1.ttype == "void"  || v2.ttype == "void") {
        errorHandler.error("Operación binaria debe ser entre 'bool' o 'int'.");
    }
    */

    switch (exp->op) {
        case PLUS_OP: case MINUS_OP: case MUL_OP: case DIV_OP: // long int // check!
            if (v1.ttype == "int" && v2.ttype == "int") return ImpType("int");
            else {
                errorHandler.error("Operación aritmética requiere enteros (int).");
            }
        case LT_OP: case LE_OP: case GT_OP: case GE_OP: // long int // check!
            if (v1.ttype == "int" && v2.ttype == "int") return ImpType("bool"); 
            else {
                errorHandler.error("Operación de comparación requiere enteros (int).");
            }
        case EQ_OP: case NEQ_OP: // structs?
            if(v1.ttype ==v2.ttype) return ImpType("bool"); // mismo tipo: bool, int
            else{
                errorHandler.error("Operación requiere de igualdad enteros (int) o booleanos (bool).");
            }
        case MOD_OP: // long int // check!
            if (v1.ttype == "int" && v2.ttype == "int") return ImpType("int"); 
            else{
                errorHandler.error("Operación de módulo requiere enteros (int).");
            }           
        case AND_OP: case OR_OP: // check!
            if (v1.ttype == "bool" && v2.ttype == "bool") return ImpType("bool");
            else {
                errorHandler.error("Operación lógica requiere booleanos (bool).");
            }
        default:
            errorHandler.error("Operador binario no reconocido: " + Exp::binOpToChar(exp->op));
    }
    return ImpType();
}

// check!
ImpType CheckVisitor::visit(UnaryExp* exp) {
    ImpType e = exp->exp->accept(this);
    ImpType result;
    // int, longint
    if(e.ttype != "int" && e.ttype != "bool") {
        errorHandler.error("Operación unaria debe ser int or bool");
    }

    switch(exp->op) {
        case U_NEG_OP:
            if(e.ttype=="int") return ImpType("int");

        case U_NOT_OP:
            if(e.ttype=="bool") return ImpType("bool");
        default:
        errorHandler.error("Operador unario no reconocido: " + Exp::unaryOpToChar(exp->op));        
    }
    return ImpType();
}

// check!
ImpType CheckVisitor::visit(NumberExp* e) {
    return ImpType("int"); // long int
}

// check!
ImpType CheckVisitor::visit(BoolExp* e) {
    return ImpType("bool");
}

ImpType CheckVisitor::visit(IdentifierExp* e) {
    // chequear variable en entorno
    if(!env.check(e->name)) {
        errorHandler.error("Variable '" + e->name + "' no existe.");
    }
    
    ImpType tipo= env.lookup(e->name);
    
    if(tipo.ttype == "i32") return ImpType("int");
    if(tipo.ttype == "bool") return ImpType("bool");

    // struct will change it
    errorHandler.error("Variable '" + e->name + "' no declarada o tipo no reconocido.");
    return ImpType();
}

// check!
ImpType CheckVisitor::visit(IfExp* e) {
    if (e->condition->accept(this).ttype == "bool") {
        ImpType thenT = e->then->accept(this);
        ImpType elseT = e->els->accept(this);
        if(!thenT.match(elseT)){
            errorHandler.error("Then y Else de una expresión ternaria deben tener el mismo tipo");
        }
        return thenT;
    }
    errorHandler.error("La condición del if debe ser bool.");
    return ImpType();
}

// seems check!
ImpType CheckVisitor::visit(FunctionCallExp* e) {
    if(!env.check(e->name)) {
        errorHandler.error("Función '" + e->name + "' no existe.");
    }

    ImpType ftype = env.lookup(e->name);
    if(e->argList.size() != ftype.types.size()){
        errorHandler.error("Llamada a la función '" + e->name + "' no coincide con la cantidad de parámetros definida");
    }
    int ite = 0;
    for(auto arg : e->argList){
        if(arg->accept(this).ttype != ftype.types[ite]){
            errorHandler.error("El tipo del argumento '" + to_string(ite) + "'-ésimo no coincide con el de la función '" + e->name + "'");
        }
    }
    return ftype;
}

// check!
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

// check!
void CheckVisitor::visit(PrintStatement* s) {
    if(s->exp->accept(this).ttype != "int"){
        errorHandler.error("La impresión debe ser un int");
    }
}

// check!
void CheckVisitor::visit(IfStatement* s) {
    if (s->condition->accept(this).ttype == "bool") {
        s->then->accept(this);
        if (s->els) s->els->accept(this);
    } else{
        errorHandler.error("La condición del if debe ser bool.");
    }
}

// check!
void CheckVisitor::visit(WhileStatement* stm) {
    numberLoop ++;
    if(stm->condition->accept(this).ttype == "bool") {
        stm->body->accept(this);
    }
    else {
        errorHandler.error("La condición del while debe ser bool.");
    }
    numberLoop --;
}

// check!
void CheckVisitor::visit(ForStatement* s) {
    // tiene que ser mut, int    
    numberLoop ++;
    if(!s->mut){
        errorHandler.error("El for debe ser mut.");
    }

    ImpType startT = s->start->accept(this);
    if (startT.ttype != "int") {
        errorHandler.error("El inicio del for debe ser un entero.");
    }
    ImpType endT = s->end->accept(this);
    if (endT.ttype != "int") {
        errorHandler.error("El final del for debe ser un entero.");
    }
    s->body->accept(this);

    numberLoop --;
}

// seems check
void CheckVisitor::visit(ReturnStatement* s) {
    if(s->exp == nullptr){
        if(returnType.ttype != ""){
            errorHandler.error("Return no coindice con tipo de función");
        }
    }
    else {
        if(returnType.ttype != s->exp->accept(this).ttype){
            errorHandler.error("Return no coindice con tipo de función");
        }
    }
}

// check!
void CheckVisitor::visit(BreakStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("Error: 'break' fuera de un bucle.");
    }
}

// check!
void CheckVisitor::visit(ContinueStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("Error: 'break' fuera de un bucle.");
    }
}

// check!
void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        errorHandler.error("Variable '" + vd->name + "' ya declarada.");
    }
    // long int?
    // mut?
    // mapa con variables mutables
    if (vd->type == "i32" || vd->type == "i64" || vd->type == "bool" || structs_info.count(vd->type)) {
        env.add_var(vd->name, vd->type);
    } else {
        errorHandler.error("Tipo de variable no reconocido: '" + vd->type + "'.");
    }
}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    if(!env.check(stm->name)) {
        errorHandler.error("Función '" + stm->name + "' no existe.");
    }

    ImpType ftype = env.lookup(stm->name);
    if(stm->argList.size() != ftype.types.size()){
        errorHandler.error("Llamada a la función '" + stm->name + "' no coincide con la cantidad de parámetros definida");
    }
    int ite = 0;
    for(auto arg : stm->argList){
        if(arg->accept(this).ttype != ftype.types[ite]){
            errorHandler.error("El tipo del argumento '" + to_string(ite) + "'-ésimo no coincide con el de la función '" + stm->name + "'");
        }
    }    
}

// check!
void CheckVisitor::visit(ParamDec* vd) {
    if (vd->type == "i32" || vd->type == "i64" || vd->type == "bool" || structs_info.count(vd->type)) {
        env.add_var(vd->name, vd->type);
    } else {
        errorHandler.error("Tipo de parámetro no reconocido: '" + vd->type + "'.");
    }
}

// check!
void CheckVisitor::visit(FunDec* vd) {
    ImpType ftype;
    list<string> argTypes;
    returnType = vd->type;
    env.add_level();
    
    // check that types exists
    for (auto p : vd->params)
        p->accept(this);

    for (auto p : vd->params)
        ftype.types.push_back(p->type);
    
    ftype.ttype = vd->type;

    env.add_var(vd->name, ftype);
    
    vd->body->accept(this); 
    env.remove_level();
}

// check!
void CheckVisitor::visit(StatementList* s) {
     for (auto stmt : s->stms) {
        stmt->accept(this); 
    }
}

// check!
void CheckVisitor::visit(Body* b) {
    env.add_level();
    b->stmList->accept(this);
    env.remove_level();
}
