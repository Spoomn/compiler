#include "Parser.h"
#include "Debug.h"
#include <vector>
#include <iostream>
#include <cstdlib>

ParserClass::ParserClass(ScannerClass* scanner, SymbolTableClass* symTab)
    : mScanner(scanner), mSymTab(symTab)
{
}

TokenClass ParserClass::Match(TokenType expectedType) {
    TokenClass currentToken = mScanner->GetNextToken();
    MSG("\tCurrent Token: " << currentToken.GetTokenTypeName() << " (" << static_cast<int>(currentToken.GetTokenType())
        << ") . Lexeme: \"" << currentToken.GetLexeme() << "\"");

    if (currentToken.GetTokenType() != expectedType) {
        std::cerr << "Error in ParserClass::Match on line " << mScanner->GetLineNumber() << ": "
                  << "Expected token type "
                  << TokenClass::GetTokenTypeName(expectedType)
                  << ", but got type " << currentToken.GetTokenTypeName()
                  << " with lexeme \"" << currentToken.GetLexeme() << "\"." << std::endl;
        std::exit(1);
    }
    MSG("\tSuccessfully matched Token Type: " << currentToken.GetTokenTypeName() << ". Lexeme: \"" << currentToken.GetLexeme() << "\"");
    return currentToken;
}

StartNode * ParserClass::Start() {
    ProgramNode* programNode = Program();
    Match(ENDFILE_TOKEN);
    StartNode* startNode = new StartNode(programNode);
    return startNode;
}

ProgramNode* ParserClass::Program() {
    Match(VOID_TOKEN);
    Match(MAIN_TOKEN);
    Match(LPAREN_TOKEN);
    Match(RPAREN_TOKEN);
    
    BlockNode* blockNode = Block();
    ProgramNode* programNode = new ProgramNode(blockNode);
    return programNode;
}

BlockNode* ParserClass::Block() {
    Match(LCURLY_TOKEN);
    StatementGroupNode* statementGroupNode = StatementGroup();
    Match(RCURLY_TOKEN);
    BlockNode* blockNode = new BlockNode(statementGroupNode);
    return blockNode;
}

StatementGroupNode* ParserClass::StatementGroup() {
    StatementGroupNode* stmtGroup = new StatementGroupNode();
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    while (tt == INT_TOKEN || tt == IDENTIFIER_TOKEN || tt == COUT_TOKEN || tt == LCURLY_TOKEN ||
         tt == IF_TOKEN || tt == FOR_TOKEN || tt == WHILE_TOKEN || tt == DO_TOKEN || tt == REPEAT_TOKEN || tt == SEMICOLON_TOKEN) {
        StatementNode* stmt = Statement();
        if (stmt != nullptr) {
            stmtGroup->AddStatement(stmt);
        }
        token = mScanner->PeekNextToken();
        tt = token.GetTokenType();
    }
    return stmtGroup;
}

StatementNode* ParserClass::Statement() {
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    MSG("\tParsing Statement: " << token.GetTokenTypeName() << " with lexeme \"" << token.GetLexeme() << "\"");
    if (tt == INT_TOKEN) {
        MSG("Statement recognized as DeclarationStatement");
        return DeclarationStatement();
    } else if (tt == IDENTIFIER_TOKEN) {
        MSG("Statement recognized as AssignmentOrCompoundStatement");
        return AssignmentOrCompoundStatement();
    } else if (tt == COUT_TOKEN) {
        MSG("Statement recognized as CoutStatement");
        return CoutStatement();
    } else if (tt == IF_TOKEN) {
        MSG("Statement recognized as IfStatement");
        return IfStatement();
    } else if (tt == FOR_TOKEN) {
        MSG("Statement recognized as ForStatement");
        return ForStatement();
    } else if (tt == WHILE_TOKEN) {
        MSG("Statement recognized as WhileStatement");
        return WhileStatement();
    } else if (tt == DO_TOKEN) {
        MSG("Statement recognized as DoWhileStatement");
        return DoWhileStatement(); 
    } else if (tt == LCURLY_TOKEN) {
        MSG("Statement recognized as Block (curly)");
        return Block();
    } else if (tt == REPEAT_TOKEN) {
        MSG("Statement recognized as RepeatStatement");
        return RepeatStatement();
    } else if (tt == SEMICOLON_TOKEN){
        Match(SEMICOLON_TOKEN);
        MSG("Statement recognized as SemiColon");
        return new NullStatementNode();
    } else {
        MSG("Statement not recognized, returning null.");
        return nullptr;
    }
}

DeclarationStatementNode* ParserClass::DeclarationStatement() {
    Match(INT_TOKEN);
    IdentifierNode* idNode = Identifier();
    ExpressionNode* expr = nullptr;
    if (mScanner->PeekNextToken().GetTokenType() == ASSIGNMENT_TOKEN){
        Match(ASSIGNMENT_TOKEN);
        expr = Expression();
    }
    Match(SEMICOLON_TOKEN);
    DeclarationStatementNode* declStmt = new DeclarationStatementNode(idNode, expr);
    return declStmt;
}

// AssignmentStatementNode* ParserClass::AssignmentStatement() {
//     IdentifierNode* idNode = Identifier();
//     Match(ASSIGNMENT_TOKEN);
//     ExpressionNode* expr = Expression();
//     Match(SEMICOLON_TOKEN);
//     AssignmentStatementNode* assignStmt = new AssignmentStatementNode(idNode, expr);
//     return assignStmt;
// }

StatementNode* ParserClass::AssignmentOrCompoundStatement() {
    IdentifierNode* id = Identifier();
    TokenType tt = mScanner->PeekNextToken().GetTokenType();
    if (tt == ASSIGNMENT_TOKEN) {
        Match(ASSIGNMENT_TOKEN);
        ExpressionNode* rhs = Expression();
        Match(SEMICOLON_TOKEN);
        return new AssignmentStatementNode(id, rhs);
    }
    else if (tt == PLUS_EQUAL_TOKEN) {
        Match(PLUS_EQUAL_TOKEN);
        ExpressionNode* rhs = Expression();
        Match(SEMICOLON_TOKEN);
        return new PlusEqualsStatementNode(id, rhs);
    }
    else if (tt == MINUS_EQUAL_TOKEN) {
        Match(MINUS_EQUAL_TOKEN);
        ExpressionNode* rhs = Expression();
        Match(SEMICOLON_TOKEN);
        return new MinusEqualsStatementNode(id, rhs);
    }
    else if (tt == PLUS_PLUS_TOKEN) {
        Match(PLUS_PLUS_TOKEN);
        Match(SEMICOLON_TOKEN);
        return new PlusPlusStatementNode(id);
    }
    else if (tt == MINUS_MINUS_TOKEN) {
        Match(MINUS_MINUS_TOKEN);
        Match(SEMICOLON_TOKEN);
        return new MinusMinusStatementNode(id);
    }
    else {
        std::cerr << "Error: expected =, +=, -=, ++, or -- after identifier\n";
        std::exit(1);
    }
}

CoutStatementNode *ParserClass::CoutStatement()
{
    Match(COUT_TOKEN);
    std::vector<ExpressionNode*> items;
    do{
        Match(INSERTION_TOKEN);
        TokenClass next = mScanner->PeekNextToken();
        if(next.GetTokenType() == ENDL_TOKEN){
            Match(ENDL_TOKEN);
            items.push_back(nullptr);

        }else{
            ExpressionNode* expr = Expression();
            items.push_back(expr);
        }

    }
    while(mScanner->PeekNextToken().GetTokenType() == INSERTION_TOKEN);

    Match(SEMICOLON_TOKEN);
    return new CoutStatementNode(items);
}

StatementNode* ParserClass::IfStatement() {
    Match(IF_TOKEN);            
    Match(LPAREN_TOKEN);        
    ExpressionNode* condition = Expression();
    Match(RPAREN_TOKEN);        
    
    StatementNode* thenStmt = Statement();
    
    StatementNode* elseStmt = nullptr;
    if (mScanner->PeekNextToken().GetTokenType() == ELSE_TOKEN) {
        Match(ELSE_TOKEN);
        elseStmt = Statement();
    }
    
    return new IfStatementNode(condition, thenStmt, elseStmt);
}

StatementNode* ParserClass::ForStatement() {
    Match(FOR_TOKEN);
    Match(LPAREN_TOKEN);

    StatementNode* initStmt = nullptr;
    if (mScanner->PeekNextToken().GetTokenType() != SEMICOLON_TOKEN) {
        if (mScanner->PeekNextToken().GetTokenType() == INT_TOKEN) {
            initStmt = DeclarationStatement();
        } else {
            IdentifierNode* id = Identifier();
            Match(ASSIGNMENT_TOKEN);
            ExpressionNode* expr = Expression();
            initStmt = new AssignmentStatementNode(id, expr);
            Match(SEMICOLON_TOKEN);
        }
    } else {
        Match(SEMICOLON_TOKEN);
    }

    ExpressionNode* condExpr = nullptr;
    if (mScanner->PeekNextToken().GetTokenType() != SEMICOLON_TOKEN) {
        condExpr = Expression();
    }
    Match(SEMICOLON_TOKEN);

    StatementNode* stepStmt = nullptr;
    if (mScanner->PeekNextToken().GetTokenType() != RPAREN_TOKEN) {
        IdentifierNode* id2 = Identifier();
        TokenType tt2 = mScanner->PeekNextToken().GetTokenType();
        if (tt2 == ASSIGNMENT_TOKEN) {
            Match(ASSIGNMENT_TOKEN);
            ExpressionNode* expr2 = Expression();
            stepStmt = new AssignmentStatementNode(id2, expr2);
        } else if (tt2 == PLUS_EQUAL_TOKEN) {
            Match(PLUS_EQUAL_TOKEN);
            ExpressionNode* expr2 = Expression();
            stepStmt = new PlusEqualsStatementNode(id2, expr2);
        } else if (tt2 == MINUS_EQUAL_TOKEN) {
            Match(MINUS_EQUAL_TOKEN);
            ExpressionNode* expr2 = Expression();
            stepStmt = new MinusEqualsStatementNode(id2, expr2);
        } else if (tt2 == PLUS_PLUS_TOKEN) {
            Match(PLUS_PLUS_TOKEN);
            stepStmt = new PlusPlusStatementNode(id2);
        } else if (tt2 == MINUS_MINUS_TOKEN) {
            Match(MINUS_MINUS_TOKEN);
            stepStmt = new MinusMinusStatementNode(id2);
        } else {
            std::cerr << "Error in ForStatement: expected assignment or increment/decrement operator after identifier\n";
            std::exit(1);
        }
    }
    Match(RPAREN_TOKEN);

    StatementNode* body = Statement();

    return new ForStatementNode(initStmt, condExpr, stepStmt, body);
}

StatementNode* ParserClass::WhileStatement() {
    Match(WHILE_TOKEN);
    Match(LPAREN_TOKEN);
    ExpressionNode* condition = Expression();
    Match(RPAREN_TOKEN);
    
    StatementNode* body = Statement();
    
    return new WhileStatementNode(condition, body);
}

StatementNode* ParserClass::DoWhileStatement() {
    Match(DO_TOKEN);
    StatementNode* body = Statement();
    Match(WHILE_TOKEN);
    Match(LPAREN_TOKEN);
    ExpressionNode* condition = Expression();
    Match(RPAREN_TOKEN);
    // Optional semicolon
    if (mScanner->PeekNextToken().GetTokenType() == SEMICOLON_TOKEN) {
        Match(SEMICOLON_TOKEN);
    }
    
    return new DoWhileStatementNode(body, condition);
}


StatementNode* ParserClass::RepeatStatement() {
    Match(REPEAT_TOKEN);
    Match(LPAREN_TOKEN);
    ExpressionNode* expr = Expression();
    Match(RPAREN_TOKEN);
    Match(LCURLY_TOKEN);
    StatementGroupNode* stmtGroup = StatementGroup();
    Match(RCURLY_TOKEN);
    
    return new RepeatStatementNode(expr, stmtGroup);
}

IdentifierNode* ParserClass::Identifier() {
    TokenClass token = Match(IDENTIFIER_TOKEN);
    IdentifierNode* idNode = new IdentifierNode(token.GetLexeme(), mSymTab);
    return idNode;

}

IntegerNode* ParserClass::Integer() {
    TokenClass token = Match(INTEGER_TOKEN);
    int value = std::atoi(token.GetLexeme().c_str());
    IntegerNode* intNode = new IntegerNode(value);
    return intNode;
}

// ExpressionNode Order of Operations:
// Expression -> Or -> And -> Relational -> PlusMinus -> TimesDivide -> Power -> Factor
ExpressionNode* ParserClass::Expression() {
    return Or();
}

ExpressionNode* ParserClass::Relational() {
    ExpressionNode* left = PlusMinus();
    
    TokenType tt = mScanner->PeekNextToken().GetTokenType();
    
    if (tt == LESS_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new LessNode(left, right);
    }
    else if (tt == LESSEQUAL_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new LessEqualNode(left, right);
    }
    else if (tt == GREATER_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new GreaterNode(left, right);
    }
    else if (tt == GREATEREQUAL_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new GreaterEqualNode(left, right);
    }
    else if (tt == EQUAL_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new EqualNode(left, right);
    }
    else if (tt == NOTEQUAL_TOKEN) {
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new NotEqualNode(left, right);
    }
    else if (tt == MOD_TOKEN){
        Match(tt);
        ExpressionNode* right = PlusMinus();
        return new ModNode(left, right);
    }
    
    return left;
}

ExpressionNode* ParserClass::PlusMinus(){
    ExpressionNode* current = TimesDivide();

    while(true){
        TokenType tt= mScanner->PeekNextToken().GetTokenType();
        if(tt== PLUS_TOKEN){
            Match(tt);
            current = new PlusNode(current, TimesDivide());
        }
        else if (tt==MINUS_TOKEN){
            Match(tt);
            current = new MinusNode(current, TimesDivide());
        }
        else{
            return current;
        }
    }
}

ExpressionNode* ParserClass::TimesDivide(){
    ExpressionNode* current = Power();

    while(true){
        TokenType tt= mScanner->PeekNextToken().GetTokenType();
        if(tt== TIMES_TOKEN){
            Match(tt);
            current = new TimesNode(current, Power());
        }
        else if (tt==DIVIDE_TOKEN){
            Match(tt);
            current = new DivideNode(current, Power());
        }
        else{
            return current;
        }
    }
}

ExpressionNode* ParserClass::Factor() {
    TokenClass token = mScanner->PeekNextToken();
    TokenType tt = token.GetTokenType();
    
    if (tt == IDENTIFIER_TOKEN) {
        return Identifier();
    } else if (tt == INTEGER_TOKEN) {
        return Integer();
    } else if (tt == LPAREN_TOKEN) {
        Match(LPAREN_TOKEN);
        ExpressionNode* expr = Expression();
        Match(RPAREN_TOKEN);
        return expr;
    } else {
        std::cerr << "Error in ParserClass::Factor: unexpected token "
                  << token.GetTokenTypeName() << " with lexeme \"" 
                  << token.GetLexeme() << "\"." << std::endl;
        std::exit(1);
    }
}

ExpressionNode *ParserClass::Power()
{
    ExpressionNode* current = Factor();
    while (true) {
        TokenType tt = mScanner->PeekNextToken().GetTokenType();
        if (tt == POWER_TOKEN) {
            Match(tt);
            current = new ExponentNode(current, Factor());
        } else {
            return current;
        }
    }    
}

ExpressionNode* ParserClass::And(){
    ExpressionNode* left = Relational();
    while(true){
        TokenType tt = mScanner->PeekNextToken().GetTokenType();
        MSG("And() peek token: " << gTokenTypeNames[tt]);
        if (tt == AND_TOKEN) {
            Match(tt);
            left = new AndNode(left, Relational());
        } else {
            return left;
        }
    }
}

ExpressionNode* ParserClass::Or(){
    ExpressionNode* left = And();
    while(true){
        TokenType tt = mScanner->PeekNextToken().GetTokenType();
        MSG("Or() peek token: " << gTokenTypeNames[tt]);
        if (tt == OR_TOKEN) {
            Match(tt);
            left = new OrNode(left, And());
        } else {
            return left;
        }
    }
}
