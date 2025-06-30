#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        ERR, //
        END, //
        PLUS, // +
        MINUS, // -
        MUL, // *
        DIV, // /
        MOD, // %
        NUM, // numero 
        PD, // )
        PI, // (
        LD, // }
        LI, // {
        ID, // string for ids
        PRINTLN, // println
        PRINT, // print
        ASSIGN, // =
        PC, // ;
        LT, // <
        LE, // <= 
        GT, // >
        GE, // >= 
        EQ, // ==
        NEQ, // !=
        IF, // if
        ELSE, // else
        WHILE, // while
        COMMA, // ,
        DOT, // .
        DOTS, //..
        LET, // let
        MUT, // mut
        FOR, // for
        TRUE, // true
        FALSE, // false
        RETURN, // return
        FN, // fn
        BREAK, // break
        CONTINUE, // continue
        IN, // in
        COLON, // :
        STRUCT, // struct
        AND, // &&
        NOT, // !
        OR, // ||
        QUOTE, // "
        ARROW, // ->
        PLUSASSIGN, // +=
        MINUSASSIGN, // -=
        MULASSIGN, // *=
        DIVASSIGN, // /=
        MODASSIGN, // %=
        BORROW // &
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