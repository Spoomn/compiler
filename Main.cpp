#include "Token.h"
#include "Scanner.h"
#include "StateMachine.h"
#include "Symbol.h"
#include "Node.h"
#include "Parser.h"
#include "Debug.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

// void TestScanner();
// void TestSymbolTable();
// void TestParseTree();
// void TestParser();
// void TestOutputParser();
// void TestInterpreter();
// void TestTest();
void CodeAndExecute(const std::string &filename);

int main() {
    // TestScanner();
    // TestSymbolTable();
    // TestParseTree();
    // TestParser();
    // TestOutputParser();
    // TestInterpreter();
    // TestTest();
    CodeAndExecute("test.txt");

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

// void TestParseTree() {
//     std::cout << "\n-- UNIT TEST: Parse Tree --\n" << std::endl;
//     SymbolTableClass symTab;

//     // IdentifierNode* declId = new IdentifierNode("x", &symTab);
//     // DeclarationStatementNode* declStmt = new DeclarationStatementNode(declId);
    
//     IdentifierNode* assignId = new IdentifierNode("x", &symTab);
//     ExpressionNode* intThree = new IntegerNode(3);
//     ExpressionNode* intFour = new IntegerNode(4);
//     ExpressionNode* plusExpr = new TimesNode(intThree, intFour);
//     AssignmentStatementNode* assignStmt = new AssignmentStatementNode(assignId, plusExpr);

//     IdentifierNode* coutID = new IdentifierNode("x", &symTab);
//     CoutStatementNode* coutStmt = new CoutStatementNode(coutID);

//     StatementGroupNode* stmtGroup = new StatementGroupNode();
//     // stmtGroup->AddStatement(declStmt);
//     stmtGroup->AddStatement(assignStmt);
//     stmtGroup->AddStatement(coutStmt);

//     BlockNode* block = new BlockNode(stmtGroup);

//     ProgramNode* program = new ProgramNode(block);

//     StartNode* start = new StartNode(program);
//     int mathResult = plusExpr->Evaluate();
//     assert(mathResult == 12);
//     std::cout << "Math result: " << mathResult << std::endl;
//     delete start;

//     std::cout << "Parse tree nodes successfully deleted." << std::endl;
// }

void TestParser() {
    std::cout << "\n-- UNIT TEST: Parser (no output) --\n" << std::endl;
    
    const std::string testFiles[] = {
        "parser_test_1.txt",
        "parser_test_2.txt",
        "parser_test_3.txt"
    };
    int numTests = sizeof(testFiles) / sizeof(testFiles[0]);
    for (int i = 0; i < numTests; i++) {
        std::cout << "\nTest " << (i + 1) << ": " << testFiles[i] << std::endl;
        ScannerClass scanner(testFiles[i]);
        SymbolTableClass symTab;
        ParserClass parser(&scanner, &symTab);
        parser.Start();
        std::cout << "Test " << (i + 1) << " passed successfully." << std::endl;
    }
}

void TestOutputParser(){
    std::cout << "\n-- UNIT TEST: Parser with output --\n" << std::endl;

    ScannerClass scanner("parser_output_test_1.txt");
    std::cout << "Original Code Input:\n" << std::endl;
    std::ifstream file("parser_output_test_1.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    SymbolTableClass symTab;
    ParserClass parser(&scanner, &symTab);

    StartNode* start = parser.Start();

    
    std::cout << "\n--- Parsed Tree Output ---\n" << std::endl;
    start->PrintTree();
    std::cout << "\n--- End of Parsed Tree Output ---\n" << std::endl;
    std::cout << "Deleting parsed tree..." << std::endl;
    delete start;
    std::cout << "Parsed tree deleted" << std::endl;
    std::cout << "\nParser with output test completed." << std::endl;
}

void TestInterpreter() {
    std::cout << "\n-- UNIT TEST: Interpreter --\n" << std::endl;
    

    ScannerClass scanner("ifwhile_test.txt");
    SymbolTableClass symTab;
    ParserClass parser(&scanner, &symTab);
    MSG("Starting parser...");
    
    StartNode* root = parser.Start();
    
    std::cout << "\n--- Parsed Tree Output ---\n" << std::endl;
    root->PrintTree();
    
    std::cout << "\nInterpreting Program Output:\n" << std::endl;
    root->Interpret();
    std::cout << std::endl;
    
    delete root;
    
    std::cout << "\nInterpreter test completed." << std::endl;
}

void TestTest(){
    std::cout << "\n-- UNIT TEST: Test --\n" << std::endl;
    
    ScannerClass scanner("test2.txt");
    SymbolTableClass symTab;
    ParserClass parser(&scanner, &symTab);
    MSG("Starting parser...");
    
    StartNode* root = parser.Start();
    
    std::cout << "\n--- Parsed Tree Output ---\n" << std::endl;
    root->PrintTree();
    
    std::cout << "\nInterpreting Program Output:\n" << std::endl;
    root->Interpret();
    std::cout << std::endl;
    
    delete root;
    
    std::cout << "\nTest test completed." << std::endl;
}

void CodeAndExecute(const std::string &filename)
{
    // 1) build the scanner, symbol table, and parser
    ScannerClass    scanner(filename);
    SymbolTableClass symbolTable;
    ParserClass     parser(&scanner, &symbolTable);

    // 2) parse → AST
    StartNode * root = parser.Start();

    // 3) generate bytecodes
    InstructionsClass machineCode;
    root->Code(machineCode);
    machineCode.Finish();
    // machineCode.PrintAllMachineCodes();

    // 4) run them on VM
    machineCode.Execute();

    // 5) tear down the AST
    delete root;
}
