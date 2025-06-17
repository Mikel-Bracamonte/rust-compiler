#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
//#include "visitor.h"
//#include "gencode.h"
//#include "imp_type.h"

using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);
    cout << "Scanner exitoso" << endl;
    
    cout << endl;
    cout << "Iniciando parsing:" << endl;
    Parser parser(&scanner); 
    try {
        Exp* exp = parser.parseAExp();
        cout << "Parsing exitoso" << endl << endl;
        cout << "Iniciando Visitor:" << endl;
        PrintVisitor* printVisitor = new PrintVisitor();
        //ImpCODE interpreter;
        cout << endl;
        cout << "IMPRIMIR:" << endl;
        exp->accept(printVisitor);
        //cout  << endl;
        //cout << endl << "Run program:" << endl;
        //interpreter.interpret(program);
        //cout << "End of program execution" << endl;
        delete exp;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }
    

    return 0;
}