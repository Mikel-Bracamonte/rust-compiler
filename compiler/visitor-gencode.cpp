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

void StatementList::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}
void Body::accept(ImpVisitor* visitor) {
    visitor->visit(this);
}

//////////////////////////////////////////////////////////////////////////////////

void GenCodeVisitor::gencode(Program* p) {
    out << ".data" << endl;
    out << "print_fmt: .string \"%ld \\n\""<<endl;
    out << ".text" << endl;

    for(auto f: p->funs){
        f->accept(this);
    }

    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;

    /*
    env.clear();
    label_counter = 0;
    offset = 0;

    cout << ".data" << endl;
    cout << "print_fmt: .string \"%ld\\n\"" << endl;
    cout << ".text" << endl;
    cout << ".globl main" << endl;
    cout << "main:" << endl;
    cout << "  pushq %rbp" << endl;
    cout << "  movq %rsp, %rbp" << endl;
    p->accept(this);  
    cout << "  movl $0, %eax" << endl;
    cout << "  leave" << endl;
    cout << "  ret" << endl;
    cout << ".section .note.GNU-stack,\"\",@progbits" << endl;
    */
}



ImpType GenCodeVisitor::visit(BinaryExp* e) {
    e->left->accept(this);
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
            break;
        case LE_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " setle %al" << endl
                << " movzbq %al, %rax" << endl;
            break;
        case EQ_OP:
            out << " cmpq %rcx, %rax" << endl
                << " movl $0, %eax" << endl
                << " sete %al" << endl
                << " movzbq %al, %rax" << endl;
            break;
        case GT_OP:
            break;
        case GE_OP:
            break;
        case NEQ_OP:
            break;
        case MOD_OP:
            break;
        case AND_OP:
            break;
        case OR_OP:
            break;
        default:
            errorHandler.error("Not binaryOp supported");
    }
    return ImpType();
}

ImpType GenCodeVisitor::visit(UnaryExp* e) {
    e->exp->accept(this);
    switch (e->op) {
        case U_NEG_OP:
            out << " neg %rax" << endl; 
            break;
        case U_NOT_OP:
            // TODO 
            break;
        default:
            errorHandler.error("Not unaryOp supported");
    }
    return ImpType();
}

// check!
ImpType GenCodeVisitor::visit(NumberExp* e) {
    out << " movq $" << e->value << ", %rax"<<endl;
    return ImpType();
}

// check!
ImpType GenCodeVisitor::visit(BoolExp* e) {
    out << " movq $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return ImpType();
}

// seems check!
ImpType GenCodeVisitor::visit(IdentifierExp* e) {
    assert(env.check(e->name));
    out << " movq " << get<1>(env.lookup(e->name)) << "(%rbp), %rax"<<endl;
    return ImpType();
}

// check!
ImpType GenCodeVisitor::visit(IfExp* e) {
    int lbl = label_counter++;
    e->condition->accept(this);
    out << "  cmpq $0, %rax" << endl;
    out << "  je else_" << lbl << endl;
    e->then->accept(this);
    out << "  jmp endif_" << lbl << endl;
    out << "else_" << lbl << ":" << endl;
    e->els->accept(this);
    out << "endif_" << lbl << ":" << endl;
    
    return ImpType();
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

    return ImpType();
}

void GenCodeVisitor::visit(AssignStatement* s) {
    assert(env.check(s->name));
    s->right->accept(this);
    out << " movq %rax, " << get<1>(env.lookup(s->name)) << "(%rbp)" << endl;
    /*
    ImpType val = s->rhs->accept(this); 
    cout << "  movq %rax, " << stack_offsets[s->id] << "(%rbp)" << endl;
    env.update(s->id, val);  
    */
}

// check!
void GenCodeVisitor::visit(PrintStatement* s) {
    s->exp->accept(this);
    out <<
        " movq %rax, %rsi" << endl
        << " leaq print_fmt(%rip), %rdi" << endl
        << " movl $0, %eax" << endl
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
    out << "while_" << lbl << ":" << endl;
    s->condition->accept(this);
    out << "  testq %rax, %rax" << endl; 
    out << "  je endwhile_" << lbl << endl;
    s->body->accept(this);
    out << "  jmp while_" << lbl << endl;
    out << "endwhile_" << lbl << ":" << endl;
}

void GenCodeVisitor::visit(ForStatement* s) {
    int lbl = label_counter++;
    string var = s->name;

    ImpType type;
    type.set_basic_type("i32");
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
    s->exp->accept(this);
    out << "jmp .end_" << nombreFuncion << endl;
}

void GenCodeVisitor::visit(BreakStatement* s) {
    
}

void GenCodeVisitor::visit(ContinueStatement* s) {
    
}

// seems check!
void GenCodeVisitor::visit(VarDec* vd) {
    ImpType type;
    type.set_basic_type(vd->type);
    env.add_var(vd->name, {type, offset});
    offset -= 8;
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
    ImpType type;
    type.set_basic_type(vd->type);
    env.add_var(vd->name, {type, offset});
    offset -= 8;
}

void GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    env.clear();
    env.add_level();
    offset = -8;
    nombreFuncion = f->name;

    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    
    out << ".globl " << f->name << endl;
    out << f->name << ":" << endl;
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    
    int reserva = 80; // deberia ser calculado con las variables
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

void GenCodeVisitor::visit(StatementList* s) {
    for (auto it : s->stms) {
        it->accept(this);
    }
}

void GenCodeVisitor::visit(Body* b) {
    env.add_level();
    b->stmList->accept(this);
    env.remove_level();
    /*
    env.add_level();
    long old_offset = offset;
    b->vardecs->accept(this);
    long locals_size = -current_offset;
    if (locals_size > 0)
        cout << "  subq $" << locals_size << ", %rsp" << endl;

    b->slist->accept(this);
    current_offset = old_offset;
    env.remove_level();
    */
}

////////////////////////////////////////////////////