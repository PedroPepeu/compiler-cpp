#include <iostream>
#include <vector>
#include "Tokens.h"
#include "TokenType.h"
#include "ast.h"

using namespace std;
using namespace TokenType;

vector<Tokens> tokens;

class parser {

private:

bool not_eof() {
    return tokens[0].type != TokenType::EOF;
}

Tokens at() {
    return tokens[0];
}

Tokens eat() {
    Tokens prev = tokens[0];
    tokens.erase(tokens.begin());
    return prev;
}

Tokens expect(TokenType type, const string& err) {
    Tokens prev = tokens[0];
    if(!prev.value.empty() || prev.type != type) {
        cerr << "Parser Error:\n" << err << prev.value << " - Expecting: " << type << endl;
        exit(EXIT_FAILURE);
    }
    tokens.erase(tokens.begin());
    return prev;
}

Block parse_block() {
    expect(TokenType::OpenCurlyBrace, "Expecting '{' to start a block.");

    vector<Stmt> statements;

    while(currentToken().type != TokenType::CloseCurlyBrace && not_eof()) {
        statements.push_back(parse_stmt());
    }

    expect(TokenType::CloseCurlyBrace, "Expecting '}' to end a block.");

    return Block("Block", statements);
}

While parse_while() {
    expect(TokenType::While, "Expecting 'while' keyword.");

    Expr condition = parse_expr();
    Block body = parse_block();

    return While("While", condition, body);
}

If parse_if() {
    expect(TokenType::If, "Expecting 'if' keyword.");

    const condition = parse_expr();
    const thenBranch = parse_block();

    Block elseBranch;

    if(currentToken().type == TokenType::Else) {
        eat();
        elseBranch = parse_block();
    } else {
        elseBranch = Block{"EmptyBlock", {}};
    }

    return If("If", condition, thenBranch, elseBranch);
}

FunctionDeclaration parse_function_declaration() {
    expect(TokenType::Function, "Expecting 'function' keyword.");

    Identifier identifier = parse_identifier();

    expect(TokenType::OpenParen, "Expecting '(' after function identifier");

    vector<Parameter> parameters;

    while(currentToken().type != TokenType::CloseParen) {
        parameters.push_back({
                "Parameter",
                parse_function_declaration_parameters()
                });

        if(currentToken().type == TokenType::Comma) {
            eat();
        }
    }

    expect(TokenType::CloseParen, "Expecting ')' after function identifier");

    Block body = parse_block();

    return FunctionDeclaration{
        "FunctionDeclaration",
            identifier,
            parameters,
            body
    };
}

Identifier parse_function_delcaration_parameters() {
    TokenType paramType = currentToken().type;

    if(paramType == TokenType::Bool ||
            paramType == TokenType::String ||
            paramType == TokenType::Float ||
            paramType == TokenType::Int ||
            paramType == TokenType::Char) {
        
        TokenType type = eat().type;
        string symbol = eat().value;

        return Identifier{"Identifier", type, symbol};
    } else {
        cerr << "Parameter variable type is not correct: " << currentToken().value << endl;
        exit(EXIT_FAILURE);
    }
}

SwitchStmt parse_switch() {
    expect(TokenType::Switch, "Expecting 'switch' keyword.");

    Expr expression = parse_expr();
    expect(TokenType::OpenCurlyBrace, "Expecting '{' to start a switch block.");

    std::vector<CaseStmt> cases;
    std::vector<Stmt> defaultCase;

    while (currentToken().type != TokenType::CloseCurlyBrace) {
    
        if (currentToken().type == TokenType::Case) {
        
            eat();
            Expr value = parse_expr();
            expect(TokenType::Colon, "Expecting ':' after case expression.");
            std::vector<Stmt> body;

            while (currentToken().type != TokenType::Break) {
                body.push_back(parse_stmt());
            }
        
            eat();
            eat();
            cases.push_back(CaseStmt{"CaseStmt", value, body});
        
        } else if (currentToken().type == TokenType::Default) {
            
            eat();
            expect(TokenType::Colon, "Expecting ':' after 'default' keyword.");
            
            while (currentToken().type != TokenType::Break) {
                defaultCase.push_back(parse_stmt());
            
            }
            
            eat();
            eat();
        
        }
    }

    eat();

    return SwitchStmt{"SwitchStmt", expression, cases, defaultCase};
}

ForStmt parse_for() {
    expect(TokenType::For, "Expecting 'for' keyword.");

    expect(TokenType::OpenParen, "Expecting '(' after 'for' keyword.");

    VariableDeclaration initialization;
    Expr initializationExpr;
    
    if (currentToken().type != TokenType::Semi) {
        
        if (currentToken().type == TokenType::Var || currentToken().type == TokenType::Let || currentToken().type == TokenType::Const) {
            
            initialization = parse_variable_declaration();
        
        } else {
        
            initializationExpr = parse_expr();
        
        }
    }

    Expr condition = parse_for_condition();
    expect(TokenType::Semi, "Expecting ';' after condition.");

    Expr increment;
    
    if (currentToken().type != TokenType::CloseParen) {
        increment = parse_expr();
    }

    expect(TokenType::CloseParen, "Expecting ')' after increment.");

    Block body = parse_block();

    return ForStmt{"ForStmt", initialization, initializationExpr, condition, increment, body};
}

Expr parse_for_condition() {
    if (currentToken().type == TokenType::Identifier) {
        Identifier identifier = parse_identifier();
        if (currentToken().type == TokenType::LessThan) {
            eat();
            Expr value = parse_expr();
            return BinaryExpr{"BinaryExpr", identifier, value, TokenType::LessThan};
        } else {
            return identifier;
        }
    } else {
        return nullptr;
    }
}

Stmt parse_stmt() {
    if (currentToken().type == TokenType::Int ||
        currentToken().type == TokenType::Float ||
        currentToken().type == TokenType::Bool ||
        currentToken().type == TokenType::Char ||
        currentToken().type == TokenType::String) {

        TokenType type = (currentToken().type == TokenType::Int ||
                          currentToken().type == TokenType::Float ||
                          currentToken().type == TokenType::Bool ||
                          currentToken().type == TokenType::Char ||
                          currentToken().type == TokenType::String)
                         ? eat().type
                         : TokenType::Null;

        Identifier identifier = parse_identifier();

        if (currentToken().value == "=") {
            eat();
            Expr initializer = parse_expr();

            if (currentToken().type != TokenType::Semi) {
                std::cerr << "Invalid syntax, missing Semicolon symbol" << std::endl;
                exit(EXIT_FAILURE);
            }
            eat();

            return VariableDeclaration{"VariableDeclaration", identifier, type, initializer};
        } else {
            if (currentToken().type != TokenType::Semi) {
                std::cerr << "Invalid syntax, missing Semicolon symbol" << std::endl;
                exit(EXIT_FAILURE);
            }
            eat();

            return VariableDeclaration{"VariableDeclaration", identifier, type, nullptr};
        }
    } else if (currentToken().type == TokenType::Semi) {
        eat();
        return Stmt{"Semicolon"};
    } else if (currentToken().type == TokenType::Function) {
        return parse_function_declaration();
    } else if (currentToken().type == TokenType::While) {
        return parse_while();
    } else if (currentToken().type == TokenType::If) {
        return parse_if();
    } else if (currentToken().type == TokenType::OpenCurlyBrace) {
        return parse_block();
    } else if (currentToken().type == TokenType::Switch) {
        return parse_switch();
    } else if (currentToken().type == TokenType::For) {
        return parse_for();
    }

    return parse_expr();
}

Identifier parse_identifier() {
    return Identifier{"Identifier", eat().value};
}

Expr parse_expr() {
    return parse_additive_expr();
}

Expr parse_additive_expr() {
    Expr left = parse_multiplicative_expr();

    while (
        currentToken().type == TokenType::Plus ||
        currentToken().type == TokenType::Minus ||
        currentToken().type == TokenType::PlusEquals ||
        currentToken().type == TokenType::MinusEquals ||
        currentToken().type == TokenType::PlusPlus ||
        currentToken().type == TokenType::MinusMinus
    ) {
        TokenType operatorType = eat().type;
        Expr right;

        if (operatorType == TokenType::PlusPlus || operatorType == TokenType::MinusMinus) {
            right = UnaryExpr{"UnaryExpr", operatorType};
        } else {
            right = parse_multiplicative_expr();
        }

        left = BinaryExpr{"BinaryExpr", left, right, operatorType};

        if (currentToken().type == TokenType::Semi) {
            eat();
        }
    }

    return left;
}

Expr parse_multiplicative_expr() {
    Expr left = parse_primary_expr();

    while (
        currentToken().type == TokenType::Star ||
        currentToken().type == TokenType::Slash ||
        currentToken().type == TokenType::Percent ||
        currentToken().type == TokenType::StarEquals ||
        currentToken().type == TokenType::SlashEquals ||
        currentToken().type == TokenType::LessThan
    ) {
        TokenType operatorType = eat().type;
        Expr right;

        if (
            operatorType == TokenType::PlusPlus ||
            operatorType == TokenType::MinusMinus
        ) {
            right = UnaryExpr{"UnaryExpr", operatorType};
        } else {
            right = parse_multiplicative_expr();
        }

        left = BinaryExpr{"BinaryExpr", left, right, operatorType};

        if (currentToken().type == TokenType::Semi) {
            eat();
        }
    }

    return left;
}

FunctionCall parse_function_call(Identifier identifier) {
    expect(TokenType::OpenParen, "Expecting '(' after function identifier.");

    std::vector<Expr> args;

    while (currentToken().type != TokenType::CloseParen) {
        args.push_back(parse_expr());

        if (currentToken().type == TokenType::Comma) {
            eat();
        }
    }

    expect(TokenType::CloseParen, "Expecting ')' after function arguments.");

    return FunctionCall{"FunctionCall", identifier, args};
}

Expr parse_primary_expr() {
    TokenType tk = currentToken().type;

    switch (tk) {
        case TokenType::Identifier: {
            Identifier identifier = parse_identifier();

            if (currentToken().type == TokenType::OpenParen) {
                return parse_function_call(identifier);
            }
            return identifier;
        }

        case TokenType::Number:
            return NumericLiteral{"NumericLiteral", std::stof(eat().value)};

        case TokenType::Float:
            return FloatLiteral{"FloatLiteral", std::stof(eat().value)};

        case TokenType::MinusMinus:
        case TokenType::PlusPlus: {
            TokenType operatorType = eat().type;
            Expr operand = parse_primary_expr();
            return UnaryExpr{"UnaryExpr", operatorType, operand};
        }

        case TokenType::OpenParen: {
            eat();
            Expr value = parse_expr();
            expect(
                TokenType::CloseParen,
                "Unexpected token found inside parenthesized expression. Expected closing parenthesis."
            );
            return value;
        }

        case TokenType::_return: {
            eat();
            if (currentToken().type == TokenType::Semi) {
                return NodeType::_return{"_return", nullptr};
            } else {
                Expr expression = parse_expr();
                return NodeType::_return{"_return", expression};
            }
        }

        case TokenType::Char:
            return CHAR{"Char", currentToken(), value};

        case TokenType::String:
            return STRING{"String", currentToken(), value};

        case TokenType::Semicolon,
             return Stmt{"Semicolon"};

        default:
            cerr << "Unexpected token found during parsing!" << end;
            exit(EXIT_FAILURE);

    }
}

public:
    
Program* produceAST(vector<Token> _tokens) {
    tokens = move(_tokens);
    Program* program = new Program(AstNodeType::Program, {});

    while(not_eof()) {
        program->body.push_back(parse.stmt());
    }

    return program;
}

};
