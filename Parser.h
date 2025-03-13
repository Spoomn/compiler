#pragma once
#include "Scanner.h"
#include "Symbol.h"
#include "Token.h"

class ParserClass
{
private:
    ScannerClass* mScanner;
    SymbolTableClass* mSymTab;

    TokenClass Match(TokenType expectedType);

    void Program();
    void Block();
    void StatementGroup();
    void Statement();
    void DeclarationStatement();
    void AssignmentStatement();
    void CoutStatement();
    void Expression();
    void Identifier();
    void Integer();

public:
    ParserClass(ScannerClass* scanner, SymbolTableClass* symTab);
    void Start();
};