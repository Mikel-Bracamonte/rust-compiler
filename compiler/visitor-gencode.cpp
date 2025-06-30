#include "imp_visitor.h"
#include <cassert>

ImpType BinaryExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType UnaryExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType NumberExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType BoolExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType IdentifierExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType IfExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType FunctionCallExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType StructExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType StructExpAttr::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

ImpType PostfixExp::accept(ImpVisitor* visitor) {
    return visitor->visit(this);
}

void AssignStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void PrintStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void IfStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void WhileStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void ForStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void ReturnStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void BreakStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void ContinueStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void VarDec::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void FunctionCallStatement::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void ParamDec::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void FunDec::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void StructDec::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void AttrDec::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void StatementList::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

void Body::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

//////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::getSize(string s) {
    if(s == "i32") {
        return 4;
    } else if(s == "i64" || s == "0num") {
        return 8;
    } else if(s == "bool") {
        return 1;
    } else {
        return structs_info[s].size;
    }
}

string GenCodeVisitor::getRegister(string s, string reg) {
    bool flag = false;
    if (reg == "a" || reg == "b" || reg == "c" || reg == "d"){
        flag = true;
    }
    else if(reg == "si" || reg == "di" || reg == "bp" || reg == "sp"){
        flag = false;
    }
    else {
        cout << "Registro falso" << endl;
        exit(0);
    }
    if(s == "i32") {
        if(flag) return "%e" + reg + "x";
        else return "%e" + reg;
    } else if(s == "i64" || s == "0num") {
        if(flag) return "%r" + reg + "x";
        else return "%r" + reg;
    } else if(s == "bool") {
        return "%" + reg + "l";
    } else {
        cout << "Tipo no aceptado" << endl;
        exit(0);
    }
}

bool GenCodeVisitor::isStruct(string type) {
    return type != "i32" && type != "i64" && type != "bool" && type != "0num";
}

void GenCodeVisitor::nose(int src_base, int dest_base, bool reference) {
    bool is_struct = isStruct(struct_name);
    if(!is_struct) {
        if(reference) {
            out << " movq " << src_base << "(%rbp), %rax" << endl;
            out << " movq %rax, " << dest_base << "(%rcx)" << endl;
        } else {
            out << " movq " << src_base << "(%rbp), %rax" << endl;
            out << " movq %rax, " << dest_base << "(%rbp)" << endl;
        }
        return;
    }

    string old_struct_name = struct_name;
    for(auto i : structs_info[struct_name].offsets) {
        struct_name = structs_info[struct_name].types[i.first].ttype;
        nose(src_base + i.second, dest_base + i.second, reference);
        struct_name = old_struct_name;
    }
}

//////////////////////////////////////////////////////////////////////////////////
void GenCodeVisitor::gencode(Program* p) {
    out << ".data" << endl;
    out << "print_fmt_ln: .string \"%ld \\n\""<<endl;
    out << "print_fmt: .string \"%ld \""<<endl;
    out << ".text" << endl;

    for(auto s: p->structs) {
        s->accept(this);
    }

    for(auto f: p->funs){
        f->accept(this);
    }

    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
}
//parser aumentar structs


ImpType GenCodeVisitor::visit(BinaryExp* e) {
    ImpType imp_type = e->left->accept(this);
    string reg = getRegister(imp_type.ttype, "a");
    out << " push " + reg << endl;
    ImpType imp_type2 = e->right->accept(this);
    string reg2 = getRegister(imp_type2.ttype, "c");
    string opReg = reg + ", " + reg2;
    out << " mov " + opReg << endl;
    out << " pop " + reg << endl;

    switch (e->op) {
        case PLUS_OP:
            out << " add " + opReg << endl; 
            break;
        case MINUS_OP:
            out << " sub " + opReg << endl;
            break;
        case MUL_OP:
            out << " imul " + opReg << endl;
            break;
        case DIV_OP:
            out << " cqto" << endl
                << " idiv " + reg2 << endl;
            break;
        case LT_OP:
            out << " cmp %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setl %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case LE_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setle %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case EQ_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " sete %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case GT_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setg %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case GE_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setge %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case NEQ_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setne %al" << endl
                << " movzbq %al, %rax" << endl;
            imp_type.ttype = "bool";
            break;
        case MOD_OP:
            out << " cqto" << endl
                << " idivq %rcx" << endl
                << " mov %rdx, %rax" << endl;
            break;
        case AND_OP:
            out << " and %cl, %al" << endl
                << " movzbq %al, %rax" << endl;
            break;
        case OR_OP:
            out << " or %cl, %al" << endl
                << " movzbq %al, %rax" << endl;
            break;
        default:
            errorHandler.error("Not binaryOp supported");
    }
    return imp_type;
}

// check?
ImpType GenCodeVisitor::visit(UnaryExp* e) {
    ImpType imp_type = e->exp->accept(this);
    switch (e->op) {
        case U_NEG_OP:
            out << " neg %rax" << endl; 
            break;
        case U_NOT_OP:
            out << " sub $1, %rax" << endl; 
            break;
        default:
            errorHandler.error("Not unaryOp supported");
    }
    return imp_type;
}

// check!
ImpType GenCodeVisitor::visit(NumberExp* e) {
    out << " mov $" << e->value << ", %rax"<<endl;
    return ImpType("0num");
}

// check!
ImpType GenCodeVisitor::visit(BoolExp* e) {
    out << " mov $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return ImpType("bool");
}

// seems check!
ImpType GenCodeVisitor::visit(IdentifierExp* e) {
    assert(env.check(e->name));
    ImpType imp_type = get<0>(env.lookup(e->name));
    string type = imp_type.ttype;
    bool is_struct = isStruct(type);
    if(is_struct) {
        struct_name = type;
        if(imp_type.reference) {
            struct_offset = 0;
        } else {
            struct_offset = get<1>(env.lookup(e->name));
        }
    } else {
        out << " mov " << get<1>(env.lookup(e->name)) << "(%rbp), %rax"<<endl;
    }
    return imp_type;
}

// check!
ImpType GenCodeVisitor::visit(IfExp* e) {
    int lbl = label_counter++;
    e->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je else_" << lbl << endl;
    ImpType imp_type = e->then->accept(this);
    out << " jmp endif_" << lbl << endl;
    out << "else_" << lbl << ":" << endl;
    e->els->accept(this);
    out << "endif_" << lbl << ":" << endl;
    
    return imp_type;
}

ImpType GenCodeVisitor::visit(FunctionCallExp* e) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = e->argList.begin(); it != e->argList.end(); ++it) {
        ImpType imp_type = (*it)->accept(this);
        if(isStruct(imp_type.ttype)) {
            out << " movq %rbp, %rax" << endl;
            out << " addq $" << struct_offset << ", %rax" << endl;
            out << " movq %rax, " << argRegs[i] << endl;
        } else {
            out << " movq %rax, " << argRegs[i] << endl;
        }
        ++i;
    }
    string ret_type = functions_info[e->name].ttype;
    if(isStruct(ret_type)) {
        out << " subq $" << structs_info[ret_type].size << ", %rsp" << endl;
        struct_offset = temp_offset;
        temp_offset -= structs_info[ret_type].size;
    }
    out << " call " << e->name << endl;

    return functions_info[e->name];
}

void GenCodeVisitor::visit(AssignStatement* s) {
    assert(env.check(s->names[0]));
    ImpType imp_type = s->right->accept(this);
    bool is_struct = isStruct(imp_type.ttype);
    int offset_assign = get<1>(env.lookup(s->names[0]));
    int var_offset = offset_assign;
    string name = get<0>(env.lookup(s->names[0])).ttype;
    bool reference = get<0>(env.lookup(s->names[0])).reference;
    if(reference) {
        offset_assign = 0;
    }
    for(int i = 1; i < s->names.size(); ++i) {
        offset_assign = offset_assign + structs_info[name].offsets[s->names[i]];
        name = structs_info[name].types[s->names[i]].ttype;
    }
    switch (s->op) {
        case AS_ASSIGN_OP:
            if(is_struct) {
                out << " mov " << var_offset << "(%rbp), %rcx" << endl;
                nose(struct_offset, offset_assign, reference);
            } else {
                if(reference) {
                    out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                    out << " mov %rax, " << offset_assign << "(%rcx)" << endl;
                } else {
                    out << " mov %rax, " << offset_assign << "(%rbp)" << endl;
                }
            }
            break;
        case AS_PLUS_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                out << " addq %rax, " << offset_assign << "(%rcx)" << endl;
            } else {
                out << " addq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MINUS_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rcx" << endl;
                out << " subq %rax, " << offset_assign << "(%rcx)" << endl;
            } else {
                out << " subq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MUL_OP: // not checked yet
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rdx" << endl;
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rdx), %rax" << endl;
                out << " imulq %rcx, %rax" << endl;
                out << " mov %rax, " << offset_assign << "(%rdx)" << endl;
            } else {
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rbp), %rax" << endl;
                out << " imulq %rcx, %rax" << endl;
                out << " mov %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_DIV_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rsi" << endl;
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rsi), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " mov %rax, " << offset_assign << "(%rsi)" << endl;
            } else {
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rbp), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " mov %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_MOD_OP:
            if(reference) {
                out << " movq " << var_offset << "(%rbp), %rsi" << endl;
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rsi), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " mov %rdx, " << offset_assign << "(%rsi)" << endl;
            } else {
                out << " mov %rax, %rcx" << endl;
                out << " mov " << offset_assign << "(%rbp), %rax" << endl;
                out << " cqto" << endl;
                out << " idivq %rcx" << endl;
                out << " mov %rdx, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        default:
            errorHandler.error("Not assignOp supported");
    }
}

// check!
void GenCodeVisitor::visit(PrintStatement* s) {
    s->exp->accept(this);
    out <<
        " mov %rax, %rsi" << endl;
    if(s->ln){
        out << " leaq print_fmt_ln(%rip), %rdi" << endl;
    }
    else {
        out << " leaq print_fmt(%rip), %rdi" << endl;
    }
    out << " movl $0, %eax" << endl
        << " call printf@PLT" << endl;
}

// check!
void GenCodeVisitor::visit(IfStatement* s) {
    int lbl = label_counter++;
    s->condition->accept(this);
    out << "  cmpq $0, %rax" << endl;
    out << "  je else_" << lbl << endl;
    s->then->accept(this);
    out << "  jmp endif_" << lbl << endl;
    out << "else_" << lbl << ":" << endl;
    if (s->els) s->els->accept(this);
    out << "endif_" << lbl << ":" << endl;
}

void GenCodeVisitor::visit(WhileStatement* s) {
    int lbl = label_counter++;
    
    nombreLoop.push("while_" + to_string(lbl));
    out << "while_" << lbl << ":" << endl;
    s->condition->accept(this);
    out << "  testq %rax, %rax" << endl; 
    out << "  je endwhile_" << lbl << endl;
    s->body->accept(this);
    out << "  jmp while_" << lbl << endl;
    out << "endwhile_" << lbl << ":" << endl;

    nombreLoop.pop();
}

void GenCodeVisitor::visit(ForStatement* s) {
    int lbl = label_counter++;
    string var = s->name;

    ImpType type("i32");
    env.add_var(var, {type, offset});
    int var_offset = offset;
    offset -= 8;

    s->start->accept(this);
    out << " mov %rax, " << offset << "(%rbp)" << endl;
    int current_offset = offset;
    offset -= 8;
    s->end->accept(this);
    out << " mov %rax, " << offset << "(%rbp)" << endl;
    int end_offset = offset;
    offset -= 8;
    
    out << "for_" << lbl << ":" << endl;
    out << " mov " << current_offset << "(%rbp), %rax" << endl;
    out << " mov " << end_offset << "(%rbp), %rcx" << endl;
    out << " cmpq %rcx, %rax" << endl;
    out << "jge endfor_" << lbl << endl;
    out << " mov %rax, " << var_offset << "(%rbp)" << endl;
    out << " addq $1, %rax" << endl;
    out << " mov %rax, " << current_offset << "(%rbp)" << endl;

    s->body->accept(this);

    out << "jmp for_" << lbl << endl;
    out << "endfor_" << lbl << ":" << endl;
}

void GenCodeVisitor::visit(ReturnStatement* s) {
    if(s->exp) {
        ImpType imp_type = s->exp->accept(this);
        if(isStruct(imp_type.ttype)) {
            nose(struct_offset, structs_info[imp_type.ttype].size + 16, imp_type.reference);
        }
    }
    out << "jmp .end_" << nombreFuncion << endl;
}

// check!
void GenCodeVisitor::visit(BreakStatement* s) {
    assert(!nombreLoop.empty());
    out << "jmp end" << nombreLoop.top() << endl;
}

// check!
void GenCodeVisitor::visit(ContinueStatement* s) {
    assert(!nombreLoop.empty());
    out << "jmp " << nombreLoop.top() << endl;
}

// seems check!
void GenCodeVisitor::visit(VarDec* vd) {
    ImpType type(vd->type);
    env.add_var(vd->name, {type, offset});
    int offset_assign = offset;
    bool is_struct = isStruct(vd->type);
    offset -=  is_struct ? structs_info[vd->type].size : getSize(vd->type); // TODO depende del tipo
    if(vd->exp != nullptr) {
        vd->exp->accept(this);
        if(is_struct) {
            nose(struct_offset, offset_assign, false);
        } else {
            out << " mov %rax, " << offset_assign << "(%rbp)" << endl;
        }
    }
}

void GenCodeVisitor::visit(FunctionCallStatement* stm) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = stm->argList.begin(); it != stm->argList.end(); ++it) {
        ImpType imp_type = (*it)->accept(this);
        if(isStruct(imp_type.ttype)) {
            out << " movq %rbp, %rax" << endl;
            out << " addq $" << struct_offset << ", %rax" << endl;
            out << " movq %rax, " << argRegs[i] << endl;
        } else {
            out << " movq %rax, " << argRegs[i] << endl;
        }
        ++i;
    }
    out << " call " << stm->name << endl;
}

void GenCodeVisitor::visit(ParamDec* vd) {
    ImpType type(vd->type);
    type.reference = isStruct(vd->type);
    env.add_var(vd->name, {type, offset});
    offset -= 8;
}

void GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    env.clear();
    env.add_level();
    offset = -8;
    nombreFuncion = f->name;
    functions_info[f->name] = ImpType(f->type);

    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    out << ".globl " << f->name << endl;
    out << f->name << ":" << endl;
    out << " push %rbp" << endl;
    out << " mov %rsp, %rbp" << endl;
    // int reserva = reserva_function[f->name];
    int reserva = 800; // deberia ser calculado con las variables
    temp_offset_base = -reserva;
    temp_offset = temp_offset_base;
    out << " subq $" << reserva << ", %rsp" << endl;

    int size = f->params.size();
    int i = 0;
    for(auto it = f->params.begin(); it != f->params.end(); ++it){
        out << " movq " << argRegs[i] << ", " << offset << "(%rbp)" << endl;
        (*it)->accept(this);
        ++i;
    }
    f->body->accept(this);

    out << ".end_" << nombreFuncion << ":" << endl;
    out << "leave" << endl;
    out << "ret" << endl;
    entornoFuncion = false;
}

void GenCodeVisitor::visit(StructDec* s){
    StructInfo struct_info;
    int offset_struct = 0;
    for(auto i : s->attrs) {
        struct_info.offsets[i->name] = offset_struct;
        struct_info.types[i->name] = ImpType(i->type);
        offset_struct -= getSize(i->type);
    }
    struct_info.size = -offset_struct;
    structs_info[s->name] = struct_info;
}

void GenCodeVisitor::visit(AttrDec* a){
    // No es necesario creo
}

ImpType GenCodeVisitor::visit(PostfixExp* e) {
    ImpType imp_type = e->left->accept(this);
    string type = imp_type.ttype;
    ImpType return_imp_type = structs_info[type].types[e->right];
    return_imp_type.reference = imp_type.reference;
    string return_type = return_imp_type.ttype;

    bool is_struct = isStruct(return_type);
    cout << struct_offset + structs_info[type].offsets[e->right] << " " << struct_name << endl;
    if(is_struct) {
        struct_name = return_type;
        struct_offset = struct_offset + structs_info[type].offsets[e->right];
    } else {
        if(imp_type.reference) {
            out << " mov " << struct_offset + structs_info[type].offsets[e->right] << "(%rax), %rax" << endl;
        } else {
            out << " mov " << struct_offset + structs_info[type].offsets[e->right] << "(%rbp), %rax" << endl;
        }
    }
    return return_imp_type;
}

ImpType GenCodeVisitor::visit(StructExp* e) {
    int current_struct_offset = temp_offset;

    out << " subq $" << structs_info[e->name].size << ", %rsp" << endl;
    temp_offset -= structs_info[e->name].size;
    cout << e->name << " " << structs_info[e->name].size << endl;
    for(auto a : e->attrs) {
        ImpType imp_type = a->exp->accept(this);
        string type = imp_type.ttype;
        bool is_struct = isStruct(type);
        if(is_struct) {
            nose(struct_offset, current_struct_offset + structs_info[e->name].offsets[a->name], false);
        } else {
            out << " mov %rax, " << current_struct_offset + structs_info[e->name].offsets[a->name] << "(%rbp)" << endl;
        }
    }
    struct_name = e->name;
    struct_offset = current_struct_offset;
    return ImpType(e->name);
}

ImpType GenCodeVisitor::visit(StructExpAttr* a) {
    // No es necesario creo
    return ImpType();
}

void GenCodeVisitor::visit(StatementList* s) {
    for (auto it : s->stms) {
        it->accept(this);
    }
}

void GenCodeVisitor::visit(Body* b) {
    env.add_level();
    b->stmList->accept(this);
    env.remove_level();
}

////////////////////////////////////////////////////