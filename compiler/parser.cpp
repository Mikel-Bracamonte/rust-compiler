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

// parse funcs
Program* Parser::parseProgram() {
    Program* p = NULL;
    return p;
}

FunDec* Parser::parseFunDec() {
    FunDec* e = NULL;
    return e;
}

ParamDec* Parser::parseParamDec() {
    ParamDec* e = NULL;
    return e;
}

// check '}'
Body* Parser::parseBody() {
    StatementList* sl = parseStatementList();
    // assert (previous->token == Token::LD )
    return new Body(sl);
}

// while !match(Token::LD)
StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    sl->add(parseStatement());
    while (match(Token::PC)) {
        sl->add(parseStatement());
    }
    return sl;
}

// do again all
Stm* Parser::parseStatement() {
    
    Stm* s = NULL;
    /*
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case

    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (match(Token::ASSIGN)) {
            e = parseCExp();
            s = new AssignStatement(lex, e);
        }

    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }
    else if (match(Token::IF)) {
        e = parseCExp();
        if (!match(Token::THEN)) {
            cout << "Error: se esperaba 'then' después de la expresión." << endl;
            exit(1);
        }
        
        tb = parseBody();

        if (match(Token::ELSE)) {
            fb = parseBody();
        }
        if (!match(Token::ENDIF)) {
            cout << "Error: se esperaba 'end' al final de la declaración." << endl;
            exit(1);
        }
        s = new IfStatement(e, tb, fb);

    }
    else if (match(Token::WHILE)) {
        e = parseCExp();
        if (!match(Token::DO)) {
            cout << "Error: se esperaba 'do' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::ENDWHILE)) {
            cout << "Error: se esperaba 'endwhile' al final de la declaración." << endl;
            exit(1);
        }
        s = new WhileStatement(e, tb);

    }
    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    */
    return s;
}

// make sense: a && b || c && d?
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

// checked!
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

// add %
// check order, make sense: a % b % c * d / e?
Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
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
            f->name = texto;
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
