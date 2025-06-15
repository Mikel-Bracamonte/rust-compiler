#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"
#include "error_handler.h"

class Parser {
private:
    ErrorHandler error_handler;
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
public:
    Parser(Scanner* scanner);
    Program* parseProgram();
    FunDec* parseFunDec();
    ParamDec* parseParamDec();
    Body* parseBody();
    StatementList* parseStatementList();
    Stm* parseStatement();
    Exp* parseAExp();
    Exp* parseBExp();
    Exp* parseCExp();
    Exp* parseExpression();
    Exp* parseTerm();
    Exp* parseFactor();
};

#endif // PARSER_H