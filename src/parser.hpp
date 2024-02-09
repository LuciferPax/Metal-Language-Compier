#pragma once

#include "tokenizer.hpp"
#include <variant>
#include <optional>
#include <iostream>


struct NodeExprIdent {
    Token ident;
};

struct LitType {
    TokenType type;

    explicit LitType(TokenType t) : type(t) {}
};

struct NodeExprLit {
    LitType l_type;
    TokenType tokenType;
};

struct NodeExpr {
    std::variant<NodeExprLit, NodeExprIdent> var;
};

struct NodeStmtExit {
    NodeExpr expr;
    NodeStmtExit();
};

struct VarType {
    TokenType type;

    explicit VarType(TokenType t) : type(t) {}
};

struct NodeStmtDeclType {
    VarType v_type;
    Token ident;
    std::optional<NodeExpr> expr;

    NodeStmtDeclType(const VarType& variableType, const Token& identifier, const NodeExpr& expression)
            : v_type(variableType), ident(identifier), expr(expression) {}
};

struct NodeStmt {
    std::variant<NodeStmtExit, NodeStmtDeclType> var;
};

struct NodeProg {
    std::vector<NodeStmt> stmts;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}

    std::optional<NodeExpr> parse_expr();
    std::optional<NodeStmt> parse_stmt();
    std::optional<NodeProg> parse_prog();

private:
    [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const;
    inline Token consume();

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};

std::optional<NodeExpr> Parser::parse_expr() {
    if (peek().has_value() && peek().value().type == TokenType::int_lit) {
        TokenType lType = consume().type;
        auto stmt_int_lit = NodeExprLit(LitType(lType), TokenType::int_lit);
        return NodeExpr{ stmt_int_lit };
    }
    else if (peek().has_value() && peek().value().type == TokenType::ident) {
        return NodeExpr{ NodeExprIdent{ consume() } };
    }
    else {
        // Handle the case when none of the conditions are met
        std::cout << "Invalid expression" << std::endl;
        exit(1);
    }
    return {};
}

std::optional<NodeStmt> Parser::parse_stmt() {
    if (peek().value().type == TokenType::exit && peek(1).has_value() &&
        peek(1).value().type == TokenType::open_paren) {
        consume();
        consume();
        NodeStmtExit stmt_exit;
        if (auto node_expr = parse_expr()) {
            if (peek().value().type == TokenType::open_paren) {
                consume();
            } else {
                std::cout << "Expected opening parenthesis" << std::endl;
                exit(1);
            }
            if (peek().has_value() && peek().value().type == TokenType::close_paren) {
                consume();
            } else {
                std::cout << "Expected closing parenthesis" << std::endl;
                exit(1);
            }
            if (peek().has_value() && peek().value().type == TokenType::semi) {
                consume();
            } else {
                std::cout << "Expected semicolon" << std::endl;
                exit(1);
            }
            return NodeStmt{.var = stmt_exit};
        } else if (peek().has_value() && (
                peek().value().type == TokenType::int_lit ||
                peek().value().type == TokenType::i8_lit ||
                peek().value().type == TokenType::i16_lit ||
                peek().value().type == TokenType::i32_lit ||
                peek().value().type == TokenType::i64_lit ||
                peek().value().type == TokenType::i128_lit ||
                peek().value().type == TokenType::isize_lit ||
                peek().value().type == TokenType::u8_lit ||
                peek().value().type == TokenType::u16_lit ||
                peek().value().type == TokenType::u32_lit ||
                peek().value().type == TokenType::u64_lit ||
                peek().value().type == TokenType::u128_lit ||
                peek().value().type == TokenType::usize_lit ||
                peek().value().type == TokenType::f8_lit ||
                peek().value().type == TokenType::f16_lit ||
                peek().value().type == TokenType::f32_lit ||
                peek().value().type == TokenType::f64_lit ||
                peek().value().type == TokenType::f128_lit ||
                peek().value().type == TokenType::fsize_lit ||
                peek().value().type == TokenType::char_lit ||
                peek().value().type == TokenType::bool_lit ||
                (peek().value().type == TokenType::String_lit &&
                 peek(1).has_value() && peek(1).value().type == TokenType::ident &&
                 peek(2).has_value() && peek(2).value().type == TokenType::eql))) {
            TokenType vType = consume().type;
            consume();
            auto stmt_decl_type = NodeStmtDeclType(VarType(vType), consume(), NodeExpr{NodeExprLit{ LitType(vType), vType}});
            consume();
            if (auto expr = parse_expr()) {
                stmt_decl_type.expr = expr.value();
            } else {
                std::cout << "Invalid expression" << std::endl;
                exit(1);
            }
            if (peek().has_value() && peek().value().type == TokenType::semi) {
                consume();
            } else {
                std::cout << "Expected semicolon" << std::endl;
                exit(1);
            }
            return NodeStmt{.var = stmt_decl_type};
        } else {
            return {};
        }
    }
    else {
        return {};
    }
}

std::optional<NodeProg> Parser::parse_prog()  {
    NodeProg prog;
    while (peek().has_value()) {
        if (auto stmt = parse_stmt()) {
            prog.stmts.push_back(stmt.value());
        }
        else {
            std::cout << "Invalid statement" << std::endl;
            exit(1);
        }
    }
    return prog;
}

std::optional<Token> Parser::peek(int offset) const {
    if (m_index + offset >= m_tokens.size()) {
        return {};
    }
    else {
        return m_tokens.at(m_index + offset);
    }
}

Token Parser::consume() {
    return m_tokens.at(m_index++);
}
