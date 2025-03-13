#include "Parser.h"
#include <iostream>
#include <cstdlib>

ParserClass::ParserClass(ScannerClass* scanner, SymbolTableClass* symTab)
    : mScanner(scanner), mSymTab(symTab)
{
}

TokenClass ParserClass::Match(TokenType expectedType) {
    TokenClass token = mScanner->GetNextToken();
    if (token.GetTokenType() != expectedType) {
        std::cerr << "Error in ParserClass::Match. Expected token type "
                  << TokenClass::GetTokenTypeName(expectedType)
                  << ", but got type " << token.GetTokenTypeName()
                  << " with lexeme \"" << token.GetLexeme() << "\"." << std::endl;
        std::exit(1);
    }
    return token;
}

void ParserClass::Start() {
    (void)mSymTab;
    Program();
    Match(ENDFILE_TOKEN);
}

void ParserClass::Program() {
    Match(VOID_TOKEN);
    Match(MAIN_TOKEN);
    Match(LPAREN_TOKEN);
    Match(RPAREN_TOKEN);
    Block();
}

void ParserClass::Block() {
    Match(LCURLY_TOKEN);
    StatementGroup();
    Match(RCURLY_TOKEN);
}

void ParserClass::StatementGroup() {
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    while (tt == INT_TOKEN || tt == IDENTIFIER_TOKEN || tt == COUT_TOKEN || tt == LCURLY_TOKEN) {
        Statement();
        token = mScanner->PeekNextToken();
        tt = token.GetTokenType();
    }
}

void ParserClass::Statement() {
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    if (tt == INT_TOKEN) {
        DeclarationStatement();
    } else if (tt == IDENTIFIER_TOKEN) {
        AssignmentStatement();
    } else if (tt == COUT_TOKEN) {
        CoutStatement();
    } else if (tt == LCURLY_TOKEN) {
        Block();
    } else {
        std::cerr << "Error in ParserClass::Statement: unexpected token " 
                  << token.GetTokenTypeName() << " with lexeme \"" 
                  << token.GetLexeme() << "\"." << std::endl;
        std::exit(1);
    }
}

void ParserClass::DeclarationStatement() {
    Match(INT_TOKEN);
    Identifier();
    Match(SEMICOLON_TOKEN);
}

void ParserClass::AssignmentStatement() {
    Identifier();
    Match(ASSIGNMENT_TOKEN);
    Expression();
    Match(SEMICOLON_TOKEN);
}

void ParserClass::CoutStatement() {
    Match(COUT_TOKEN);
    Match(INSERTION_TOKEN);
    Expression();
    Match(SEMICOLON_TOKEN);
}

void ParserClass::Expression() {
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    if (tt == IDENTIFIER_TOKEN) {
        Identifier();
    } else if (tt == INTEGER_TOKEN) {
        Integer();
    } else {
        std::cerr << "Error in ParserClass::Expression: unexpected token " 
                  << token.GetTokenTypeName() << " with lexeme \"" 
                  << token.GetLexeme() << "\"." << std::endl;
        std::exit(1);
    }
}

void ParserClass::Identifier() {
    Match(IDENTIFIER_TOKEN);
}

void ParserClass::Integer() {
    Match(INTEGER_TOKEN);
}