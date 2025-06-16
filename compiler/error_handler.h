#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <iostream>
#include <stdexcept>
#include <string>
#include "token.h"
#include <sstream>

class ErrorHandler {
private:
    std::string moduleName;

public:
    ErrorHandler() {}
    ErrorHandler(std::string name) : moduleName(name) {}

    // Muestra y lanza error si el token actual no es el esperado
    void expect(const Token& expected, const Token& bef, const Token& found) {
        if (found.type != expected.type) {
            std::ostringstream oss;
            oss << "[" << moduleName << "] Error: se esperaba: "
                << expected << " después de: "
                << bef << ", pero se encontró: "
                << found;
            throw std::runtime_error(oss.str());
        }
    }

    void expect(const Token& expected, const Token& bef, std::string found) {
        std::ostringstream oss;
        oss << "[" << moduleName << "] Error: se esperaba: "
            << expected << " después de: "
            << bef << ", pero se encontró: "
            << found;
        throw std::runtime_error(oss.str());
    }

    // Solo imprime un mensaje de error contextual y lanza excepción
    void error(const std::string& msg) const {
        std::ostringstream oss;
        oss << "[" << moduleName << "] Error: " << msg;
        throw std::runtime_error(oss.str());
    }
};

#endif
