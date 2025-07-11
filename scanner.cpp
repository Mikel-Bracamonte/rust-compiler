#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s),first(0), current(0) { 
    errorHandler = ErrorHandler("Scanner");
}


bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() && is_white_space(input[current]) ) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;
    
    int comment_depth = 0;
    if(input[current] == '/') {
        if(current + 1 < input.length() && input[current + 1] == '/') {
            while(input[current] != '\n') {
                if(current >= input.length()) {
                    return new Token(Token::END);
                }
                current++;
            }
        } else if(current + 1 < input.length() && input[current + 1] == '*') {
            comment_depth++;
            current += 2;
            while(comment_depth > 0) {
                if(current >= input.length()) {
                    return new Token(Token::END);
                }
                if(input[current] == '/' && current + 1 < input.length() && input[current + 1] == '*') {
                    comment_depth++;
                    current++;
                } else if(input[current] == '*' && current + 1 < input.length() && input[current + 1] == '/') {
                    comment_depth--;
                    current++;
                }
                current++;
            }
        }
    }

    while (current < input.length() && is_white_space(input[current]) ) current++;
    first = current;

    c = input[current];

    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUM, input, first, current - first);
    }

    else if (isalpha(c)) {
        current++;
        while (current < input.length() && (isalnum(input[current]) || input[current] == '_'))
            current++;
        string word = input.substr(first, current - first);
        if (word == "println") {
            token = new Token(Token::PRINTLN, word, 0, word.length());
        } else if (word == "print") {
            token = new Token(Token::PRINT, word, 0, word.length());
        }  else if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        } else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        } else if (word == "while") {
            token = new Token(Token::WHILE, word, 0, word.length());
        } else if (word == "for") {
            token = new Token(Token::FOR, word, 0, word.length());
        } else if (word == "let") {
            token = new Token(Token::LET, word, 0, word.length());
        } else if (word == "mut") {
            token = new Token(Token::MUT, word, 0, word.length());
        } else if (word == "true") {
            token = new Token(Token::TRUE, word, 0, word.length());
        } else if (word == "false") {
            token = new Token(Token::FALSE, word, 0, word.length());
        } else if (word == "return") {
            token = new Token(Token::RETURN, word, 0, word.length());
        } else if (word == "fn") {
            token = new Token(Token::FN, word, 0, word.length());
        } else if (word == "break") {
            token = new Token(Token::BREAK, word, 0, word.length());
        } else if (word == "continue") {
            token = new Token(Token::CONTINUE, word, 0, word.length());
        } else if (word == "in") {
            token = new Token(Token::IN, word, 0, word.length());
        } else if (word == "struct") {
            token = new Token(Token::STRUCT, word, 0, word.length());
        } else {
            token = new Token(Token::ID, word, 0, word.length());
        }
    }

    else if (strchr("+-*/()=;,<>!.:|&{}\"%", c)) {
        switch(c) {
            case '+':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::PLUSASSIGN, "+=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::PLUS, c);
                }
                break;
            case '-':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::MINUSASSIGN, "-=", 0, 2);
                    current++;
                } else if (current + 1 < input.length() && input[current + 1] == '>') {
                    token = new Token(Token::ARROW, "->", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MINUS, c);
                }
                break;
            case '*':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::MULASSIGN, "*=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MUL, c);
                }
                break;
            case '/':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::DIVASSIGN, "/=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::DIV, c);
                }
                break;
            case '%':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::MODASSIGN, "%=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MOD, c);
                }
                break;
            case ',': token = new Token(Token::COMMA, c); break;
            case '.':
                if (current + 1 < input.length() && input[current + 1] == '.') {
                    token = new Token(Token::DOTS, "..", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::DOT, c);
                }
                break;
            case '(': token = new Token(Token::PI, c); break;
            case ')': token = new Token(Token::PD, c); break;
            case '{': token = new Token(Token::LI, c); break;
            case '}': token = new Token(Token::LD, c); break;
            case '!':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::NEQ, "!=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::NOT, c);
                }
                break;
            case '|':
                if (current + 1 < input.length() && input[current + 1] == '|') {
                    token = new Token(Token::OR, "||", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ERR, c);
                }
                break;
            case '&':
                if (current + 1 < input.length() && input[current + 1] == '&') {
                    token = new Token(Token::AND, "&&", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::BORROW, c);
                }
                break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GE, ">=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GT, c);
                }
                break;
            case ';': token = new Token(Token::PC, c); break;
            case ':': token = new Token(Token::COLON, c); break;
            case '\"': token = new Token(Token::QUOTE, c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERR, c);
        }
        current++;
    }
    else {
        token = new Token(Token::ERR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { 
    errorHandler = ErrorHandler("Scanner");
}

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERR) {
            scanner->errorHandler.error("carácter inválido: " + current->text);
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}