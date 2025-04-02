#include "Node.h"
#include "Symbol.h"
#include "Debug.h"

StartNode::StartNode(ProgramNode* program) : program(program) {}

StartNode::~StartNode() {
    MSG("Deleting StartNode\n");
    delete program;
}


ProgramNode::ProgramNode(BlockNode* block) : block(block) {}

ProgramNode::~ProgramNode() {
    MSG("Deleting ProgramNode\n");
    delete block;
}


BlockNode::BlockNode(StatementGroupNode* statementGroup) : statementGroup(statementGroup) {}

BlockNode::~BlockNode() {
    MSG("Deleting BlockNode\n");
    delete statementGroup;
}


void StatementGroupNode::AddStatement(StatementNode* statement) {
    statements.push_back(statement);
}

StatementGroupNode::~StatementGroupNode() {
    for (StatementNode* stmt : statements) {
        MSG("Deleting StatementNode \n");
        delete stmt;
    }
}

IfStatementNode::IfStatementNode(ExpressionNode* condition, StatementNode* thenStmt, StatementNode* elseStmt) 
    : condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {}

IfStatementNode::~IfStatementNode() {
    MSG("Deleting IfStatementNode\n");
    delete condition;
    delete thenStmt;
    if(elseStmt){
        delete elseStmt;
    }
}


WhileStatementNode::WhileStatementNode(ExpressionNode* condition, StatementNode* body) 
    : condition(condition), body(body) {}

WhileStatementNode::~WhileStatementNode() {
    MSG("Deleting WhileStatementNode\n");
    delete condition;
    delete body;
}

RepeatStatementNode::RepeatStatementNode(ExpressionNode* expression, StatementGroupNode* statementGroup) 
    : expression(expression), statementGroup(statementGroup) {}

RepeatStatementNode::~RepeatStatementNode() {
    MSG("Deleting RepeatStatementNode\n");
    delete expression;
    delete statementGroup;
}


ExpressionNode::~ExpressionNode() {
    MSG("Deleting ExpressionNode\n");
}

CoutStatementNode::CoutStatementNode(ExpressionNode* expression) : expression(expression) {}

CoutStatementNode::~CoutStatementNode() {
    MSG("Deleting CoutStatementNode\n");
    delete expression;
}


IdentifierNode::IdentifierNode(const std::string& label, SymbolTableClass* symbolTable) 
    : label(label), symbolTable(symbolTable) {}


void IdentifierNode::DeclareVariable() const {
    symbolTable->AddEntry(label);
}
void IdentifierNode::SetValue(int v) const {
    symbolTable->SetValue(label, v);
}
int IdentifierNode::GetIndex() const {
    return symbolTable->GetIndex(label);
}
int IdentifierNode::Evaluate() const {
    return symbolTable->GetValue(label);
}

DeclarationStatementNode::DeclarationStatementNode(IdentifierNode* identifier, ExpressionNode* expression) : identifier(identifier), expression(expression) {}

DeclarationStatementNode::~DeclarationStatementNode() {
    MSG("Deleting DeclarationStatementNode\n");
    delete identifier;
    delete expression;
}

AssignmentStatementNode::AssignmentStatementNode(IdentifierNode* identifier, ExpressionNode* expression) : identifier(identifier), expression(expression) {}

AssignmentStatementNode::~AssignmentStatementNode() {
    MSG("Deleting AssignmentStatementNode\n");
    delete identifier;
    delete expression;
}

IntegerNode::IntegerNode(int value) : value(value) {}

int IntegerNode::Evaluate() const {
    return value;
}


BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right) : left(left), right(right) {}

BinaryOperatorNode::~BinaryOperatorNode() {
    MSG( "Deleting BinaryOperatorNode\n");
    delete left;
    delete right;
}



PlusNode::PlusNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int PlusNode::Evaluate() const  {
    return left->Evaluate() + right->Evaluate();
}

MinusNode::MinusNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int MinusNode::Evaluate() const  {
    return left->Evaluate() - right->Evaluate();
}

TimesNode::TimesNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int TimesNode::Evaluate() const {
    return left->Evaluate() * right->Evaluate();
}

DivideNode::DivideNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int DivideNode::Evaluate() const {
    int rightValue = right->Evaluate();
    if (rightValue == 0) {
        throw std::runtime_error("Division by zero error");
    }
    return left->Evaluate() / rightValue;
}

LessNode::LessNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int LessNode::Evaluate() const {
    return left->Evaluate() < right->Evaluate() ? 1 : 0;
}

LessEqualNode::LessEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int LessEqualNode::Evaluate() const {
    return left->Evaluate() <= right->Evaluate() ? 1 : 0;
}

GreaterNode::GreaterNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int GreaterNode::Evaluate() const {
    return left->Evaluate() > right->Evaluate() ? 1 : 0;
}

GreaterEqualNode::GreaterEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int GreaterEqualNode::Evaluate() const {
    return left->Evaluate() >= right->Evaluate() ? 1 : 0;
}

EqualNode::EqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int EqualNode::Evaluate() const {
    return left->Evaluate() == right->Evaluate() ? 1 : 0;
}

NotEqualNode::NotEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int NotEqualNode::Evaluate() const {
    return left->Evaluate() != right->Evaluate() ? 1 : 0;
}

ModNode::ModNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int ModNode::Evaluate() const {
    int rightValue = right->Evaluate();
    if (rightValue == 0) {
        throw std::runtime_error("Division by zero error");
    }
    return left->Evaluate() % rightValue;
}

AndNode::AndNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int AndNode::Evaluate() const {
    return left->Evaluate() && right->Evaluate() ? 1 : 0;
}

OrNode::OrNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int OrNode::Evaluate() const {
    return left->Evaluate() || right->Evaluate() ? 1 : 0;
}

// PRINT METHODS
void StartNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Start" << std::endl;
    if(program){
        program->PrintTree(indent + 1);
    }
}

void ProgramNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) {
        std::cout << "  ";
    }
    std::cout << "Program" << std::endl;
    if(block){
        block->PrintTree(indent + 1);
    }
}

void BlockNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Block" << std::endl;
    if(statementGroup){
        statementGroup->PrintTree(indent + 1);
    }
}

void StatementGroupNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "StatementGroup" << std::endl;
    for (StatementNode* stmt : statements) {
        stmt->PrintTree(indent + 1);
    }
}

void CoutStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Cout" << std::endl;
    expression->PrintTree(indent + 1);
}


void IntegerNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Integer: " << value << std::endl;
}

void IdentifierNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Identifier: " << label << std::endl;
}

void BinaryOperatorNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Binary Operator" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void PlusNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Plus" << std::endl;
    if(left){
        left->PrintTree(indent + 1);
    }
    if(right){
        right->PrintTree(indent + 1);
    }
}

void MinusNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Minus" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void TimesNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Times" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void DivideNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Divide" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void LessNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Less" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void LessEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "LessEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void GreaterNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Greater" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void GreaterEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "GreaterEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void EqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Equal" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void NotEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "NotEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void ModNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Mod" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void DeclarationStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "DeclarationStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
}

void AssignmentStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "AssignmentStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
    if (expression) {
        expression->PrintTree(indent + 1);
    }
}

void IfStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "IfStatement" << std::endl;
    condition->PrintTree(indent + 1);
    thenStmt->PrintTree(indent + 1);
    if(elseStmt){
        elseStmt->PrintTree(indent + 1);
    }
}

void WhileStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "WhileStatement" << std::endl;
    condition->PrintTree(indent + 1);
    body->PrintTree(indent + 1);
}

void RepeatStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "RepeatStatement" << std::endl;
    expression->PrintTree(indent + 1);
    statementGroup->PrintTree(indent + 1);
}

void AndNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "And" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void OrNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Or" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

// INTERPRET METHODS
void StartNode::Interpret() const {
    program->Interpret();
}

void ProgramNode::Interpret() const {
    block->Interpret();
}

void BlockNode::Interpret() const {
    statementGroup->Interpret();
}

void StatementGroupNode::Interpret() const {
    for (const StatementNode* stmt : statements) {
        stmt->Interpret();
    }
}

void DeclarationStatementNode::Interpret() const {
    identifier->DeclareVariable();
    if(expression){
        identifier->SetValue(expression->Evaluate());
    }
}

void AssignmentStatementNode::Interpret() const {
    identifier->SetValue(expression->Evaluate());
}

void CoutStatementNode::Interpret() const {
    std::cout << expression->Evaluate() << std::endl;
}

void IfStatementNode::Interpret() const {
    if (condition->Evaluate()) {
        thenStmt->Interpret();
    } else if (elseStmt) {
        elseStmt->Interpret();
    }
}

void WhileStatementNode::Interpret() const {
    while (condition->Evaluate()) {
        body->Interpret();
    }
}

void RepeatStatementNode::Interpret() const {
    int count = expression->Evaluate();
    for(int i = 0; i < count; i++) {
        statementGroup->Interpret();
    }   
}