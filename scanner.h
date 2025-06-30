#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"
#include "error_handler.h"
class Scanner {
private:
    std::string input;
    int first, current;
public:
    ErrorHandler errorHandler;
    Scanner(const char* in_s);
    Token* nextToken();
    void reset();
    ~Scanner();
};

void test_scanner(Scanner* scanner);

#endif // SCANNER_H