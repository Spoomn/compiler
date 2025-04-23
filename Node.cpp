#include "Node.h"
#include "Symbol.h"
#include "Debug.h"

StartNode::StartNode(ProgramNode* program) : program(program) {}

StartNode::~StartNode() {
    MSG("Deleting StartNode\n");
    delete program;
}

void StartNode::Interpret() const {
    program->Interpret();
}

void StartNode::Code(InstructionsClass &machineCode)
{
    program->Code(machineCode);
}

void StartNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Start" << std::endl;
    if(program){
        program->PrintTree(indent + 1);
    }
}

ProgramNode::ProgramNode(BlockNode* block) : block(block) {}

ProgramNode::~ProgramNode() {
    MSG("Deleting ProgramNode\n");
    delete block;
}

void ProgramNode::Interpret() const {
    block->Interpret();
}

void ProgramNode::Code(InstructionsClass &machineCode)
{
    block->Code(machineCode);
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


BlockNode::BlockNode(StatementGroupNode* statementGroup) : statementGroup(statementGroup) {}

BlockNode::~BlockNode() {
    MSG("Deleting BlockNode\n");
    delete statementGroup;
}

void BlockNode::Interpret() const {
    statementGroup->Interpret();
}

void BlockNode::Code(InstructionsClass &machineCode)
{
    statementGroup->Code(machineCode);
}

void BlockNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Block" << std::endl;
    if(statementGroup){
        statementGroup->PrintTree(indent + 1);
    }
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

void StatementGroupNode::Interpret() const {
    for (const StatementNode* stmt : statements) {
        stmt->Interpret();
    }
}

void StatementGroupNode::Code(InstructionsClass &machineCode)
{
    for (StatementNode* stmt : statements) {
        stmt->Code(machineCode);
    }
}

void StatementGroupNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "StatementGroup" << std::endl;
    for (StatementNode* stmt : statements) {
        stmt->PrintTree(indent + 1);
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

void IfStatementNode::Interpret() const {
    if (condition->Evaluate()) {
        thenStmt->Interpret();
    } else if (elseStmt) {
        elseStmt->Interpret();
    }
}

void IfStatementNode::Code(InstructionsClass &machineCode)
{
    condition->CodeEvaluate(machineCode);
    unsigned char * InsertAddress = machineCode.SkipIfZeroStack();
    unsigned char * address1 = machineCode.GetAddress();
    thenStmt->Code(machineCode);
    unsigned char * address2 = machineCode.GetAddress();
    machineCode.SetOffset(InsertAddress, (int)(address2 - address1));
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

WhileStatementNode::WhileStatementNode(ExpressionNode* condition, StatementNode* body) 
    : condition(condition), body(body) {}

WhileStatementNode::~WhileStatementNode() {
    MSG("Deleting WhileStatementNode\n");
    delete condition;
    delete body;
}

void WhileStatementNode::Interpret() const {
    while (condition->Evaluate()) {
        body->Interpret();
    }
}

void WhileStatementNode::Code(InstructionsClass &machineCode)
{
    unsigned char* address1 = machineCode.GetAddress();
    condition->CodeEvaluate(machineCode);
    unsigned char* insertSkip = machineCode.SkipIfZeroStack();
    unsigned char* address2 = machineCode.GetAddress();
    body->Code(machineCode);
    unsigned char* insertJump = machineCode.Jump();
    unsigned char* address3 = machineCode.GetAddress();
    machineCode.SetOffset(insertSkip, static_cast<int>(address3 - address2));
    machineCode.SetOffset(insertJump, static_cast<int>(address1 - address3));
}

void WhileStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "WhileStatement" << std::endl;
    condition->PrintTree(indent + 1);
    body->PrintTree(indent + 1);
}

RepeatStatementNode::RepeatStatementNode(ExpressionNode* expression, StatementGroupNode* statementGroup) 
    : expression(expression), statementGroup(statementGroup) {}

RepeatStatementNode::~RepeatStatementNode() {
    MSG("Deleting RepeatStatementNode\n");
    delete expression;
    delete statementGroup;
}

void RepeatStatementNode::Interpret() const {
    int count = expression->Evaluate();
    for(int i = 0; i < count; i++) {
        statementGroup->Interpret();
    }   
}

void RepeatStatementNode::Code(InstructionsClass &machineCode)
{
    expression->CodeEvaluate(machineCode);

}

void RepeatStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "RepeatStatement" << std::endl;
    expression->PrintTree(indent + 1);
    statementGroup->PrintTree(indent + 1);
}

ExpressionNode::~ExpressionNode() {
    MSG("Deleting ExpressionNode\n");
}

CoutStatementNode::CoutStatementNode(ExpressionNode* expression) : expression(expression) {}

CoutStatementNode::~CoutStatementNode() {
    MSG("Deleting CoutStatementNode\n");
    delete expression;
}

void CoutStatementNode::Interpret() const {
    std::cout << expression->Evaluate() << std::endl;
}

void CoutStatementNode::Code(InstructionsClass &machineCode)
{
    expression->CodeEvaluate(machineCode);
    machineCode.PopAndWrite();
    MSG("Printing value\n");
}

void CoutStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Cout" << std::endl;
    expression->PrintTree(indent + 1);
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
void IdentifierNode::CodeEvaluate(InstructionsClass &machineCode)
{
    machineCode.PushVariable(this->GetIndex());
}
void IdentifierNode::PrintTree(int indent) const
{
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Identifier: " << label << std::endl;
}

DeclarationStatementNode::DeclarationStatementNode(IdentifierNode* identifier, ExpressionNode* expression) : identifier(identifier), expression(expression) {}

DeclarationStatementNode::~DeclarationStatementNode() {
    MSG("Deleting DeclarationStatementNode\n");
    delete identifier;
    delete expression;
}

void DeclarationStatementNode::Interpret() const {
    identifier->DeclareVariable();
    if(expression){
        identifier->SetValue(expression->Evaluate());
    }
}

void DeclarationStatementNode::Code(InstructionsClass &machineCode)
{
    identifier->DeclareVariable();
    if(expression){
        expression->CodeEvaluate(machineCode);
        int slot = identifier->GetIndex();
        machineCode.PopAndStore(slot);
        MSG("Storing value in slot: " << slot << std::endl);
    }
}

void DeclarationStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "DeclarationStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
}

AssignmentStatementNode::AssignmentStatementNode(IdentifierNode* identifier, ExpressionNode* expression) : identifier(identifier), expression(expression) {}

AssignmentStatementNode::~AssignmentStatementNode() {
    MSG("Deleting AssignmentStatementNode\n");
    delete identifier;
    delete expression;
}

void AssignmentStatementNode::Interpret() const {
    identifier->SetValue(expression->Evaluate());
}

void AssignmentStatementNode::Code(InstructionsClass &machineCode)
{
    expression->CodeEvaluate(machineCode);
    int slot = identifier->GetIndex();
    machineCode.PopAndStore(slot);
    MSG("Storing value in slot: " << slot << std::endl);
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

IntegerNode::IntegerNode(int value) : value(value) {}

int IntegerNode::Evaluate() const {
    return value;
}

void IntegerNode::CodeEvaluate(InstructionsClass &machineCode)
{
    machineCode.PushValue(value);
}

void IntegerNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Integer: " << value << std::endl;
}


BinaryOperatorNode::BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right) : left(left), right(right) {}

BinaryOperatorNode::~BinaryOperatorNode() {
    MSG( "Deleting BinaryOperatorNode\n");
    delete left;
    delete right;
}

void BinaryOperatorNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Binary Operator" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}



PlusNode::PlusNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int PlusNode::Evaluate() const  {
    return left->Evaluate() + right->Evaluate();
}

void PlusNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopAddPush();

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

MinusNode::MinusNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int MinusNode::Evaluate() const  {
    return left->Evaluate() - right->Evaluate();
}

void MinusNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopSubPush();
}

void MinusNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Minus" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

TimesNode::TimesNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int TimesNode::Evaluate() const {
    return left->Evaluate() * right->Evaluate();
}

void TimesNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopMulPush();
}

void TimesNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Times" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}


DivideNode::DivideNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int DivideNode::Evaluate() const {
    int rightValue = right->Evaluate();
    if (rightValue == 0) {
        throw std::runtime_error("Division by zero error");
    }
    return left->Evaluate() / rightValue;
}

void DivideNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopDivPush();
}

void DivideNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Divide" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

LessNode::LessNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int LessNode::Evaluate() const {
    return left->Evaluate() < right->Evaluate() ? 1 : 0;
}

void LessNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopLessPush();
}

void LessNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Less" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

LessEqualNode::LessEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int LessEqualNode::Evaluate() const {
    return left->Evaluate() <= right->Evaluate() ? 1 : 0;
}

void LessEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopLessEqualPush();
}

void LessEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "LessEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

GreaterNode::GreaterNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int GreaterNode::Evaluate() const {
    return left->Evaluate() > right->Evaluate() ? 1 : 0;
}

void GreaterNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopGreaterPush();
}

void GreaterNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Greater" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

GreaterEqualNode::GreaterEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int GreaterEqualNode::Evaluate() const {
    return left->Evaluate() >= right->Evaluate() ? 1 : 0;
}

void GreaterEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopGreaterEqualPush();
}

void GreaterEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "GreaterEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

EqualNode::EqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int EqualNode::Evaluate() const {
    return left->Evaluate() == right->Evaluate() ? 1 : 0;
}

void EqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopEqualPush();
}

void EqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Equal" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

NotEqualNode::NotEqualNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int NotEqualNode::Evaluate() const {
    return left->Evaluate() != right->Evaluate() ? 1 : 0;
}

void NotEqualNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopNotEqualPush();
}

void NotEqualNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "NotEqual" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

ModNode::ModNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int ModNode::Evaluate() const {
    int rightValue = right->Evaluate();
    if (rightValue == 0) {
        throw std::runtime_error("Division by zero error");
    }
    return left->Evaluate() % rightValue;
}

void ModNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
}
    

void ModNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Mod" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

AndNode::AndNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

void AndNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopAndPush();
}
int AndNode::Evaluate() const
{
    return left->Evaluate() && right->Evaluate() ? 1 : 0;
}

void AndNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "And" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

OrNode::OrNode(ExpressionNode* left, ExpressionNode* right) : BinaryOperatorNode(left, right) {}

int OrNode::Evaluate() const {
    return left->Evaluate() || right->Evaluate() ? 1 : 0;
}

void OrNode::CodeEvaluate(InstructionsClass &machineCode)
{
    left ->CodeEvaluate(machineCode);
    right->CodeEvaluate(machineCode);
    machineCode.PopPopOrPush();
}

void OrNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Or" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}

void NullStatementNode::Code(InstructionsClass &machineCode)
{
}
