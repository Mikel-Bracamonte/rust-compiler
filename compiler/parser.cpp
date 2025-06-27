#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    errorHandler = ErrorHandler("Parser");
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

// check!
Program* Parser::parseProgram() {
    Program* p = new Program();
    // TODO parseStruct
    if(match(Token::STRUCT)) {
        while(match(Token::STRUCT)) {
            p->structs.push_back(parseStructDec());
        }
        return p;
    }
    else if(match(Token::FN)) {
        p->funs.push_back(parseFunDec());
        while(match(Token::FN)) {
            p->funs.push_back(parseFunDec());
        }
        return p;
    } else{
        errorHandler.error("Se esperaba struct o fun");
        return p;
    }

}

// TODO
StructDec* Parser::parseStructDec() {
    if(!match(Token::STRUCT)){
        errorHandler.expect(Token::STRUCT, current->text);
    }
    if(!match(Token::ID)){
        errorHandler.expect(Token::ID, current->text);
    }
    StructDec* s = new StructDec();
    s->name = previous->text;
    if(!match(Token::PI)){
        errorHandler.expect(Token::PI, current->text);
    }
    do {
        if (!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text); // nombre del campo
        }
        string name = previous->text;
        if (!match(Token::COLON)) {
            errorHandler.expect(Token::COLON, current->text);
        }
        if (!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text); // tipo del campo
        }
        string attrType = previous->text;
        s->attrs.push_back(new AttrDec(name, attrType));

    } while (match(Token::COMMA));

    if(!match(Token::LD)){
        errorHandler.expect(Token::LD, current->text);
    }
    return s;
    // TODO AttrDec()
}





// check!
FunDec* Parser::parseFunDec() {
    FunDec* e = new FunDec();
    if(!match(Token::ID)) {
        errorHandler.expect(Token::ID, current->text);
    }
    e->name = previous->text == "main" ? previous->text : "fn_" + previous->text;
    if(!match(Token::PI)) {
        errorHandler.expect(Token::PI, current->text);
    }
    if(!match(Token::PD)) {
        e->params.push_back(parseParamDec());
        while(match(Token::COMMA)) {
            e->params.push_back(parseParamDec());
        }
        if(!match(Token::PD)) {
            errorHandler.expect(Token::PD, current->text);
        }
    }
    if(match(Token::ARROW)) {
        if(!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text);
        }
        e->type = previous->text;
    }
    if(!match(Token::LI)) {
        errorHandler.expect(Token::LI, current->text);
    }
    e->body = parseBody();
    if(!match(Token::LD)) {
        errorHandler.expect(Token::LD, current->text);
    }
    return e;
}

ParamDec* Parser::parseParamDec() {
    if(!match(Token::ID)) {
        errorHandler.expect(Token::ID, current->text);
    }
    string name = previous->text;
    bool mut = false;
    if(!match(Token::COLON)) {
        errorHandler.expect(Token::COLON, current->text);
    }
    if(match(Token::MUT)) {
        mut = true;
    }
    if(!match(Token::ID)) {
        errorHandler.expect(Token::ID, current->text);
    }
    string type = previous->text;
    return new ParamDec(name, type, mut);
}

// check!
Body* Parser::parseBody() {
    StatementList* sl = parseStatementList();
    return new Body(sl);
}

// check!
StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    Stm* s = parseStatement();
    if(!s) {
        errorHandler.error("Expected statement");
    }
    sl->add(s);
    s = parseStatement();
    while(s) {
        sl->add(s);
        s = parseStatement();
    }
    return sl;
}

// check!
Stm* Parser::parseStatement() {
    if(match(Token::ID)) {
        string name = previous->text;
        if(match(Token::PI)) {
            FunctionCallStatement* stmt = new FunctionCallStatement();
            stmt->name = name == "main" ? name : "fn_" + name;
            if(!match(Token::PD)) {
                do {
                    stmt->add(parseAExp());
                } while (match(Token::COMMA));
                if(!match(Token::PD)) {
                    errorHandler.expect(Token::PD, current->text);
                }
            }
            if(!match(Token::PC)) {
                errorHandler.expect(Token::PC, current->text);
            }
            return stmt;
        }
        else if(match(Token::ASSIGN) || match(Token::PLUSASSIGN) || match(Token::MINUSASSIGN) || match(Token::MULASSIGN)
        || match(Token::DIVASSIGN) || match(Token::MODASSIGN)) {
            AssignOp op;
            switch(previous->type) {
                case Token::ASSIGN:
                    op = AS_ASSIGN_OP;
                    break;
                case Token::PLUSASSIGN:
                    op = AS_PLUS_OP;
                    break;
                case Token::MINUSASSIGN:
                    op = AS_MINUS_OP;
                    break;
                case Token::MULASSIGN:
                    op = AS_MUL_OP;
                    break;
                case Token::DIVASSIGN:
                    op = AS_DIV_OP;
                    break;
                case Token::MODASSIGN:
                    op = AS_MOD_OP;
                    break;
            }
            Exp* exp = parseAExp();
            if(!match(Token::PC)) {
                errorHandler.expect(Token::PC, current->text);
            }
            return new AssignStatement(name, exp, op);
        } else {
            errorHandler.error("Expected assign");
        }
    } else if(match(Token::PRINTLN) || match(Token::PRINT)) {
        bool ln = false;
        if(previous->type == Token::PRINTLN) {
            ln = true;
        }
        if(!match(Token::NOT)) {
            errorHandler.expect(Token::NOT, current->text);
        }
        if(!match(Token::PI)) {
            errorHandler.expect(Token::PI, current->text);
        }
        if(!match(Token::QUOTE)) {
            errorHandler.expect(Token::QUOTE, current->text);
        }
        if(!match(Token::LI)) {
            errorHandler.expect(Token::LI, current->text);
        }
        if(!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        if(!match(Token::QUOTE)) {
            errorHandler.expect(Token::QUOTE, current->text);
        }
        if(!match(Token::COMMA)) {
            errorHandler.expect(Token::COMMA, current->text);
        }
        Exp* exp = parseAExp();
        if(!match(Token::PD)) {
            errorHandler.expect(Token::PD, current->text);
        }
        if(!match(Token::PC)) {
            errorHandler.expect(Token::PC, current->text);
        }
        return new PrintStatement(exp, ln);
    } else if(match(Token::IF)) {
        Exp* condition = parseAExp();
        if(!match(Token::LI)) {
            errorHandler.expect(Token::LI, current->text);
        }
        Body* then = parseBody();
        if(!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        Body* els = nullptr;
        if(match(Token::ELSE)) {
            if(!match(Token::LI)) {
                errorHandler.expect(Token::LI, current->text);
            }
            els = parseBody();
            if(!match(Token::LD)) {
                errorHandler.expect(Token::LD, current->text);
            }
        }
        return new IfStatement(condition, then, els);
    } else if(match(Token::WHILE)) {
        Exp* condition = parseAExp();
        if(!match(Token::LI)) {
            errorHandler.expect(Token::LI, current->text);
        }
        Body* body = parseBody();
        if(!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        return new WhileStatement(condition, body);
    } else if(match(Token::FOR)) {
        bool mut = false;
        if(match(Token::MUT)) {
            mut = true;
        }
        if(!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text);
        }
        string name = previous->text;
        if(!match(Token::IN)) {
            errorHandler.expect(Token::IN, current->text);
        }
        Exp* start = parseAExp();
        if(!match(Token::DOTS)) {
            errorHandler.expect(Token::DOTS, current->text);
        }
        Exp* end = parseAExp();
        if(!match(Token::LI)) {
            errorHandler.expect(Token::LI, current->text);
        }
        Body* body = parseBody();
        if(!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        return new ForStatement(mut, name, start, end, body);
    } else if(match(Token::RETURN)) {
        Exp* exp = nullptr;
        if(!match(Token::PC)) {
            exp = parseAExp();
            if(!match(Token::PC)) {
                errorHandler.expect(Token::PC, current->text);
            }
        }
        return new ReturnStatement(exp);
    } else if(match(Token::BREAK)) {
        if(!match(Token::PC)) {
            errorHandler.expect(Token::PC, current->text);
        }
        return new BreakStatement();
    } else if(match(Token::CONTINUE)) {
        if(!match(Token::PC)) {
            errorHandler.expect(Token::PC, current->text);
        }
        return new ContinueStatement();
    } else if(match(Token::LET)) {
        bool mut = false;
        if(match(Token::MUT)) {
            mut = true;
        }
        if(!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text);
        }
        string name = previous->text;
        if(!match(Token::COLON)) {
            errorHandler.expect(Token::COLON, current->text);
        }
        if(!match(Token::ID)) {
            errorHandler.expect(Token::ID, current->text);
        }
        string type = previous->text;
        Exp* exp = nullptr;
        if(match(Token::ASSIGN)) {
            exp = parseAExp();
        }
        if(!match(Token::PC)) {
            errorHandler.expect(Token::PC, current->text);
        }
        return new VarDec(name, type, mut, exp);
    } 
    return nullptr;
}

//check!
Exp* Parser::parseAExp(){
    Exp* left = parseBExp();
    while (match(Token::AND) || match(Token::OR)) {
        BinaryOp op;
        if (previous->type == Token::AND){
            op = AND_OP;
        }
        else if (previous->type == Token::OR){
            op = OR_OP;
        }
        Exp* right = parseBExp();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

//check!
Exp* Parser::parseBExp(){
    if (match(Token::NOT)){
        return new UnaryExp(parseCExp(), UnaryOp::U_NOT_OP);
    }
    return parseCExp();
}

//check!
Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::GT) || match(Token::GE) || match(Token::EQ) || match(Token::NEQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::GT){
            op = GT_OP;
        }
        else if (previous->type == Token::GE){
            op = GE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        else if (previous->type == Token::NEQ){
            op = NEQ_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

//check!
Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

//check!
Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV) || match(Token::MOD)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        else if (previous->type == Token::MOD){
            op = MOD_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

// check!
Exp* Parser::parseFactor() {
    Exp* e = NULL;
    bool isUnary = false;
    UnaryOp op;
    if (match(Token::MINUS)) {
        isUnary = true;
        op = U_NEG_OP;
    }
    if (match(Token::ID)) {
        string texto = previous->text;
        if(match(Token::PI)) {
            FunctionCallExp* f = new FunctionCallExp();
            f->name = texto == "main" ? texto : "fn_" + texto;
            if(match(Token::PD)) {
                e = f;
            }
            else {
                do {
                    f->add(parseAExp());
                } while (match(Token::COMMA));
                if(!match(Token::PD)) {
                    errorHandler.expect(Token::PD, current->text);
                }
                e = f;
            }
        } // else if TODO struct exp
        else if (match(Token::LI)) {
            StructExp* s = new StructExp();
            s->name = texto;

            if (!check(Token::LD)) {
                do {
                    if (!match(Token::ID)) {
                        errorHandler.expect(Token::ID, current->text);
                    }
                    string attrName = previous->text;
                    if (!match(Token::COLON)) {
                        errorHandler.expect(Token::COLON, current->text);
                    }
                    Exp* value = parseAExp();

                    s->attrs.push_back(new StructExpAttr(attrName, value));
                } while (match(Token::COMMA));
            }
            if (!match(Token::LD)) {
                errorHandler.expect(Token::LD, current->text);
            }
            e = s;
        }
        else e = new IdentifierExp(texto);
    }
    else if (match(Token::NUM)) {
        e = new NumberExp(stoi(previous->text));
    }
    else if (match(Token::TRUE)) {
        e = new BoolExp(1);
    }
    else if (match(Token::FALSE)) {
        e = new BoolExp(0);
    }
    else if (match(Token::PI)) {
        e = parseAExp();
        e->hasParenthesis = true;
        if (!match(Token::PD)) {
            errorHandler.expect(Token::PD, current->text);
        }
    }
    else if (match(Token::IF)) {
        Exp* e1 = parseAExp();
        if (!match(Token::LI)) {
            errorHandler.expect(Token::LI, current->text);
        }
        Exp* e2 = parseAExp();
        if (!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        if (!match(Token::ELSE)) {
            errorHandler.expect(Token::ELSE, Token::LD, current->text);
        }
        if (!match(Token::LI)) {
            errorHandler.expect(Token::LI, Token::ELSE, current->text);
        }
        Exp* e3 = parseAExp();
        if (!match(Token::LD)) {
            errorHandler.expect(Token::LD, current->text);
        }
        e = new IfExp(e1, e2, e3);
    }
    if (e == NULL) {
        errorHandler.error("Se esperaba un número o identificador.");
    }
    if(isUnary){
        return new UnaryExp(e, op);
    }
    else {
        return e;
    }
}


