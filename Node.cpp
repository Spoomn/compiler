#include "Node.h"
#include "Symbol.h"

StartNode::StartNode(ProgramNode* program) : program(program) {}

StartNode::~StartNode() {
    delete program;
}

ProgramNode::ProgramNode(BlockNode* block) : block(block) {}

ProgramNode::~ProgramNode() {
    delete block;
}

BlockNode::BlockNode(StatementGroupNode* statementGroup) : statementGroup(statementGroup) {}

BlockNode::~BlockNode() {
    delete statementGroup;
}

void StatementGroupNode::AddStatement(StatementNode* statement) {
    statements.push_back(statement);
}

StatementGroupNode::~StatementGroupNode() {
    for (StatementNode* stmt : statements) {
        delete stmt;
    }
}

CoutStatementNode::CoutStatementNode(ExpressionNode* expression) : expression(expression) {}

CoutStatementNode::~CoutStatementNode() {
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

DeclarationStatementNode::DeclarationStatementNode(IdentifierNode* identifier) : identifier(identifier) {}

DeclarationStatementNode::~DeclarationStatementNode() {
    delete identifier;
}

AssignmentStatementNode::AssignmentStatementNode(IdentifierNode* identifier, ExpressionNode* expression) : identifier(identifier), expression(expression) {}

AssignmentStatementNode::~AssignmentStatementNode() {
    delete identifier;
    delete expression;
}

IntegerNode::IntegerNode(int value) : value(value) {}

int IntegerNode::Evaluate() const {
    return value;
}

BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right) : left(left), right(right) {}

BinaryOperatorNode::~BinaryOperatorNode() {
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
