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
    out << " pushq %rax" << endl;
    e->right->accept(this);
    out << " movq %rax, %rcx" << endl;
    out << " popq %rax" << endl;

    switch (e->op) {
        case PLUS_OP:
            out << " addq %rcx, %rax" << endl; 
            break;
        case MINUS_OP:
            out << " subq %rcx, %rax" << endl;
            break;
        case MUL_OP:
            out << " imulq %rcx, %rax" << endl;
            break;
        case DIV_OP:
            out << " cqto" << endl
                << " idivq %rcx" << endl;
            break;
        case LT_OP:
            out << " cmpq %rcx, %rax" << endl
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
                << " movq %rdx, %rax" << endl;
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
            out << " subq $1, %rax" << endl; 
            break;
        default:
            errorHandler.error("Not unaryOp supported");
    }
    return imp_type;
}

// check!
ImpType GenCodeVisitor::visit(NumberExp* e) {
    out << " movq $" << e->value << ", %rax"<<endl;
    return ImpType("i32");
}

// check!
ImpType GenCodeVisitor::visit(BoolExp* e) {
    out << " movq $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return ImpType("bool");
}

// seems check!
ImpType GenCodeVisitor::visit(IdentifierExp* e) {
    assert(env.check(e->name));
    string type = get<0>(env.lookup(e->name)).ttype;
    bool is_struct = type != "i32" && type != "i64" && type != "bool";
    if(is_struct) {
        struct_offset = get<1>(env.lookup(e->name));
    } else {
        out << " movq " << get<1>(env.lookup(e->name)) << "(%rbp), %rax"<<endl;
    }
    return get<0>(env.lookup(e->name));
}

// check!
ImpType GenCodeVisitor::visit(IfExp* e) {
    int lbl = label_counter++;
    e->condition->accept(this);
    out << "  cmpq $0, %rax" << endl;
    out << "  je else_" << lbl << endl;
    ImpType imp_type = e->then->accept(this);
    out << "  jmp endif_" << lbl << endl;
    out << "else_" << lbl << ":" << endl;
    e->els->accept(this);
    out << "endif_" << lbl << ":" << endl;
    
    return imp_type;
}

ImpType GenCodeVisitor::visit(FunctionCallExp* e) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = e->argList.begin(); it != e->argList.end(); ++it) {
        (*it)->accept(this);
        out << " mov %rax, " << argRegs[i] << endl;
        ++i;
    }
    out << " call " << e->name << endl;

    return functions_info[e->name];
}

void GenCodeVisitor::visit(AssignStatement* s) {
    assert(env.check(s->name));
    ImpType imp_type = s->right->accept(this);
    bool is_struct = imp_type.ttype != "i32" && imp_type.ttype != "i64" && imp_type.ttype != "bool";
    int offset_assign = get<1>(env.lookup(s->name));
    switch (s->op) {
        case AS_ASSIGN_OP:
            if(is_struct) {
                for(auto i : structs_info[struct_name].offsets) {
                    out << " movq " << temp_offset + i.second << "(%rbp), %rax" << endl;
                    out << " movq %rax, " << + offset_assign + i.second << "(%rbp)" << endl;
                }
            } else {
                out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
            }
            break;
        case AS_PLUS_OP:
            out << " addq %rax, " << offset_assign << "(%rbp)" << endl;
            break;
        case AS_MINUS_OP:
            out << " subq %rax, " << offset_assign << "(%rbp)" << endl;
            break;
        case AS_MUL_OP: // not checked yet
            out << " movq %rax, %rcx" << endl;
            out << " movq " << offset_assign << "(%rbp), %rax" << endl;
            out << " imulq %rcx, %rax" << endl;
            out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
            break;
        case AS_DIV_OP:
            out << " movq %rax, %rcx" << endl;
            out << " movq " << offset_assign << "(%rbp), %rax" << endl;
            out << " cqto" << endl;
            out << " idivq %rcx" << endl;
            out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
            break;
        case AS_MOD_OP:
            out << " movq %rax, %rcx" << endl;
            out << " movq " << offset_assign << "(%rbp), %rax" << endl;
            out << " cqto" << endl;
            out << " idivq %rcx" << endl;
            out << " movq %rdx, " << offset_assign << "(%rbp)" << endl;
            break;
        default:
            errorHandler.error("Not assignOp supported");
    }
}

// check!
void GenCodeVisitor::visit(PrintStatement* s) {
    s->exp->accept(this);
    out <<
        " movq %rax, %rsi" << endl;
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
    out << "movq %rax, " << offset << "(%rbp)" << endl;
    int current_offset = offset;
    offset -= 8;
    s->end->accept(this);
    out << "movq %rax, " << offset << "(%rbp)" << endl;
    int end_offset = offset;
    offset -= 8;
    
    out << "for_" << lbl << ":" << endl;
    out << "movq " << current_offset << "(%rbp), %rax" << endl;
    out << "movq " << end_offset << "(%rbp), %rcx" << endl;
    out << "cmpq %rcx, %rax" << endl;
    out << "jge endfor_" << lbl << endl;
    out << "movq %rax, " << var_offset << "(%rbp)" << endl;
    out << "addq $1, %rax" << endl;
    out << "movq %rax, " << current_offset << "(%rbp)" << endl;

    s->body->accept(this);

    out << "jmp for_" << lbl << endl;
    out << "endfor_" << lbl << ":" << endl;
}

void GenCodeVisitor::visit(ReturnStatement* s) {
    if(s->exp) {
        s->exp->accept(this);
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
    bool is_struct = vd->type != "i32" && vd->type != "i64" && vd->type != "bool";
    offset -=  is_struct ? structs_info[vd->type].size : 8; // TODO depende del tipo
    if(vd->exp != nullptr) {
        vd->exp->accept(this);
        if(is_struct) {
            for(auto i : structs_info[struct_name].offsets) {
                out << " movq " << temp_offset + i.second << "(%rbp), %rax" << endl;
                out << " movq %rax, " << + offset_assign + i.second << "(%rbp)" << endl;
            }
        } else {
            out << " movq %rax, " << offset_assign << "(%rbp)" << endl;
        }
    }
}

void GenCodeVisitor::visit(FunctionCallStatement* stm) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int i = 0;
    for(auto it = stm->argList.begin(); it != stm->argList.end(); ++it) {
        (*it)->accept(this);
        out << " mov %rax, " << argRegs[i] << endl;
        ++i;
    }
    out << " call " << stm->name << endl;
}

void GenCodeVisitor::visit(ParamDec* vd) {
    ImpType type(vd->type);
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
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    
    int reserva = 80; // deberia ser calculado con las variables
    temp_offset = -reserva;
    out << " subq $" << reserva << ", %rsp" << endl;

    int size = f->params.size();
    int i = 0;
    for(auto it = f->params.begin(); it != f->params.end(); ++it){
        out << " mov " << argRegs[i] << ", " << offset << "(%rbp)" << endl;
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
        offset_struct -= 8; // TODO depende del tipo de variable
    }
    struct_info.size = -offset_struct; // TODO depende
    structs_info[s->name] = struct_info;
}

void GenCodeVisitor::visit(AttrDec* a){
    // No es necesario creo
}

ImpType GenCodeVisitor::visit(PostfixExp* e) {
    e->left->accept(this);
    out << " movq " << struct_offset + structs_info[struct_name].offsets[e->right] << "(%rbp), %rax" << endl;
    return structs_info[struct_name].types[e->right];
}

ImpType GenCodeVisitor::visit(StructExp* e) {
    int current_struct_offset = temp_offset;
    struct_name = e->name; // De repente es necesario guardarse el anterior por si hay structs dentro de structs no lo se
    struct_offset = current_struct_offset; //

    out << "subq $" << structs_info[struct_name].size << ", %rsp" << endl;
    for(auto a : e->attrs) {
        a->accept(this);
        struct_name = e->name;
        struct_offset = current_struct_offset;
    }
    return ImpType(e->name);
}

ImpType GenCodeVisitor::visit(StructExpAttr* a) {
    ImpType imp_type = a->exp->accept(this);
    out << " movq %rax, " << temp_offset + structs_info[struct_name].offsets[a->name] << "(%rbp)" << endl;
    return imp_type;
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