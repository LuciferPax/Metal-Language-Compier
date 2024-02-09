#pragma once

#include "parser.hpp"

class Generator {
public:
    inline explicit Generator(NodeProg prog) : m_prog(std::move(prog)) {

    }

    [[nodiscard]] std::string gen_expr(const NodeExpr& expr) const {
        struct ExprVisitor {
            void operator()(const NodeExprLit<int>& expr_lit) {
                output << "    mov rax, " << expr_lit.value << "\n";
            }
        };
    }

    [[nodiscard]] std::string gen_stmt(const NodeStmt& stmt) const {
        struct StmtVisitor {
            void operator()(const NodeStmtExit& stmt_exit) {
                output << "    mov rax, 60\n";
                output << "    mov rdi, " // TODO
                output << "    syscall\n";

            }
            void operator()(const NodeStmtDeclType& stmt_decl_type) {
            }
        };
        StmtVisitor visitor;
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]] std::string gen_prog() const {
        std::stringstream output;
        output << "global _start\n_start:\n";

        for (const NodeStmt& stmt : m_prog.stmts) {
            output << gen_stmt(stmt);
        }

        output << "    mov rax, 60\n";
        output << "    mov rdi, 0\n";
        output << "    syscall\n";
        return output.str();
    };
private:
    const NodeProg m_prog;
};