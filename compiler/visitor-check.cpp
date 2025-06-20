#include "imp_visitor.h"

void CheckVisitor::check(Program* p) {
    /*
    */
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

}

void CheckVisitor::visit(PrintStatement* s) {

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

}

void CheckVisitor::visit(FunctionCallStatement* stm) {
    
}

void CheckVisitor::visit(ParamDec* vd) {

}

void CheckVisitor::visit(FunDec* vd) {

}

void CheckVisitor::visit(StatementList* s) {

}

void CheckVisitor::visit(Body* b) {

}
