#pragma once
#include "Scanner.h"
#include "Symbol.h"
#include "Token.h"
#include "Node.h"

class ParserClass
{
private:
    ScannerClass* mScanner;
    SymbolTableClass* mSymTab;

    TokenClass Match(TokenType expectedType);
    
    ProgramNode* Program();
    BlockNode* Block();
    StatementGroupNode* StatementGroup();
    StatementNode* Statement();

    DeclarationStatementNode* DeclarationStatement();
    AssignmentStatementNode* AssignmentStatement();
    CoutStatementNode* CoutStatement();
    ExpressionNode* Expression();
    IdentifierNode* Identifier();
    IntegerNode* Integer();
    
    ExpressionNode* Relational();

    ExpressionNode* PlusMinus();
    ExpressionNode* TimesDivide();
    ExpressionNode* Factor();

    ExpressionNode* And();
    ExpressionNode* Or();

    StatementNode* IfStatement();
    StatementNode* WhileStatement();
    StatementNode* RepeatStatement();

public:
    ParserClass(ScannerClass* scanner, SymbolTableClass* symTab);
    StartNode* Start();
};
