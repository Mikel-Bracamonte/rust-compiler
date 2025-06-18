#include "gencode.h"

ImpValue BinaryExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue UnaryExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue NumberExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue BoolExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue IdentifierExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue IfExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

ImpValue FunctionCallExp::accept(ImpValueVisitor* visitor) {
    return visitor->visit(this);
}

void AssignStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void PrintStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void IfStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void WhileStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void ForStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void ReturnStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void VarDec::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void FunctionCallStatement::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void ParamDec::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void FunDec::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

void StatementList::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}
void Body::accept(ImpValueVisitor* visitor) {
    visitor->visit(this);
}

//////////////////////////////////////////////////////////////////////////////////

void GenCodeVisitor::gencode(Program* p) {
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

ImpValue GenCodeVisitor::visit(BinaryExp* e) {
    /*
    ImpValue result;
    e->left->accept(this);        
    cout << "  pushq %rax" << endl;
    e->right->accept(this);       
    cout << "  movq %rax, %rcx" << endl; 
    cout << "  popq %rax" << endl;    

    switch (e->op) {
        case PLUS_OP:
            cout << "  addq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case MINUS_OP:
            cout << "  subq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case MUL_OP:
            cout << "  imulq %rcx, %rax" << endl;
            result.type = TINT;
            break;

        case DIV_OP:
            cout << "  cqto" << endl;
            cout << "  idivq %rcx" << endl;
            result.type = TINT;
            break;

        case LT_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setl %al" << endl;
            result.type = TBOOL;
            break;

        case LE_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  setle %al" << endl;
            result.type = TBOOL;
            break;

        case EQ_OP:
            cout << "  cmpq %rcx, %rax" << endl;
            cout << "  movl $0, %eax" << endl;
            cout << "  sete %al" << endl;
            result.type = TBOOL;
            break;

        default:
            cerr << "Operador binario no soportado." << endl;
            exit(1);
    }

    return result;
    */
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(UnaryExp* e) {
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(NumberExp* e) {
    /*
    ImpValue v;
    v.set_default_value(TINT);
    v.int_value = e->value;
    cout << "  movq $" << e->value << ", %rax" << endl;
    return v;
    */
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(BoolExp* e) {
    /*
    ImpValue v;
    v.set_default_value(TBOOL);
    v.bool_value = e->value;
    cout << "  movq $" << (e->value ? 1 : 0) << ", %rax" << endl;
    return v;
    */
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(IdentifierExp* e) {
    /*
    if (env.check(e->name)) {
        cout << "  movq " << stack_offsets[e->name] << "(%rbp), %rax" << endl;
        return env.lookup(e->name);
    } else {
        cerr << "Error: variable " << e->name << " no declarada" << endl;
        exit(1);
    }
    */
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(IfExp* e) {
    return ImpValue();
}

ImpValue GenCodeVisitor::visit(FunctionCallExp* e) {
    return ImpValue();
}

void GenCodeVisitor::visit(AssignStatement* s) {
    /*
    ImpValue val = s->rhs->accept(this); 
    cout << "  movq %rax, " << stack_offsets[s->id] << "(%rbp)" << endl;
    env.update(s->id, val);  
    */
}

void GenCodeVisitor::visit(PrintStatement* s) {
    /*
    s->e->accept(this);
    cout << "  movq %rax, %rsi" << endl;
    cout << "  leaq print_fmt(%rip), %rdi" << endl;
    cout << "  movl $0, %eax" << endl;
    cout << "  call printf@PLT" << endl;
    */
}

void GenCodeVisitor::visit(IfStatement* s) {
    /*
    int lbl = etiquetas++;
    s->condition->accept(this);
    cout << "  cmpq $0, %rax" << endl;
    cout << "  je else_" << lbl << endl;
    s->then->accept(this);
    cout << "  jmp endif_" << lbl << endl;
    cout << "else_" << lbl << ":" << endl;
    if (s->els) s->els->accept(this);
    cout << "endif_" << lbl << ":" << endl;
    */
}

void GenCodeVisitor::visit(WhileStatement* s) {
    /*
    int lbl_cond = etiquetas++;
    int lbl_end  = etiquetas++;
    cout << "while_" << lbl_cond << ":" << endl;
    s->condition->accept(this);
    cout << "  testq %rax, %rax" << endl;
    cout << "  je endwhile_" << lbl_end << endl;
    s->b->accept(this);
    cout << "  jmp while_" << lbl_cond << endl;
    cout << "endwhile_" << lbl_end << ":" << endl;
    */
}

void GenCodeVisitor::visit(ForStatement* s) {
    
}

void GenCodeVisitor::visit(ReturnStatement* s) {
    
}

void GenCodeVisitor::visit(VarDec* vd) {
    /*
    ImpVType tt = ImpValue::get_basic_type(vd->type);
    for (const auto& var : vd->vars) {
        ImpValue v;
        v.set_default_value(tt);
        env.add_var(var, v);
        current_offset -= 8;
        stack_offsets[var] = current_offset;
    }
    */
}

void GenCodeVisitor::visit(FunctionCallStatement* stm) {
    
}

void GenCodeVisitor::visit(ParamDec* vd) {

}

void GenCodeVisitor::visit(FunDec* vd) {

}

void GenCodeVisitor::visit(StatementList* s) {
    /*
    for (auto it : s->stms) {
        it->accept(this);
    }
    */
}

void GenCodeVisitor::visit(Body* b) {
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

void CheckVisitor::check(Program* p) {
    /*
    */
}