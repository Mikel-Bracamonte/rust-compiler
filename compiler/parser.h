#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"
#include "error_handler.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
public:
    ErrorHandler errorHandler;
    Parser(Scanner* scanner);
    Program* parseProgram();
    StructDec* parseStructDec();
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