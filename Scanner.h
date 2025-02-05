#pragma once
#include "Token.h"
#include "StateMachine.h"
#include <iostream>
#include <fstream>
#include <string>

class ScannerClass {
    public:
        ScannerClass(const std::string &inputFileName);

        ~ScannerClass();

        TokenClass GetNextToken();

        int GetLineNumber() const;


    private:
        std::ifstream mFin;
        int mLineNumber;
    };
