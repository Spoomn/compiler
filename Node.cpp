#include "Node.h"
#include "Symbol.h"
#include "Debug.h"
#include <cmath>

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

    unsigned char* skipThenAddr = machineCode.SkipIfZeroStack();
    unsigned char* thenStart   = machineCode.GetAddress();

    thenStmt->Code(machineCode);

    unsigned char* jumpOverElse = machineCode.Jump();
    unsigned char* elseStart    = machineCode.GetAddress();

    machineCode.SetOffset(skipThenAddr,
                         static_cast<int>(elseStart - thenStart));

    if (elseStmt) {
        elseStmt->Code(machineCode);
    }
    unsigned char* afterElse = machineCode.GetAddress();

    machineCode.SetOffset(jumpOverElse,
                         static_cast<int>(afterElse - elseStart));
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

DoWhileStatementNode::DoWhileStatementNode(StatementNode* body, ExpressionNode* condition) 
    : body(body), condition(condition) {}

DoWhileStatementNode::~DoWhileStatementNode() {
    MSG("Deleting DoStatementNode\n");
    delete condition;
    delete body;
}

void DoWhileStatementNode::Interpret() const {
    do {
        body->Interpret();
    } while (condition->Evaluate());
}

void DoWhileStatementNode::Code(InstructionsClass &machineCode)
{
    unsigned char* address1 = machineCode.GetAddress();

    body->Code(machineCode);

    condition->CodeEvaluate(machineCode);

    unsigned char* insertSkip = machineCode.SkipIfZeroStack();
    unsigned char* address2 = machineCode.GetAddress();

    unsigned char* insertJump = machineCode.Jump();
    unsigned char* address3 = machineCode.GetAddress();

    machineCode.SetOffset(insertSkip, static_cast<int>(address3 - address2));

    machineCode.SetOffset(insertJump, static_cast<int>(address1 - address3));
}

void DoWhileStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "DoWhileStatement" << std::endl;
    body->PrintTree(indent + 1);
    condition->PrintTree(indent + 1);
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
    int slot = machineCode.AllocateSlot();

    expression->CodeEvaluate(machineCode);
    machineCode.PopAndStore(slot);

    unsigned char* loopHead = machineCode.GetAddress();

    machineCode.PushVariable(slot);
    unsigned char* skipAddr = machineCode.SkipIfZeroStack();
    unsigned char* bodyStart = machineCode.GetAddress();

    statementGroup->Code(machineCode);

    machineCode.PushVariable(slot);
    machineCode.PushValue(1);
    machineCode.PopPopSubPush();
    machineCode.PopAndStore(slot);

    unsigned char* backJump = machineCode.Jump();
    unsigned char* afterLoop = machineCode.GetAddress();

    machineCode.SetOffset(skipAddr,
                         static_cast<int>(afterLoop - bodyStart));
    machineCode.SetOffset(backJump,
                         static_cast<int>(loopHead - afterLoop));
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

CoutStatementNode::CoutStatementNode(const std::vector<ExpressionNode *> &items) : items(items) {}

CoutStatementNode::~CoutStatementNode()
{
    MSG("Deleting CoutStatementNode\n");
    for (auto ptr : items){
        if (ptr) delete ptr;
    }
}

void CoutStatementNode::Interpret() const {
    for (auto ptr : items) {
        if (ptr) {
            std::cout << ptr->Evaluate() << " ";
        }
        else {
            std::cout << std::endl;
        }

    }
}

void CoutStatementNode::Code(InstructionsClass &machineCode)
{
    for (auto ptr : items){
        if (ptr){
            ptr->CodeEvaluate(machineCode);
            machineCode.PopAndWrite();
        }else{
            machineCode.WriteEndLinux64();
        }
    }
}

void CoutStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "CoutChain" << std::endl;
    for (auto ptr : items) {
        if (ptr) ptr->PrintTree(indent+1);
        else {
            for (int i = 0; i < indent+1; i++) std::cout << "  ";
            std::cout << "endl" << std::endl;
        }
    }
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
    machineCode.PopPopModPush();
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


PlusEqualsStatementNode::PlusEqualsStatementNode(IdentifierNode* id, ExpressionNode* expr)
  : identifier(id), expression(expr) {}

PlusEqualsStatementNode::~PlusEqualsStatementNode() {
    delete identifier;
    delete expression;
}

void PlusEqualsStatementNode::PrintTree(int indent) const
{
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "PlusEqualsStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
    if (expression) {
        expression->PrintTree(indent + 1);
    }
}

void PlusEqualsStatementNode::Interpret() const {
    int old = identifier->Evaluate();
    int right = expression->Evaluate();
    identifier->SetValue(old + right);
}

void PlusEqualsStatementNode::Code(InstructionsClass &machineCode) {
    machineCode.PushVariable(identifier->GetIndex());
    expression->CodeEvaluate(machineCode);
    machineCode.PopPopAddPush();
    machineCode.PopAndStore(identifier->GetIndex());
}

MinusEqualsStatementNode::MinusEqualsStatementNode(IdentifierNode* id, ExpressionNode* expr)
  : identifier(id), expression(expr) {}

MinusEqualsStatementNode::~MinusEqualsStatementNode() {
    delete identifier;
    delete expression;

}
void MinusEqualsStatementNode::PrintTree(int indent) const
{
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "MinusEqualsStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
    if (expression) {
        expression->PrintTree(indent + 1);
    }
}
void MinusEqualsStatementNode::Interpret() const {
    int old = identifier->Evaluate();
    int right = expression->Evaluate();
    identifier->SetValue(old - right);
}
void MinusEqualsStatementNode::Code(InstructionsClass &machineCode) {
    machineCode.PushVariable(identifier->GetIndex());
    expression->CodeEvaluate(machineCode);
    machineCode.PopPopSubPush();
    machineCode.PopAndStore(identifier->GetIndex());
}

ForStatementNode::ForStatementNode(StatementNode* init,
    ExpressionNode* cond,
    StatementNode* step,
    StatementNode* body)
: initStmt(init), condition(cond), stepStmt(step), body(body) {}

ForStatementNode::~ForStatementNode() {
delete initStmt;
delete condition;
delete stepStmt;
delete body;
}

void ForStatementNode::Interpret() const {
    if (initStmt) initStmt->Interpret();
    while (!condition || condition->Evaluate()) {
        body->Interpret();
        if (stepStmt) stepStmt->Interpret();
    }
}

void ForStatementNode::Code(InstructionsClass& machineCode) {
    if (initStmt) initStmt->Code(machineCode);

    unsigned char* address1 = machineCode.GetAddress();

    if (condition) {
        condition->CodeEvaluate(machineCode);
    }
    unsigned char* insertSkip = condition
    ? machineCode.SkipIfZeroStack()
    : nullptr;
    unsigned char* address2 = machineCode.GetAddress();

    body->Code(machineCode);

    if (stepStmt) stepStmt->Code(machineCode);

    unsigned char* insertJump = machineCode.Jump();
    unsigned char* address3    = machineCode.GetAddress();

    machineCode.SetOffset(insertJump,
    static_cast<int>(address1 - address3));
    if (condition) {
    machineCode.SetOffset(insertSkip,
    static_cast<int>(address3 - address2));
    }
}

void ForStatementNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
        std::cout << "ForStatement" << std::endl;
        if (initStmt)   initStmt->PrintTree(indent + 1);
        if (condition)  condition->PrintTree(indent + 1);
        if (stepStmt)   stepStmt->PrintTree(indent + 1);
        if (body)       body->PrintTree(indent + 1);
}

PlusPlusStatementNode::PlusPlusStatementNode(IdentifierNode *id)
  : identifier(id) {}

PlusPlusStatementNode::~PlusPlusStatementNode() {
    delete identifier;
}

void PlusPlusStatementNode::PrintTree(int indent) const
{
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "PlusPlusStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
}

void PlusPlusStatementNode::Interpret() const {
    int old = identifier->Evaluate();
    identifier->SetValue(old + 1);
}

void PlusPlusStatementNode::Code(InstructionsClass &machineCode) {
    machineCode.PushVariable(identifier->GetIndex());
    machineCode.PushValue(1);
    machineCode.PopPopAddPush();
    machineCode.PopAndStore(identifier->GetIndex());
}

MinusMinusStatementNode::MinusMinusStatementNode(IdentifierNode *id)
  : identifier(id) {}

MinusMinusStatementNode::~MinusMinusStatementNode() {
    delete identifier;
}

void MinusMinusStatementNode::PrintTree(int indent) const
{
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "MinusMinusStatement" << std::endl;
    if (identifier) {
        identifier->PrintTree(indent + 1);
    }
}

void MinusMinusStatementNode::Interpret() const {
    int old = identifier->Evaluate();
    identifier->SetValue(old - 1);
}

void MinusMinusStatementNode::Code(InstructionsClass &machineCode) {
    machineCode.PushVariable(identifier->GetIndex());
    machineCode.PushValue(1);
    machineCode.PopPopSubPush();
    machineCode.PopAndStore(identifier->GetIndex());
}

ExponentNode::ExponentNode(ExpressionNode *left, ExpressionNode *right): BinaryOperatorNode(left, right) {}

void ExponentNode::CodeEvaluate(InstructionsClass &mc) {
    int result = this->Evaluate();
    mc.PushValue(result);
}

int ExponentNode::Evaluate() const
{
    return pow(left->Evaluate(), right->Evaluate());
}

void ExponentNode::PrintTree(int indent) const {
    for (int i = 0; i < indent; i++) std::cout << "  ";
    std::cout << "Exponent" << std::endl;
    if (left) left->PrintTree(indent + 1);
    if (right) right->PrintTree(indent + 1);
}