#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        ERR, END, PLUS, MINUS, MUL, DIV, MOD, NUM, PD, PI, LD, LI, ID, PRINTLN, ASSIGN, PC, 
        LT, LE, GT, GE, EQ, NEQ, IF, ELSE, WHILE, COMMA, DOT, LET, MUT, FOR, TRUE,
        FALSE, RETURN, FN, BREAK, CONTINUE, IN, COLON, STRUCT, AND, NOT, OR, QUOTE,
        ARROW, PLUSASSIGN, MINUSASSIGN, MULASSIGN, DIVASSIGN, MODASSIGN
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H