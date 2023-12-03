#include<bits/stdc++.h>
#include "Tokens.h"
#include "TokenType.h"

using namespace std;

enum class NodeType {
    Program,
    NumericLiteral,
    Identifier,
    BinaryExpr,
    _return,
    VariableDeclaration,
    String,
    Semicolon,
    Block,
    Parameter,
    FunctionDeclaration,
    FunctionCall,
    For,
    While,
    If,
    UnaryExpr,
    Char,
    FloatLiterail,
    SwitchStmt,
    CaseStmt,
    DefaultStmt,
    CloseCurlyBrace,
    ForStmt
};

struct Expr;
struct Stmt;

struct Stmt {
    NodeType kind;
};

struct Expr : Stmt {};

struct BinaryExpr : Expr {
    Expr* left;
    Expr* right;
    TokenType operatorType;
};

struct Identifier : Expr {
    string symbol;
};

struct NumericLiteral : Expr {
    double value;
};

struct FloatLiteral : Expr {
    double value;
};

struct VariableDeclaration : Stmt {
    Identifier* identifier;
    TokenType* type;
    Expr* initializer;
};

struct String : Stmt {
    string symbol;
};

struct Char : Stmt {
    string symbol;
};

struct Semicolon : Stmt {
    string symbol;
};

struct Block : Stmt {
    vector<Stmt*> statemants;
};

struct Parameter : Expr {
    Identifier* identifier;
};

struct FunctionDeclaration : Stmt {
    Identifier* identifier;
    vector<Parameter*> parameters;
    Block* body;
};

struct FunctionCall : Expr {
    Identifier* identifier;
    vector<Expr*> arguments;
};

struct While : Stmt {
    Expr* condition;
    Block* body;
};

struct If : Stmt {
    Expr* condition;
    Black* thenBranch;
    Block* elseBranch;
};

struct UnaryExpr : Stmt {
    TokenType operatorType;
};

struct SwitchStmt : Stmt {
    NodeType kind;
    Expr* expression;
    vector<CaseStmt*> cases;
    Block* defaultBlock;
};

struct CaseStmt : Stmt {
    NodeType kind;
    Expr* value;
    vector<Stmt*> body;
};

struct DefaultStmt : Stmt {
    NodeType kind;
    vector<Stmt*> body;
};

struct ForStmt : Stmt {
    NodeType kind;
    Stmt* initialization;
    Expr* condition;
    Expr* increment;
    Block* body;
};
