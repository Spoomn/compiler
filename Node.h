#pragma once
#include <vector>
#include <string>
#include <iostream>
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
        virtual void PrintTree(int indent = 0) const = 0;
        virtual void Interpret() const = 0;

};

class StartNode : public Node {
    public:
        StartNode(ProgramNode* program);
        ~StartNode() ;
        virtual void PrintTree(int indent = 0) const override;
        virtual void Interpret() const override;
    private:
        ProgramNode* program;
};

class ProgramNode : public Node {
    public:
        ProgramNode(BlockNode* block);
        ~ProgramNode();
        virtual void PrintTree(int indent = 0) const override;
        virtual void Interpret() const override;
    private:
        BlockNode* block;
};

class StatementGroupNode : public Node {
    public:
        void AddStatement(StatementNode* statement);
        ~StatementGroupNode();
        virtual void PrintTree(int indent = 0) const override;
        virtual void Interpret() const override;
    private:
        std::vector<StatementNode*> statements;
};

// STATEMENT NODES
// These nodes are used to represent statements in the parse tree.
class StatementNode : public Node {
    public:
        virtual ~StatementNode() {};
};

class BlockNode : public StatementNode {
    public:
        BlockNode(StatementGroupNode* statementGroup);
        ~BlockNode();
        virtual void PrintTree(int indent = 0) const override;
        virtual void Interpret() const override;
    private:
        StatementGroupNode* statementGroup;
};

class DeclarationStatementNode : public StatementNode {
    public:
        DeclarationStatementNode(IdentifierNode* identifier, ExpressionNode* expression);
        ~DeclarationStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        IdentifierNode* identifier;
        ExpressionNode* expression;
};

// These nodes are used to represent assignment statements in the parse tree.
class AssignmentStatementNode : public StatementNode {
    public:
        AssignmentStatementNode(IdentifierNode* identifier, ExpressionNode* expression);
        ~AssignmentStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        IdentifierNode* identifier;
        ExpressionNode* expression;
};

class CoutStatementNode : public StatementNode {
    public:
        CoutStatementNode(ExpressionNode* expression);
        ~CoutStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        ExpressionNode* expression;
};

class IfStatementNode : public StatementNode {
    public:
        IfStatementNode(ExpressionNode* condition, StatementNode* trueBlock, StatementNode* falseBlock);
        ~IfStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        ExpressionNode* condition;
        StatementNode* thenStmt;
        StatementNode* elseStmt;
};

class WhileStatementNode : public StatementNode {
    public:
        WhileStatementNode(ExpressionNode* condition, StatementNode* body);
        ~WhileStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        ExpressionNode* condition;
        StatementNode* body;
};

class RepeatStatementNode : public StatementNode {
    public:
        RepeatStatementNode(ExpressionNode* expression, StatementGroupNode* statementGroup);
        ~RepeatStatementNode();
        void virtual PrintTree(int indent = 0) const override;
        void virtual Interpret() const override;
    private:
        ExpressionNode* expression;
        StatementGroupNode* statementGroup;
};

class NullStatementNode : public StatementNode {
    public:
        NullStatementNode() {}
        virtual ~NullStatementNode() {}
        virtual void Interpret() const override {
            // Empty statement: do nothing.
        }
        virtual void PrintTree(int indent) const override {
            for (int i = 0; i < indent; i++) std::cout << "  ";
            std::cout << "NullStatement" << std::endl;
        }
    };

// EXPRESSION NODES
// These nodes are used to represent expressions in the parse tree.
class ExpressionNode {
    
    public:
        virtual int Evaluate() const = 0;    
        virtual ~ExpressionNode();
        virtual void PrintTree(int indent = 0) const = 0;
    };

    
class IdentifierNode : public ExpressionNode {
    public:
        IdentifierNode(const std::string& label, SymbolTableClass* symbolTable);

        void DeclareVariable() const;
        void SetValue(int v) const;
        int GetIndex() const;
        int Evaluate() const override;

        void virtual PrintTree(int indent = 0) const override;
    private:
        std::string label;
        SymbolTableClass* symbolTable;
};


class IntegerNode : public ExpressionNode {
    public:
        IntegerNode(int value);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;
    private:
        int value;
};

class BinaryOperatorNode : public ExpressionNode {
    public:
        BinaryOperatorNode(ExpressionNode* left, ExpressionNode* right);
        virtual ~BinaryOperatorNode();
        void virtual PrintTree(int indent = 0) const override;
    protected:
        ExpressionNode* left;
        ExpressionNode* right;
};

class PlusNode : public BinaryOperatorNode {
    public:
        PlusNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;
};

class MinusNode : public BinaryOperatorNode {
    public:
        MinusNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;
};

class TimesNode : public BinaryOperatorNode {
    public:
        TimesNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class DivideNode : public BinaryOperatorNode {
    public:
        DivideNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class LessNode : public BinaryOperatorNode {
    public:
        LessNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class LessEqualNode : public BinaryOperatorNode {
    public:
        LessEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class GreaterNode : public BinaryOperatorNode {
    public:
        GreaterNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class GreaterEqualNode : public BinaryOperatorNode {
    public:
        GreaterEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class EqualNode : public BinaryOperatorNode {
    public:
        EqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class NotEqualNode : public BinaryOperatorNode {
    public:
        NotEqualNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class ModNode : public BinaryOperatorNode {
    public:
        ModNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class AndNode : public BinaryOperatorNode {
    public:
        AndNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};

class OrNode : public BinaryOperatorNode {
    public:
        OrNode(ExpressionNode* left, ExpressionNode* right);
        int Evaluate() const override;
        void virtual PrintTree(int indent = 0) const override;

};