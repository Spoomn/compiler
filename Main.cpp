#include "Token.h"
#include "Scanner.h"
#include "StateMachine.h"
#include "Symbol.h"
#include "Node.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

void TestScanner();
void TestSymbolTable();
void TestParseTree();

int main() {
    TestScanner();
    TestSymbolTable();
    TestParseTree();

    return 0;
}

void TestScanner() {
    std::cout << "\n-- UNIT TEST: Scanner --\n" << std::endl;
    
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
}

void TestSymbolTable() {
    std::cout << "\n-- UNIT TEST: SymbolTableClass --\n" << std::endl;

    SymbolTableClass symTab;
    assert(symTab.GetCount() == 0);

    // Test normal AddEntry for "x"
    try {
        symTab.AddEntry("x");
        std::cout << "Added entry 'x' successfully." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when adding 'x': " << e.what() << std::endl;
        return;
    }
    assert(symTab.Exists("x"));
    assert(symTab.GetCount() == 1);

    // Test duplicate AddEntry for "x"
    try {
        symTab.AddEntry("x");  // Should throw an error
        std::cerr << "Error: Duplicate AddEntry did not throw an exception." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected duplicate add error: " << e.what() << std::endl;
    }

    // Test SetValue on "x"
    try {
        symTab.SetValue("x", 42);
        std::cout << "Set 'x' to 42 successfully." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when setting 'x': " << e.what() << std::endl;
        return;
    }

    // Test GetValue on "x"
    try {
        int value = symTab.GetValue("x");
        assert(value == 42);
        std::cout << "Got 'x' value: " << value << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when getting 'x': " << e.what() << std::endl;
        return;
    }

    // Test GetValue on a non-existent variable
    try {
        symTab.GetValue("z");
        std::cerr << "Error: GetValue on non-existent variable 'z' did not throw an exception." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected error for 'z': " << e.what() << std::endl;
    }

    // Test SetValue on a non-existent variable
    try {
        symTab.SetValue("z", 55);
        std::cerr << "Error: SetValue on non-existent variable 'z' did not throw an exception." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected error for 'z': " << e.what() << std::endl;
    }

    // Test GetIndex on a non-existent variable
    try {
        symTab.GetIndex("z");
        std::cerr << "Error: GetIndex on non-existent variable 'z' did not throw an exception." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught expected error for GetIndex on 'z': " << e.what() << std::endl;
    }

    // Add another variable "y"
    try {
        symTab.AddEntry("y");
        std::cout << "Added entry 'y' successfully." << std::endl;
        symTab.SetValue("y", 99);
        std::cout << "Set 'y' to 99 successfully." << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when handling 'y': " << e.what() << std::endl;
        return;
    }

    assert(symTab.Exists("y"));
    assert(symTab.GetCount() == 2);

    try {
        int valueY = symTab.GetValue("y");
        assert(valueY == 99);
        std::cout << "Got 'y' value: " << valueY << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when getting 'y': " << e.what() << std::endl;
        return;
    }

    // Get and display indices
    try {
        int indexX = symTab.GetIndex("x");
        int indexY = symTab.GetIndex("y");
        std::cout << "Index of 'x': " << indexX << "\n";
        std::cout << "Index of 'y': " << indexY << "\n";
    } catch (const std::runtime_error& e) {
        std::cerr << "Unexpected error when getting indices: " << e.what() << std::endl;
        return;
    }

    std::cout << "SymbolTableClass tests passed successfully!" << std::endl;
}

void TestParseTree() {
    std::cout << "\n-- UNIT TEST: Parse Tree --\n" << std::endl;

    ExpressionNode* three = new IntegerNode(3);
    ExpressionNode* four = new IntegerNode(4);
    ExpressionNode* ten = new IntegerNode(10);
    ExpressionNode* two = new IntegerNode(2);

    ExpressionNode* addNode = new PlusNode(three, four);    // (3 + 4)
    ExpressionNode* subNode = new MinusNode(ten, two);      // (10 - 2)
    ExpressionNode* mulNode = new TimesNode(addNode, subNode); // (3 + 4) * (10 - 2)

    int result = mulNode->Evaluate();
    std::cout << "Expression evaluates to: " << result << std::endl;
    assert(result == 56);

    delete mulNode;

    std::cout << "Parse tree nodes successfully deleted." << std::endl;
}