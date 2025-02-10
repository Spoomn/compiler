#include "Token.h"
#include "Scanner.h"
#include "StateMachine.h"
#include "Symbol.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

int main(){
    // UNIT TESTS for Scanner 1 & 2
    ScannerClass scanner("code.txt");
    TokenType tt;
    int currentLine = -1;

    do {
        TokenClass tc = scanner.GetNextToken();
        int tokenLine = scanner.GetLineNumber();
        if (tokenLine != currentLine) {
            std::cout << "\nLine " << tokenLine << ":\n";
            currentLine = tokenLine;
        }
        std::cout << tc << std::endl;
        tt = tc.GetTokenType();
    } while (tt != ENDFILE_TOKEN);
    
    std::cout << "\nTotal lines: " << scanner.GetLineNumber() << std::endl;

    // UNIT TESTS FOR SymbolTableClass
    std::cout << "\n--UNIT TESTS FOR SymbolTableClass--\n" << std::endl;
    SymbolTableClass symTab;
    
    assert(symTab.GetCount() == 0);

    // Test normal AddEntry for "x"
    try {
        symTab.AddEntry("x");
        std::cout << "Added entry 'x' successfully." << std::endl;
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when adding 'x': " << e.what() << std::endl;
        return 1;
    }
    assert(symTab.Exists("x") == true);
    assert(symTab.GetCount() == 1);

    // Test duplicate AddEntry for "x"
    try {
        symTab.AddEntry("x");  // This should throw an error
        std::cerr << "Error: Duplicate AddEntry did not throw an exception." << std::endl;
        return 1;
    } catch (const std::runtime_error & e) {
        std::cout << "Caught expected duplicate add error: " << e.what() << std::endl;
    }

    // Test normal SetValue on "x"
    try {
        symTab.SetValue("x", 42);
        std::cout << "Set 'x' to 42 successfully." << std::endl;
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when setting 'x': " << e.what() << std::endl;
        return 1;
    }

    // Test GetValue on "x"
    try {
        int value = symTab.GetValue("x");
        assert(value == 42);
        std::cout << "Got 'x' value: " << value << std::endl;
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when getting 'x': " << e.what() << std::endl;
        return 1;
    }

    // Test GetValue on a non-existent variable ("z")
    try {
        symTab.GetValue("z");
        std::cerr << "Error: GetValue on non-existent variable 'z' did not throw an exception." << std::endl;
        return 1;
    } catch (const std::runtime_error & e) {
        std::cout << "Caught expected error on GetValue for non-existent variable 'z': " << e.what() << std::endl;
    }

    // Test SetValue on a non-existent variable ("z")
    try {
        symTab.SetValue("z", 55);
        std::cerr << "Error: SetValue on non-existent variable 'z' did not throw an exception." << std::endl;
        return 1;
    } catch (const std::runtime_error & e) {
        std::cout << "Caught expected error on SetValue for non-existent variable 'z': " << e.what() << std::endl;
    }

    // Test GetIndex on a non-existent variable ("z")
    try {
        symTab.GetIndex("z");
        std::cerr << "Error: GetIndex on non-existent variable 'z' did not throw an exception." << std::endl;
        return 1;
    } catch (const std::runtime_error & e) {
        std::cout << "Caught expected error on GetIndex for non-existent variable 'z': " << e.what() << std::endl;
    }

    // Test adding and using another variable ("y")
    try {
        symTab.AddEntry("y");
        std::cout << "Added entry 'y' successfully." << std::endl;
        symTab.SetValue("y", 99);
        std::cout << "Set 'y' to 99 successfully." << std::endl;
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when handling 'y': " << e.what() << std::endl;
        return 1;
    }
    assert(symTab.Exists("y") == true);
    assert(symTab.GetCount() == 2);
    try {
        int valueY = symTab.GetValue("y");
        assert(valueY == 99);
        std::cout << "Got 'y' value: " << valueY << std::endl;
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when getting 'y': " << e.what() << std::endl;
        return 1;
    }

    // Get and display indices
    try {
        int indexX = symTab.GetIndex("x");
        int indexY = symTab.GetIndex("y");
        std::cout << "Index of 'x': " << indexX << "\n";
        std::cout << "Index of 'y': " << indexY << "\n";
    } catch (const std::runtime_error & e) {
        std::cerr << "Unexpected error when getting indices: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "All tests passed successfully!" << std::endl;
    
    return 0;
}