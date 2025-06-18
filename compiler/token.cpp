#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::MOD: outs << "TOKEN(MOD)"; break;
        case Token::NUM: outs << "TOKEN(NUM)"; break;
        case Token::ERR: outs << "TOKEN(ERR)"; break;
        case Token::PD: outs << "TOKEN(PD)"; break;
        case Token::PI: outs << "TOKEN(PI)"; break;
        case Token::LI: outs << "TOKEN(LI)"; break;
        case Token::LD: outs << "TOKEN(LD)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::ID: outs << "TOKEN(ID)"; break;
        case Token::PRINTLN: outs << "TOKEN(PRINTLN)"; break;
        case Token::PRINT: outs << "TOKEN(PRINT)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::PC: outs << "TOKEN(PC)"; break;
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::GT: outs << "TOKEN(GT)"; break;
        case Token::GE: outs << "TOKEN(GE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::NEQ: outs << "TOKEN(NEQ)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;
        case Token::COMMA: outs << "TOKEN(COMMA)"; break;
        case Token::DOT: outs << "TOKEN(DOT)"; break;
        case Token::DOTS: outs << "TOKEN(DOTS)"; break;
        case Token::LET: outs << "TOKEN(LET)"; break;
        case Token::MUT: outs << "TOKEN(MUT)"; break;
        case Token::FOR : outs << "TOKEN(FOR)"; break;
        case Token::TRUE : outs << "TOKEN(TRUE)"; break;
        case Token::FALSE : outs << "TOKEN(FALSE)"; break;
        case Token::RETURN : outs << "TOKEN(RETURN)"; break;
        case Token::FN : outs << "TOKEN(FN)"; break;
        case Token::BREAK : outs << "TOKEN(BREAK)"; break;
        case Token::CONTINUE : outs << "TOKEN(CONTINUE)"; break;
        case Token::IN : outs << "TOKEN(IN)"; break;
        case Token::COLON : outs << "TOKEN(COLON)"; break;
        case Token::STRUCT : outs << "TOKEN(STRUCT)"; break;
        case Token::AND : outs << "TOKEN(AND)"; break;
        case Token::OR : outs << "TOKEN(OR)"; break;
        case Token::NOT : outs << "TOKEN(NOT)"; break;
        case Token::QUOTE : outs << "TOKEN(QUOTE)"; break;
        case Token::ARROW : outs << "TOKEN(ARROW)"; break;
        case Token::PLUSASSIGN : outs << "TOKEN(PLUSASSIGN)"; break;
        case Token::MINUSASSIGN : outs << "TOKEN(MINUSASSIGN)"; break;
        case Token::MULASSIGN : outs << "TOKEN(MULASSIGN)"; break;
        case Token::DIVASSIGN : outs << "TOKEN(DIVASSIGN)"; break;
        case Token::MODASSIGN : outs << "TOKEN(MODASSIGN)"; break;
        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}