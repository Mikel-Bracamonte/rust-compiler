#include "imp_visitor.h"

int CheckVisitor::getSize(string s) {
    if(s == "i32") {
        return 8;
    } else if(s == "i64") {
        return 8;
    } else if(s == "bool") {
        return 8;
    } else {
        return structs_info[s].size;
    }
}

void CheckVisitor::check(Program* p) {
    for(auto i : p->structs) {
        i->accept(this);
    }
    for(auto i : p->funs){
        i->accept(this);
    }
}

// tested
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
            if (v1.ttype == "i32" && v2.ttype == "i32") return ImpType("i32");
            else {
                errorHandler.error("Operación aritmética requiere enteros (i32).");
            }
        case LT_OP: case LE_OP: case GT_OP: case GE_OP: // long int // check!
            if (v1.ttype == "i32" && v2.ttype == "i32") return ImpType("bool"); 
            else {
                errorHandler.error("Operación de comparación requiere enteros (i32).");
            }
        case EQ_OP: case NEQ_OP: // structs?
            if(v1.ttype ==v2.ttype) return ImpType("bool"); // mismo tipo: bool, int
            else{
                errorHandler.error("Operación requiere de igualdad enteros (int) o booleanos (bool).");
            }
        case MOD_OP: // long int // check!
            if (v1.ttype == "i32" && v2.ttype == "i32") return ImpType("i32"); 
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

// check!, tested!
ImpType CheckVisitor::visit(UnaryExp* exp) {
    ImpType e = exp->exp->accept(this);
    ImpType result;
    // int, longint
    if(e.ttype != "i32" && e.ttype != "bool") {
        errorHandler.error("Operación unaria debe ser i32 or bool");
    }

    switch(exp->op) {
        case U_NEG_OP:
            if(e.ttype=="i32") return ImpType("i32");
            break;
        case U_NOT_OP:
            if(e.ttype=="bool") return ImpType("bool");
            break;
        default:
            errorHandler.error("Operador unario no reconocido: " + Exp::unaryOpToChar(exp->op));
    }
    errorHandler.error("Operador unario: " + Exp::unaryOpToChar(exp->op) + 
                        " no aceptado para el tipo '" + e.ttype + "'");
    return ImpType();
}

// check!, tested!
ImpType CheckVisitor::visit(NumberExp* e) {
    return ImpType("i32"); // long int
}

// check!, tested!
ImpType CheckVisitor::visit(BoolExp* e) {
    return ImpType("bool");
}

// check!, tested!
ImpType CheckVisitor::visit(IdentifierExp* e) {
    if(!env.check(e->name)) {
        errorHandler.error("Variable '" + e->name + "' no existe.");
    }
    
    ImpType tipo= env.lookup(e->name);
    
    return tipo;

    // struct will change it
    errorHandler.error("Variable '" + e->name + "' no declarada o tipo no reconocido.");
    return ImpType();
}

// check!, tested!
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

// seems check!, tested?
ImpType CheckVisitor::visit(FunctionCallExp* e) {
    if(!functions_info.count(e->name)) {
        errorHandler.error("Función '" + e->name + "' no existe.");
    }

    ImpType ftype = functions_info[e->name];

    if(ftype.ttype == ""){
        errorHandler.error("La función definida no retorna un valor");
    }
    if(e->argList.size() != ftype.types.size()){
        errorHandler.error("Llamada a la función '" + e->name + "' no coincide con la cantidad de parámetros definida");
    }
    int ite = 0;
    for(auto arg : e->argList){
        if(arg->accept(this).ttype != getType(ftype.types[ite])){
            errorHandler.error("El tipo del argumento '" + to_string(ite) + "'-ésimo no coincide con el de la función '" + e->name + "'");
        }
        ite++;
    }
    
    return ImpType(getType(ftype.ttype));
}

ImpType CheckVisitor::visit(StructExp* e) {
    return ImpType(e->name);
}

ImpType CheckVisitor::visit(StructExpAttr* e) {
    return ImpType();
}

// TODO chequear que left sea un struct y devolver el tipo de variable del atributo
ImpType CheckVisitor::visit(PostfixExp* e) {
    ImpType left_type = e->left->accept(this);
    return ImpType("i32");
}

// checked!, tested
// TODO ahora el id es un vector. por ahora hago names[0] para probar
void CheckVisitor::visit(AssignStatement* s) {
    return;
    if(!env.check(s->names[0])) {
        errorHandler.error("Varible '" + s->names[0] + "' no declarada.");
    }

    ImpType target = env.lookup(s->names[0]);
    ImpType src = s->right->accept(this);
    
    if (src.ttype != getType(target)) {
        errorHandler.error("Tipo incompatible en asignación a '" + s->names[0] + "'.");
    }
    if (s->op != AS_ASSIGN_OP){
        if(src.ttype != "i32") {
            errorHandler.error("Tipo incompatible con la operación de asignación: " + Exp::assignOpToChar(s->op));
        }
    }
}

// check!, tested!
void CheckVisitor::visit(PrintStatement* s) {
    return; // TODO quitar
    if(s->exp->accept(this).ttype != "i32"){
        errorHandler.error("La impresión debe ser un i32");
    }
}

// check!, tested!
void CheckVisitor::visit(IfStatement* s) {
    if (s->condition->accept(this).ttype == "bool") {
        s->then->accept(this);
        if (s->els) s->els->accept(this);
    } else{
        errorHandler.error("La condición del if debe ser bool.");
    }
}

// check!, tested!
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

// check!, tested
void CheckVisitor::visit(ForStatement* s) {
    // tiene que ser mut, int    
    numberLoop++;

    ImpType startT = s->start->accept(this);
    if (startT.ttype != "i32") {
        errorHandler.error("El inicio del for debe ser un entero.");
    }
    ImpType endT = s->end->accept(this);
    if (endT.ttype != "i32") {
        errorHandler.error("El final del for debe ser un entero.");
    }
    env.add_level();
    // add var unicamente al body de for, afuera no puede acceder
    string int_type = "i32";
    env.add_var(s->name, int_type);
    function_memory_map[function_name] += 24;
    s->body->stmList->accept(this);
    env.remove_level();

    numberLoop --;
}

// seems check, tested!
void CheckVisitor::visit(ReturnStatement* s) {
    if(s->exp == nullptr){
        if(returnType.ttype != ""){
            errorHandler.error("Return no es void");
        }
    }
    else {
        return; // TODO structs dentro de structs no se maneja
        if(s->exp->accept(this).ttype != getType(returnType.ttype)){
            errorHandler.error("Return no coindice con tipo de función");
        }
    }
}

// check!, tested!
void CheckVisitor::visit(BreakStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("Error: 'break' fuera de un bucle.");
    }
}

// check!, tested!
void CheckVisitor::visit(ContinueStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("Error: 'continue' fuera de un bucle.");
    }
}

// check!, tested!
void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        errorHandler.error("Variable '" + vd->name + "' ya declarada.");
    }
    // long int?
    // mut?
    // mapa con variables mutables
    if (vd->type == "i32" || vd->type == "i64" || vd->type == "bool" || structs_info.count(vd->type)) {
        function_memory_map[function_name] += getSize(vd->type);
        env.add_var(vd->name, vd->type);
    } else {
        errorHandler.error("Tipo de variable no reconocido: '" + vd->type + "'.");
    }

    if(vd->exp == nullptr) return;

    ImpType src = vd->exp->accept(this);
    
    if (src.ttype != getType(vd->type)) {
        errorHandler.error("Tipo incompatible en asignación a '" + vd->name + "'.");
    }

}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    if(!functions_info.count(stm->name)) {
        errorHandler.error("Función '" + stm->name + "' no existe.");
    }

    ImpType ftype = functions_info[stm->name];
    if(stm->argList.size() != ftype.types.size()){
        errorHandler.error("Llamada a la función '" + stm->name + "' no coincide con la cantidad de parámetros definida");
    }
    int ite = 0;
    for(auto arg : stm->argList){
        if(arg->accept(this).ttype != getType(ftype.types[ite])){
            errorHandler.error("El tipo del argumento '" + to_string(ite) + "'-ésimo no coincide con el de la función '" + stm->name + "'");
        }
        ite++;
    }
}

// check!, tested!
void CheckVisitor::visit(ParamDec* vd) {
    if (vd->type == "i32" || vd->type == "i64" || vd->type == "bool" || structs_info.count(vd->type)) {
        env.add_var(vd->name, vd->type);
        function_memory_map[function_name] += getSize(vd->type);
    } else {
        errorHandler.error("Tipo de parámetro no reconocido: '" + vd->type + "'.");
    }
}

// check!, tested?
void CheckVisitor::visit(FunDec* vd) {
    ImpType ftype;
    list<string> argTypes;
    function_name = vd->name;
    function_memory_map[function_name] = 8;
    
    returnType = vd->type;
    env.clear();
    env.add_level();

    for (auto p : vd->params)
        p->accept(this);

    for (auto p : vd->params)
        ftype.types.push_back(p->type);
    
    ftype.ttype = vd->type;
    
    functions_info[vd->name] = ftype;
    vd->body->accept(this); 
    env.remove_level();
}

void CheckVisitor::visit(StructDec* stm) {
    StructInfo struct_info;
    int size = 0;
    for(auto i : stm->attrs) {
        struct_info.types[i->name] = ImpType(i->type);
        size += getSize(i->type);
    }
    struct_info.size = size;
    structs_info[stm->name] = struct_info;
}

void CheckVisitor::visit(AttrDec* stm) {

}

// check!, tested!
void CheckVisitor::visit(StatementList* s) {
     for (auto stmt : s->stms) {
        stmt->accept(this); 
    }
}

// check!, tested!
void CheckVisitor::visit(Body* b) {
    env.add_level();
    b->stmList->accept(this);
    env.remove_level();
}
