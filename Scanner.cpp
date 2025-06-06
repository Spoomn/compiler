#include "Scanner.h"
#include "Debug.h"

ScannerClass::ScannerClass(const std::string &inputFileName) {
    MSG("Initializing ScannerClass object...");

    mLineNumber = 1;

    mFin.open(inputFileName.c_str(), std::ios::binary);

    if (!mFin) {
        std::cerr << "Error opening input file: " << inputFileName << '\n';
        std::cerr << "Ensure the file exists and is in the same directory as Scanner.cpp\n";
        std::exit(1);
    }
}

ScannerClass::~ScannerClass() {
    if (mFin.is_open()) {
        mFin.close();
    }
}

TokenClass ScannerClass::GetNextToken(){
    MSG("\nGetting next token...");
    while(true){
        int p = mFin.peek();
        if (p == EOF) {
            return TokenClass(ENDFILE_TOKEN, "");
        }
        char c = static_cast<char>(p);
        if (c == ' ' || c == '\t' || c == '\r') {
            mFin.get();
            continue;
        }
        if (c == '\n') {
            mFin.get();
            mLineNumber++;
            continue;
        }
        break;
    }
    
    if (mFin.peek() == '<') {
        mFin.get(); 
        if (mFin.peek() == '<') {     
            mFin.get();                
            return TokenClass(INSERTION_TOKEN, "<<");
        }
        mFin.unget();
    }
    int c1 = mFin.peek();
    if (c1 == '+') {
        mFin.get();  
        int c2 = mFin.peek();
        if (c2 == '+') { 
            mFin.get();
            return TokenClass(PLUS_PLUS_TOKEN, "++");
        }
        else if (c2 == '=') { 
            mFin.get();
            return TokenClass(PLUS_EQUAL_TOKEN, "+=");
        }
        mFin.unget();
    }
    else if (c1 == '-') {
        mFin.get();
        int c2 = mFin.peek();
        if (c2 == '-') {              
            mFin.get();
            return TokenClass(MINUS_MINUS_TOKEN, "--");
        }
        else if (c2 == '=') {        
            mFin.get();
            return TokenClass(MINUS_EQUAL_TOKEN, "-=");
        }
        mFin.unget();
    }
    // exponent python style
    else if (c1 == '*'){
        mFin.get();
        int c2 = mFin.peek();
        if (c2 == '*') {              
            mFin.get();
            return TokenClass(POWER_TOKEN, "**");
        }
        mFin.unget();
    }
    

    StateMachineClass stateMachine;
    std::string lexeme;
    MachineState currentState;
    TokenType previousTokenType;
    char c;
    do{
        c = mFin.get();
        lexeme += c;
        currentState = stateMachine.UpdateState(c, previousTokenType);
        if (c == '\n'){
            mLineNumber++;
        }

        if(currentState == START_STATE || currentState == ENDFILE_STATE)
            lexeme = "";

    } while (currentState != CANTMOVE_STATE);
    if (c == '\n'){
        mLineNumber--;
        MSG("End of line "<< mLineNumber);
    }

    if (previousTokenType == BAD_TOKEN){
        std::cerr << "Error. BAD_TOKEN from lexeme:" << lexeme;
        std::exit(1);
    }
    MSG("Final lexeme before unget: \"" << lexeme << "\"");
    MSG("Final token type from mapping: " << previousTokenType << " (" << gTokenTypeNames[previousTokenType] << ")");
    lexeme.pop_back();
    mFin.unget();
    MSG("Number of lines: " << ScannerClass::GetLineNumber());
    if (previousTokenType == IDENTIFIER_TOKEN) {
        if (lexeme == "VOID") {
            previousTokenType = VOID_TOKEN;
        } else if (lexeme == "MAIN") {
            previousTokenType = MAIN_TOKEN;
        } else if (lexeme == "INT") {
            previousTokenType = INT_TOKEN;
        } else if (lexeme == "COUT") {
            previousTokenType = COUT_TOKEN;
        } else if (lexeme == "endl"){
            previousTokenType = ENDL_TOKEN;
        }
    }
    TokenClass tc(previousTokenType, lexeme);
    return tc;
}

int ScannerClass::GetLineNumber() const{
    return mLineNumber;
}

TokenClass ScannerClass::PeekNextToken(){
    std::streampos pos = mFin.tellg();
    int savedLine = mLineNumber;
    TokenClass tc = GetNextToken();
    if(!mFin){
        mFin.clear();
    }
    mFin.seekg(pos);
    mLineNumber = savedLine;
    return tc;
}