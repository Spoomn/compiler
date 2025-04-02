#include "Token.h"
#include <iostream>

TokenClass::TokenClass(TokenType type, const std::string& lexeme)
    : mType(type), mLexeme(lexeme) {

    // Check for reserved words:
    if (lexeme == "void")
        mType = VOID_TOKEN;
    else if (lexeme == "main")
        mType = MAIN_TOKEN;
    else if (lexeme == "int")
        mType = INT_TOKEN;
    else if (lexeme == "cout")
        mType = COUT_TOKEN;
    else if (lexeme == "if")
        mType = IF_TOKEN;
    else if (lexeme == "while")
        mType = WHILE_TOKEN;
    else if (lexeme == "else")
        mType = ELSE_TOKEN;
    else if (lexeme == "or")
        mType = OR_TOKEN;
    else if (lexeme == "repeat")
        mType = REPEAT_TOKEN;
    
}

std::ostream& operator<<(std::ostream& out, const TokenClass& tc) {
    out << "Token(" << tc.GetTokenTypeName() << ", \"" << tc.GetLexeme() << "\")";
    return out;
}