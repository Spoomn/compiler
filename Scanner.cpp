#include "Scanner.h"
#include "Debug.h"

ScannerClass::ScannerClass(const std::string &inputFileName) {
    MSG("Initializing ScannerClass object...");

    mLineNumber = 1;

    mFin.open(inputFileName.c_str(), std::ios::binary);

    // Check if the file opened successfully
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

    lexeme.pop_back();
    mFin.unget();
    MSG("Number of lines: " << ScannerClass::GetLineNumber());
    TokenClass tc(previousTokenType, lexeme);
    return tc;
}

int ScannerClass::GetLineNumber() const{
    return mLineNumber;
}