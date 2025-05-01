#pragma once
#include <string>

enum TokenType {
	// Reserved Words:
	VOID_TOKEN, MAIN_TOKEN, INT_TOKEN, COUT_TOKEN, ENDL_TOKEN, IF_TOKEN, WHILE_TOKEN, ELSE_TOKEN, FOR_TOKEN, REPEAT_TOKEN, DO_TOKEN,
	// Relational Operators:
	LESS_TOKEN, LESSEQUAL_TOKEN, GREATER_TOKEN, GREATEREQUAL_TOKEN, EQUAL_TOKEN, NOTEQUAL_TOKEN, MOD_TOKEN,
	// Other Operators:
	INSERTION_TOKEN, ASSIGNMENT_TOKEN, PLUS_EQUAL_TOKEN, MINUS_EQUAL_TOKEN, PLUS_TOKEN, MINUS_TOKEN, TIMES_TOKEN, DIVIDE_TOKEN,
	 PLUS_PLUS_TOKEN, MINUS_MINUS_TOKEN, POWER_TOKEN,
	// Logical Operators:
	AND_TOKEN, OR_TOKEN,
	// Other Characters:
	SEMICOLON_TOKEN, LPAREN_TOKEN, RPAREN_TOKEN, LCURLY_TOKEN, RCURLY_TOKEN,
	// Other Token Types:
	IDENTIFIER_TOKEN, INTEGER_TOKEN, BAD_TOKEN, ENDFILE_TOKEN,


};
// IMPORTANT: The list above and the list below MUST be kept in sync.
const std::string gTokenTypeNames[] = {
	// Reserved Words:
	"VOID", "MAIN", "INT", "COUT", "ENDL", "IF", "WHILE", "ELSE", "FOR", "REPEAT", "DO",
	// Relational Operators:
	"LESS", "LESSEQUAL", "GREATER", "GREATEREQUAL", "EQUAL", "NOTEQUAL", "MOD",
	// Other Operators:
	"INSERTION", "ASSIGNMENT", "PLUS_EQUAL", "MINUS_EQUAL", "PLUS", "MINUS", "TIMES", "DIVIDE",
	 "PLUS_PLUS", "MINUS_MINUS", "POWER",
	// Logical Operators:
	"AND", "OR",
	// Other Characters:
	"SEMICOLON", "LPAREN", "RPAREN", "LCURLY", "RCURLY",
	// Other Token Types:
	"IDENTIFIER", "INTEGER", "BAD", "ENDFILE",
	};

class TokenClass
{
	private:
		TokenType mType;
		std::string mLexeme;
	public:
		TokenClass(TokenType type, const std::string& lexeme);
		TokenType GetTokenType() const { return mType; }
		const std::string& GetTokenTypeName() const
		{
			return gTokenTypeNames[mType];
		}
		std::string GetLexeme() const { return mLexeme; }
		static const std::string& GetTokenTypeName(TokenType type)
		{
			return gTokenTypeNames[type];
	}
};

std::ostream & operator<<(std::ostream & out, const TokenClass & tc);
