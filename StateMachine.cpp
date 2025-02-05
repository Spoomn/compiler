#include "StateMachine.h"
#include "Token.h"
#include "Debug.h"

StateMachineClass::StateMachineClass(){
    MSG("Initializing StateMachineClass");
    // Start in the current state
    mCurrentState = START_STATE;

    // Initialize everything to a "can't move" state
    for(int i=0; i<LAST_STATE; i++){
        for(int j=0; j<LAST_CHAR; j++){
            mLegalMoves[i][j] = CANTMOVE_STATE;
        }
    }

    // Overwrite specific edges from FA
    // Integer State
    mLegalMoves[START_STATE][DIGIT_CHAR] = INTEGER_STATE;
	mLegalMoves[INTEGER_STATE][DIGIT_CHAR] = INTEGER_STATE;

    // Identifier State
    mLegalMoves[START_STATE][LETTER_CHAR] = IDENTIFIER_STATE;
    mLegalMoves[IDENTIFIER_STATE][LETTER_CHAR] = IDENTIFIER_STATE;
    mLegalMoves[IDENTIFIER_STATE][DIGIT_CHAR] = IDENTIFIER_STATE;

    // MATH STATES
        // Plus State
        mLegalMoves[START_STATE][PLUS_CHAR] = PLUS_STATE;

        // Minus State
        mLegalMoves[START_STATE][MINUS_CHAR] = MINUS_STATE;

        // Times State
        mLegalMoves[START_STATE][ASTERISK_CHAR] = TIMES_STATE;

        // Divide State
        for (int j = 0; j < LAST_CHAR; j++) {
            if (j != ASTERISK_CHAR && j != SLASH_CHAR) {
                mLegalMoves[POSSIBLE_COMMENT_STATE][j] = DIVIDE_STATE;
            }
        }

        // Less State
        mLegalMoves[START_STATE][LESS_CHAR] = LESS_STATE;

        // Less Equal State
        mLegalMoves[LESS_STATE][EQUAL_CHAR] = LESSEQUAL_STATE;

        // Greater State
        mLegalMoves[START_STATE][GREATER_CHAR] = GREATER_STATE;

        // Greater Equal State
        mLegalMoves[GREATER_STATE][EQUAL_CHAR] = GREATEREQUAL_STATE;

        // Equal State
        mLegalMoves[START_STATE][EQUAL_CHAR] = EQUAL_STATE;

        // Not Equal State
        mLegalMoves[START_STATE][EXCLAMATION_CHAR] = EXCLAMATION_STATE;
        mLegalMoves[EXCLAMATION_STATE][EQUAL_CHAR] = NOTEQUAL_STATE;

    // Insertion State
    mLegalMoves[LESS_STATE][LESS_CHAR] = INSERTION_STATE;

    // Semicolon State
    mLegalMoves[START_STATE][SEMICOLON_CHAR] = SEMICOLON_STATE;

    // Whitespace or Return Char -> Start State 🚀
    mLegalMoves[START_STATE][WHITESPACE_CHAR] = START_STATE;
    mLegalMoves[START_STATE][RETURN_CHAR] = START_STATE;

    // Parentheses States
    mLegalMoves[START_STATE][LPAREN_CHAR] = LPAREN_STATE;
    mLegalMoves[START_STATE][RPAREN_CHAR] = RPAREN_STATE;

    // Curly States
    mLegalMoves[START_STATE][LCURLY_CHAR] = LCURLY_STATE;
    mLegalMoves[START_STATE][RCURLY_CHAR] = RCURLY_STATE;

    // COMMENT STATE LOGIC
        // Possible Comment State
        mLegalMoves[START_STATE][SLASH_CHAR] = POSSIBLE_COMMENT_STATE;
        // Enter block comment
        mLegalMoves[POSSIBLE_COMMENT_STATE][ASTERISK_CHAR] = BLOCK_COMMENT_STATE;
        // Block Comment End State (preparation to exit block comment)
        mLegalMoves[BLOCK_COMMENT_STATE][ASTERISK_CHAR] = BLOCK_COMMENT_END_STATE;

        // For any character other than asterisk, stay within comment state
        for (int j = 0; j < LAST_CHAR; j++){
            if (j!= ASTERISK_CHAR){
                mLegalMoves[BLOCK_COMMENT_STATE][j] = BLOCK_COMMENT_STATE;
            }
        }
        // Resets and exits Block Comment State
        mLegalMoves[BLOCK_COMMENT_END_STATE][SLASH_CHAR] = START_STATE;
        // Stay in end state if an asterisk is seen in Block Comment End State
        mLegalMoves[BLOCK_COMMENT_END_STATE][ASTERISK_CHAR] = BLOCK_COMMENT_END_STATE;

        // For any other character, revert to BLOCK_COMMENT_STATE.
        for (int j = 0; j < LAST_CHAR; j++) {
            if(j != SLASH_CHAR && j != ASTERISK_CHAR) {
                mLegalMoves[BLOCK_COMMENT_END_STATE][j] = BLOCK_COMMENT_STATE;
            }
        }

        // Line comment State
        mLegalMoves[POSSIBLE_COMMENT_STATE][SLASH_CHAR] = LINE_COMMENT_STATE;

        // In LINE_COMMENT_STATE, all characters except RETURN_CHAR and ENDFILE_CHAR loop:
        for (int j = 0; j < LAST_CHAR; j++) {
            if (j != RETURN_CHAR && j != ENDFILE_CHAR) {
                mLegalMoves[LINE_COMMENT_STATE][j] = LINE_COMMENT_STATE;
            }
        }   

        // Return resets to start state from a line comment state
        mLegalMoves[LINE_COMMENT_STATE][RETURN_CHAR] = START_STATE;
        // Endfile also kills a line comment state
        mLegalMoves[LINE_COMMENT_STATE][ENDFILE_CHAR] = ENDFILE_STATE;


    // EOF
    mLegalMoves[START_STATE][ENDFILE_CHAR] = ENDFILE_STATE;
    

    // Initialize all corresponding token types to BAD_TOKEN
    for(int i=0; i<LAST_STATE; i++)
	{
		mCorrespondingTokenTypes[i]=BAD_TOKEN;
	}

    // Overwrite token types with correct tokens
	mCorrespondingTokenTypes[INTEGER_STATE] = INTEGER_TOKEN;
    mCorrespondingTokenTypes[IDENTIFIER_STATE] = IDENTIFIER_TOKEN;
    mCorrespondingTokenTypes[PLUS_STATE] = PLUS_TOKEN;
    mCorrespondingTokenTypes[MINUS_STATE] = MINUS_TOKEN;
    mCorrespondingTokenTypes[TIMES_STATE] = TIMES_TOKEN;
    mCorrespondingTokenTypes[DIVIDE_STATE] = DIVIDE_TOKEN;
    mCorrespondingTokenTypes[LESS_STATE] = LESS_TOKEN;
    mCorrespondingTokenTypes[LESSEQUAL_STATE] = LESSEQUAL_TOKEN;
    mCorrespondingTokenTypes[GREATER_STATE] = GREATER_TOKEN;
    mCorrespondingTokenTypes[GREATEREQUAL_STATE] = GREATEREQUAL_TOKEN;
    mCorrespondingTokenTypes[NOTEQUAL_STATE] = NOTEQUAL_TOKEN;
    mCorrespondingTokenTypes[INSERTION_STATE] = INSERTION_TOKEN;
    mCorrespondingTokenTypes[SEMICOLON_STATE] = SEMICOLON_TOKEN;
    mCorrespondingTokenTypes[EQUAL_STATE] = EQUAL_TOKEN;
    mCorrespondingTokenTypes[LPAREN_STATE] = LPAREN_TOKEN;
    mCorrespondingTokenTypes[RPAREN_STATE] = RPAREN_TOKEN;
    mCorrespondingTokenTypes[LCURLY_STATE] = LCURLY_TOKEN;
    mCorrespondingTokenTypes[RCURLY_STATE] = RCURLY_TOKEN;
    mCorrespondingTokenTypes[ENDFILE_STATE] = ENDFILE_TOKEN;
}

MachineState StateMachineClass::UpdateState(char currentCharacter, TokenType & previousTokenType){
    MSG("Updating state for character: " << "'" << currentCharacter << "'");
    CharacterType charType = BAD_CHAR;

    if(isdigit(currentCharacter))
        charType = DIGIT_CHAR;
    else if(isalpha(currentCharacter))
        charType = LETTER_CHAR;
    else if(isspace(currentCharacter))
        charType = WHITESPACE_CHAR;
    else if(currentCharacter == '+')
        charType = PLUS_CHAR;
    else if(currentCharacter == '-')
        charType = MINUS_CHAR;
    else if(currentCharacter == ';')
        charType = SEMICOLON_CHAR;
    else if(currentCharacter == '=')
        charType = EQUAL_CHAR;
    else if(currentCharacter == '<')
        charType = LESS_CHAR;
    else if(currentCharacter == '>')
        charType = GREATER_CHAR;
    else if(currentCharacter == '!')
        charType = EXCLAMATION_CHAR;
    else if(currentCharacter == '(')
        charType = LPAREN_CHAR;
    else if(currentCharacter == ')')
        charType = RPAREN_CHAR;
    else if(currentCharacter == '{')
        charType = LCURLY_CHAR;
    else if(currentCharacter == '}')
        charType = RCURLY_CHAR;
    else if(currentCharacter == '/')
        charType = SLASH_CHAR;
    else if(currentCharacter == '*')
        charType = ASTERISK_CHAR;
    else if(currentCharacter == '\n')
        charType = RETURN_CHAR;
    else if(currentCharacter == EOF)
        charType = ENDFILE_CHAR;

    previousTokenType = mCorrespondingTokenTypes[mCurrentState];
    mCurrentState = mLegalMoves[mCurrentState][charType];

    return mCurrentState;
}