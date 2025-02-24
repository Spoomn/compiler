#pragma once
#include <vector>
#include <string>
#include "Symbol.h"
class Node;
class StartNode;
class ProgramNode;
class BlockNode;
class StatementGroupNode;
class StatementNode;
class DeclarationStatementNode;
class AssignmentStatementNode;
class CoutStatementNode;
class ExpressionNode;
class IntegerNode;
class IdentifierNode;
class BinaryOperatorNode;
class PlusNode;
class MinusNode;
class TimesNode;
class DivideNode;
class LessNode;
class LessEqualNode;
class GreaterNode;
class GreaterEqualNode;
class EqualNode;
class NotEqualNode;

class Node {
    public:
        virtual ~Node() {};
};

class StartNode : public Node {
    public:
        StartNode(ProgramNode* program);
        ~StartNode();
    private:
        ProgramNode* program;
};

class ProgramNode : public Node {
    public:
        ProgramNode(BlockNode* block);
        ~ProgramNode();
    private:
        BlockNode* block;
};

class BlockNode : public Node {
    public:
        BlockNode(StatementGroupNode* statementGroup);
        ~BlockNode();
    private:
        StatementGroupNode* statementGroup;
};

class StatementGroupNode : public Node {
    public:
        void AddStatement(StatementNode* statement);
        ~StatementGroupNode();
    private:
        std::vector<StatementNode*> statements;
};

class StatementNode : public Node {
    public:
        virtual ~StatementNode() {};
};

class DeclarationStatementNode : public StatementNode {
    public:
        DeclarationStatementNode(IdentifierNode* identifier);
        ~DeclarationStatementNode();
    private:
        IdentifierNode* identifier;
};

class AssignmentStatementNode : public StatementNode {
    public:
        AssignmentStatementNode(IdentifierNode* identifier, ExpressionNode* expression);
        ~AssignmentStatementNode();
    private:
        IdentifierNode* identifier;
        ExpressionNode* expression;
};

class ExpressionNode {
    public:
        virtual int Evaluate() const = 0;    
        virtual ~ExpressionNode() {};
    };

    
class IdentifierNode : public ExpressionNode {
    public:
        IdentifierNode(const std::string& label, SymbolTableClass* symbolTable);

        void DeclareVariable() const;
        void SetValue(int v) const;
        void GetIndex() const;
        int Evaluate() const override;
    private:
        std::string label;
        SymbolTableClass* symbolTable;
};

class CoutStatementNode : public StatementNode {
    public:
        CoutStatementNode(ExpressionNode* expression);
        ~CoutStatementNode();
    private:
        ExpressionNode* expression;
};

class IntegerNode : public ExpressionNode {
    public:
        IntegerNode(int value);
        int Evaluate() const override;
    private:
        int value;
};

class BinaryOperatorNode : public ExpressionNode {
    public:
        BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right);
        virtual ~BinaryOperatorNode();
    protected:
        ExpressionNode* left;
        ExpressionNode* right;
};

class PlusNode : public BinaryOperatorNode {
    public:
        PlusNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class MinusNode : public BinaryOperatorNode {
    public:
        MinusNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class TimesNode : public BinaryOperatorNode {
    public:
        TimesNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class DivideNode : public BinaryOperatorNode {
    public:
        DivideNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class LessNode : public BinaryOperatorNode {
    public:
        LessNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class LessEqualNode : public BinaryOperatorNode {
    public:
        LessEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class GreaterNode : public BinaryOperatorNode {
    public:
        GreaterNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class GreaterEqualNode : public BinaryOperatorNode {
    public:
        GreaterEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class EqualNode : public BinaryOperatorNode {
    public:
        EqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

class NotEqualNode : public BinaryOperatorNode {
    public:
        NotEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
};

