#pragma once
#include "Token.h"

enum MachineState {
	START_STATE, IDENTIFIER_STATE, INTEGER_STATE, CANTMOVE_STATE, 
    LESS_STATE, LESSEQUAL_STATE, GREATER_STATE, GREATEREQUAL_STATE, EXCLAMATION_STATE,
    PLUS_STATE, MINUS_STATE, TIMES_STATE, DIVIDE_STATE,
    SEMICOLON_STATE, EQUAL_STATE, NOTEQUAL_STATE, INSERTION_STATE, ENDFILE_STATE,
    LPAREN_STATE, RPAREN_STATE, LCURLY_STATE, RCURLY_STATE, 
    POSSIBLE_COMMENT_STATE, BLOCK_COMMENT_STATE, BLOCK_COMMENT_END_STATE, LINE_COMMENT_STATE,
	LAST_STATE,
	};

enum CharacterType {
	LETTER_CHAR, DIGIT_CHAR, WHITESPACE_CHAR, RETURN_CHAR,
    LESS_CHAR, GREATER_CHAR, EXCLAMATION_CHAR,
    PLUS_CHAR, MINUS_CHAR, SLASH_CHAR, ASTERISK_CHAR,
    SEMICOLON_CHAR, EQUAL_CHAR, BAD_CHAR, ENDFILE_CHAR,
    LPAREN_CHAR, RPAREN_CHAR, LCURLY_CHAR, RCURLY_CHAR,
	LAST_CHAR,
	};

class StateMachineClass {
    public:
        StateMachineClass();
        MachineState UpdateState(char currentCharacter, TokenType & previousTokenType);

    private:
        MachineState mCurrentState;

        // The matrix of legal moves:
        MachineState mLegalMoves[LAST_STATE][LAST_CHAR];

        // Which end-machine-states correspond to which token types.
        // (non end states correspond to the BAD_TOKEN token type)
        TokenType mCorrespondingTokenTypes[LAST_STATE];
};