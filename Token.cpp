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
}

std::ostream& operator<<(std::ostream& out, const TokenClass& tc) {
    out << "Token(" << tc.GetTokenTypeName() << ", \"" << tc.GetLexeme() << "\")";
    return out;
}