#include "imp_visitor.h"
#include <set>
#include <cassert>

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

bool checkBasicType(ImpType a){
    if(a.ttype == "i32" || a.ttype == "i64" || a.ttype == "0num" || a.ttype == "bool"){
        return true;
    }
    return false;
}

bool CheckVisitor::checkTypeOp(ImpType a, ImpType b) {
    if(!checkBasicType(a) && !checkBasicType(b)){
        errorHandler.error("Operaciones no están siendo realizados por tipos básicos");
    }
    if(a.ttype == b.ttype){
        return true;
    }
    else {
        if(a.ttype == "bool" || b.ttype == "bool"){
            errorHandler.error("Operaciones no aceptadas para algún booleano");
        }
        if(a.ttype == "0num" || b.ttype == "0num"){
            return true;
        }
        errorHandler.error("Operaciones realizadas entre i32 y i64");
    }
    return false;
}

string CheckVisitor::getTypeOp(ImpType a, ImpType b) {
    if(a.ttype == b.ttype){
        return a.ttype;
    }
    if(b.ttype == "0num"){
        swap(a, b);
    }
    return b.ttype;
}

set<string> typesNum = {"0num", "i32", "i64"};
// tested
ImpType CheckVisitor::visit(BinaryExp* exp) {
    ImpType v1 = exp->left->accept(this);
    ImpType v2 = exp->right->accept(this);
    // si es void -> ignorar
    checkTypeOp(v1, v2);// checkea si son el mismo tipo, o 0num para numeros
    string typeResult = getTypeOp(v1, v2);
    switch (exp->op) {
        case PLUS_OP: case MINUS_OP: case MUL_OP: case DIV_OP: // long int // check!
            if (typesNum.count(typeResult)) return ImpType(typeResult);
            else {
                errorHandler.error("Operación aritmética requiere enteros (i32).");
            }
        case LT_OP: case LE_OP: case GT_OP: case GE_OP: // long int // check!
            if (typesNum.count(typeResult)) return ImpType("bool"); 
            else {
                errorHandler.error("Operación de comparación requiere enteros (i32).");
            }
        case EQ_OP: case NEQ_OP: // structs?
            return ImpType("bool"); // mismo tipo: bool, int
            
        case MOD_OP: // long int // check!
            if (typesNum.count(typeResult)) return ImpType(typeResult); 
            else{
                errorHandler.error("Operación de módulo requiere enteros (int).");
            }           
        case AND_OP: case OR_OP: // check!
            if (typeResult == "bool") return ImpType("bool");
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
    if(!checkBasicType(e)) {
        errorHandler.error("Operación unaria debe ser número or bool");
    }

    switch(exp->op) {
        case U_NEG_OP:
            if(e.ttype!="bool") return ImpType(e.ttype);
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
    return ImpType("0num"); // long int
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
        checkTypeOp(thenT, elseT);
        return getTypeOp(thenT, elseT);
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
        ImpType imp = arg->accept(this);
        if(imp.ttype == ftype.types[ite]){
            ite ++;
            continue;
        }
        checkTypeOp(imp, ftype.types[ite]);
        ite++;
    }
    
    return ImpType(getType(ftype.ttype));
}

ImpType CheckVisitor::visit(StructExp* e) {
    /*
    if(structs_info.count(e->name)){ // el tipo del struct debería estar ya declarado
        errorHandler.error("El tipo '" + e->name + "' de struct no fue declarado");
    }

    if(structs_info[e->name].types.size() != e->attrs.size()){
        errorHandler.error("No coincide el número de atributos con el definido en el struct: " + e->name);
    }

    struct_name = e->name;
    for(auto a : e->attrs){
        a->accept(this);
    }
*/
    return ImpType(e->name);
}

ImpType CheckVisitor::visit(StructExpAttr* e) {
    /*
    if(!struct_name.empty()){
        errorHandler.error("Atributo definido fuera de un struct");
    }
    
    if(!structs_info[struct_name].types.count(e->name)){
        errorHandler.error("El atributo '" + e->name + "' no existe en el struct '" + e->name + "'.");
    }

    if(structs_info[struct_name].types[e->name].ttype != e->exp->accept(this).ttype){
        errorHandler.error("El tipo '" + e->name + "' no existe en el struct '" + e->name + "'.");
    }
*/
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
    if(!env.check(s->names[0])) {
        errorHandler.error("Varible '" + s->names[0] + "' no declarada.");
    }

    ImpType target = env.lookup(s->names[0]);
    ImpType src = s->right->accept(this);
   
    if (src.ttype != target.ttype) {
        if(src.ttype == "0num" && typesNum.count(target.ttype)){
            return;
        }
        errorHandler.error("Tipo incompatible en asignación a '" + s->names[0] + "'.");
    }
    if (s->op != AS_ASSIGN_OP){
        checkTypeOp(src, target);
        if(getTypeOp(src, target) == "bool") {
            errorHandler.error("Tipo incompatible con la operación de asignación: " + Exp::assignOpToChar(s->op));
        }
    }
}

// check!, tested!
void CheckVisitor::visit(PrintStatement* s) {
    if(!typesNum.count(s->exp->accept(this).ttype)){
        errorHandler.error("La impresión debe ser un número");
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
    if (!typesNum.count(startT.ttype)) {
        errorHandler.error("El inicio del for debe ser un entero.");
    }
    ImpType endT = s->end->accept(this);
    if (!typesNum.count(startT.ttype)) {
        errorHandler.error("El final del for debe ser un entero.");
    }
    checkTypeOp(startT, endT);
    ImpType type = getTypeOp(startT, endT);
    if(type.ttype == "0num") {
        type.ttype = "i32";
    }
    env.add_level();
    // add var unicamente al body de for, afuera no puede acceder
    function_memory_map[function_name] += 24;
    env.add_var(s->name, type.ttype);
    // TODO rust no tiene tipado definido para i.
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
        returnInsideFunc = true;
        ImpType imp = s->exp->accept(this);
        if(imp.ttype == returnType.ttype){
            return;
        }
        assert(returnType.ttype != "0num");
        checkTypeOp(imp, returnType);
    }
}

// check!, tested!
void CheckVisitor::visit(BreakStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("El 'break' fuera de un bucle.");
    }
}

// check!, tested!
void CheckVisitor::visit(ContinueStatement* s) {
    if (numberLoop <= 0) {
        errorHandler.error("El'continue' fuera de un bucle.");
    }
}

// check!, tested!
void CheckVisitor::visit(VarDec* vd) {
    if (env.check(vd->name)) {
        errorHandler.error("Variable '" + vd->name + "' ya declarada.");
    }

    if (vd->type == "i32" || vd->type == "i64" || vd->type == "bool" || structs_info.count(vd->type)) {
        function_memory_map[function_name] += getSize(vd->type);
        env.add_var(vd->name, vd->type);
    } else {
        errorHandler.error("Tipo de variable no reconocido: '" + vd->type + "'.");
    }

    if(vd->exp == nullptr) return;

    ImpType src = vd->exp->accept(this);

    checkTypeOp(src, ImpType(vd->type));
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
        ImpType imp = arg->accept(this);
        if(imp.ttype == ftype.types[ite]){
            ite ++;
            continue;
        }
        checkTypeOp(imp, ftype.types[ite]);
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
    returnInsideFunc = false;
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
    
    if(returnInsideFunc != !(vd->type == "")){
        errorHandler.error("Return es distinto a lo esperado");
    }
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
    // No es necesario
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


// TODO por si hay tiempo, agregar mut y assign checker